---
layout: english-post
title:  "Donwloading video streams"
date:   2016-01-05 23:59:00
categories: bash
comments: true
author: "Dor Avraham"
---

<a name="the-problem">[Problem: watching lectures offline](#the-problem)
=======================================================

So I had this problem of watching my university's lectures. It is really, really boring. To solve this, I was using a Firefox add-on and other software to fetch these streams and watch them on my phone while on the bus at 2x speed.

Recently, the (evil) university changed the streaming format and now the add-ons no longer work :(

<a name="digging-in">[Digging In](#digging-in)
----------------------------------------------

As I could not find any other tool for the job, I began doing research.

Using Firefox debugging tools i found out that the player was fetching hundreds of files with the extention ".ts". This is called [Transport Stream](https://en.wikipedia.org/wiki/MPEG_transport_stream) which is basically just mpeg2.

Hey, wait a second! Hundreds of files fetched and played by the player? Looking at the addresses of the files, they looked like this:

> http://.../.../media_b400000_333.ts

You are probably thinking the same thing I thought, What would happen if i type that in chrome?... Well, look and behold! I got a 10 seconds video.

Well... That 333 looks pretty... hmmm... Inviting; Maybe i should try fetching all the files from 1 to 1000 and see what happens.

    for i in {0..1000}; do curl http://.../media_b400000_${i}.ts > media_b40000_${i}.ts

That's pretty straightforward. Now how can i play it? Opening each file after another is not really possible.

<a name="take1">[Take 1 -  ffmpeg and curl](#take1)
----------------------------------------------

Researching the subject i found [this question](http://stackoverflow.com/questions/7333232/concatenate-two-mp4-files-using-ffmpeg).

So i tried this:

    ls *.ts > fileList.txt
	sed -i "s@\(.*\)@file '\1'@" fileList.txt # making sure the syntax for ffmpeg is correct

	ffmpeg -f concat -i fileList.txt -acodec copy -vcodec  copy all.ts

There's a small annoying **bug** here, can you spot it?
Using `ls *.ts` does not guarantee the correct order, it actually promises lexical order which is wrong (100.ts is right after 1.ts, should be 2.ts).
The simple solution is to use the same for loop as before instead of a counter.

    for i in {0..1000}; do echo "file 'media_b400000_${i}.ts'" >> fileList.txt; done

Now the combined file just needed to be converted to another more comfortable format (ts is crap :))

    ffmpeg -f concat -i fileList.txt -acodec copy -vcodec copy -bsf:a aac_adtstoasc all.mp4

<a name="take2">[Take 2 - Speeding things up](#take2)
-----------------------------------------------------

So using that script takes about 2 hours! That's way too long... Especially with my unstable computer.

Quick tests show that `curl` takes most of the time. There are two issues here, first is speeding the process up. The second is recovering after a crash without restarting everything and fetching all the files.

About speeding it up, well, it's easy! Just make the process parallel.
First relatively trivial way is to split it into 4 sequential chunks of about 250 parts each. Like so:

    fetch_files()
    {
    	for i in `seq $1 $2`; do curl http://.../media_b40000_${i}.ts > media_b40000_${i}.ts; done&
    }

	fetch_files 0 250&
	fetch_files 251 500&
	fetch_files 501 750&
	fetch_files 751 1000&

But this can be improved, let's make it configurable and split it into `N` parts... and let it fail with good error messages and... and... wait a minute!

Sounds like I'm reinventing the wheel here, which will obviously be invented again being "square-ish", which is not really what I'm after.

<a name="gnu-parallel">[`GNU-Parallel` to the rescue!](#gnu-parallel)

[GNU-Parallel](http://www.gnu.org/software/parallel/) is a wickedly awesome tool that knows how to receive input like `xargs` only it can run the command on each input asynchronously and it's highly customizable.

So now i reduced the problem to running GNU-Parallel with the correct parameters, like so:

    time cat fileList.txt | parallel --linebuffer --joblog --eta -j 10 "script_that_uses_curl.sh {} http://.../"

 1. --linebuffer : causes `parallel` to print the output in the correct order, without mixed lines from multiple processes
 2. --joblog: allows me to get a log file of which jobs run and how long each one took
 3. -j 10: how many threads should be running. This can be configured to use a percentage of cores your CPU has
 4. {} - The current input `parallel` works on (passed as a parameter to the script)

`parallel` has many more features, it can even export your bash variables and functions! (that is, if you are running other bash scripts)

This now takes 15-20 minutes! That is more than 80% improvement.

* The full scripts i will be linked at the bottom of the page.

<a name="curl-fail">[Curling fails](#curl-fail)

So after running this on a few videos I noticed that some of the files have the size `0 bytes`.  This could happen for example if curl fails (might be the server is down, no internet connection, etc).

Checking the return value of curl and weather the file size greater than 0 is good enough for most of the cases (100% of the cases i ran into), if the check fails we can just retry.

`fetch_part.sh`:

    #!/bin/bash

    BASE_URL=$2
    TEMP_NAME=${1}_not_yet_ready

    fetch()
    {
	    curl ${BASE_URL}/$1 > $TEMP_NAME && [ -s "$TEMP_NAME" ]
    }

    echo "Fetching ${1}..."
    while ! fetch $1 ; do
            echo "Something went wrong, failed fetching $1, trying again in 2 seconds"
            sleep 2
    done

    echo "Done fetching $1, renaming the temp file"
    mv ${1}_not_yet_ready ${1}

Using temporary files will later allow me to recover properly by allowing me to detect partially fetched files (the script stopped mid-run)

* `curl` can be configured to retry every X seconds, or until a threshold is reached... but when I wrote this, I was testing `wget` too, so this helps toggling between the two more easily.

<a name="take3">[Take 3 -  Playlist](#take3)
--------------------------------------------

When trying to fetch other videos i noticed that they are of varying lengths. Trying to figure out how to do it automatically, I noticed that the player is trying to access the address `http://.../playlist.m3u8`
It looked like this:

    #EXTM3U
    #EXT-X-VERSION:3
    #EXT-X-STREAM-INF:BANDWIDTH=1200000
    chunklist_b1200000.m3u8
    #EXT-X-STREAM-INF:BANDWIDTH=800000
    chunklist_b800000.m3u8
    #EXT-X-STREAM-INF:BANDWIDTH=400000
    chunklist_b400000.m3u8
    #EXT-X-STREAM-INF:BANDWIDTH=48000
    chunklist_b48000_ao.m3u8


This is basically a reference to other playlists depending on the chosen video quality. When fetching these playlists they look like this:

    #EXTM3U
    #EXT-X-VERSION:3
    #EXT-X-TARGETDURATION:11
    #EXT-X-MEDIA-SEQUENCE:0
    #EXTINF:10.0,
    media_b400000_0.ts
    #EXTINF:10.48,
    media_b400000_1.ts
	.
	.
	.
	media_b40000_1030.ts

The chucklist can be loaded using VLC for example. That lead to parsing the chucklist and fetching the files directly afterwards.

	curl $BASE_URL/playlist.m3u8 > playlist.m3u8
	CHUNKS_FILE=$(cat playlist.m3u8 | grep -v '#' | grep $QUALITY) # Quality=40000
	CHUNKS_URL=$BASE_URL/$CHUNKS_FILE
	curl $CHUNKS_URL > $CHUNKS_FILE

	CHUNKS_ARRAY=(`cat $CHUNKS_FILE | grep -v '#'`) # this is a bash array

`CHUNKS_ARRAY` will be passed as input to `GNU-parallel`

<a name="say-what-now">[Wait, parsing? Why ?](#say-what-now)

So yeah, given the chucklist we can just load it using ffmpeg like this :

	`ffmpeg -i http://.../chunklist_b400000.m3u8 -c copy i_am_complete.ts`

So why the hell not do that? Well... It takes about 2 hours again because ffmpeg fetches the files sequentially.

<a name="take4">[Take 4 - Recovery](#take4)
--------------------------------------------

There is an interesting additional benefit here to this design. I can easily check if the file had already been fetched properly in an earlier run. If the file exists, it does not need re-fetching.

	recover_needed_chunks()
    {
            CHUNKS_TO_FETCH=()

            for chunk in ${CHUNKS_ARRAY[*]}; do
                    if [ ! -s "$chunk" ]; then
                            echo $chunk does not exist
                            CHUNKS_TO_FETCH+=($chunk);
                    else
                            echo $chunk exists, not fetching again
                    fi
            done
    }

This means i can reboot the computer, disconnect from the Internet, etc and not waste precious time re-fetching the chunks.

<a name="wrapup">[Wrapping things up](#wrapup)
==============================================

You can find all the code here (TBD).  


----------


I hope that by the time you are reading this, i have the comment system set up. So if you enjoyed my writing, if you hated my writing or if you know how to use the force, let me know in the comments!

If i did not set up the comment system by now (*choose random excuse*), I'd still appreciate the input, you can always mail me - <dor.a2003@gmail.com>.
