---
layout: english-post
title:  "My Logging Practices"
date:   2017-05-23 22:11:16
categories: programming logging
comments: false
---

# My Logging practices

Any one who ever worked on a legacy system (you know, that code you 'just' wrote, 3 months ago) knows the importance of good logs. 

Seeing the right log message when looking for a bug could save you hours of debugging time. Distracting log messages cluttering the log could achieve the opposite effect. 

Let's start with some common pitfalls to see what could go wrong:

## Types of log files 

### The 'void'

At the extreme, a 'void' log file might look something like this:

> 2017-08-25-19:13:44 Info Process started successfully! 

That's it. That was the entire log file. 

I'm pretty sure no one ever said "Oh boy, I wish my log would look like that one!". If you hear anyone saying that, you are looking at a sadistic maniac and you should back away slowly...

The void might take other forms, like writing the same obsolete log message, that may have been relevant years ago, but right now it's just noise. 

### The Astronaut - 'Houston we have a problem' 
> 2017-08-25-19:13:44 Error Process starting
> 
> 2017-08-25-19:13:44 Error Initializing map of value
>
> 2017-08-25-19:13:44 Error Values initialized
>
> 2017-08-25-19:13:44 Error Usimg values for random calculation: 438, 276, 95, 8731...
>
> ...
>
> 2017-08-25-19:13:44 Process started successfully!

At the complete opposite of the void, there is the Astronaut which insist on reporting everything to the log (most of the time reported as errors). 

An experienced developer might value from them, as she is looking at the relevant code. But what about a junior developer? A developer from another team? Ops guy? A manager? The log will not make sense for them. 

The Astronaut sometimes manifests itself after a manager decided that "we need to log EVERYTHING". Developers thereby do as they are told and log what **they consider everything**. 

Another great benefit of the astronaut is the angry phone calls in the middle of the night, as the program puts a high contention on the IO and takes much more storage then needed. The ops team  LOVES that! 

### The philosopher

Our last category of logs are the philosophers. Everything that gets written is either so abstract or generic that it could fit to pretty much any context.

The philosopher has an interesting source. Generic messages such as :

> 2017-08-25-19:13:44 Info File saved successfully 
>
> 2017-08-25-19:13:44 Info Timer activated
>
> 2017-08-25-19:13:44 Error - received invalid request 
>
> 2017-08-25-19:13:44 Error - NullPointerException

These are actually low level details!

* I don't care that the file was written successfully, I care that the backup processes ended and where the backup is saved. That is the business logic I'm after, the file is just the low level mechanism.
* Normally I would not care only that a request was invalid, but also the request details are important and why was it not valid. 
* etc

That isn't to say NullPointerException isn't helpful in a log, but I would much rather see the business logic that failed. Just google NullPointerException + popular framework and you will find many places that it had helped people, but normally it had a stack trace attached and more data for the reader to work with.

## Things to remember 

Talking about the bad is relatively easy, let's talk about the good.
I have a few guidelines that helped me, but they are not complete.
There is much more to the subject which I intend to write about in future posts.

Just don't take any of these guidelines as rock solid rules. 

### Production logs should not contain only errors 

Remember that not all bugs result in an error. When bug hunting (or just analyzing the logs)
seeing (or not seeing) events will allow you to compare with previous versions.
Well written log message should also help with understanding execution flow. 

### The log is not meant for you! 

You may be the one writing the log, but just like with code, you are not the only reader.
When in production, the first people to see logs are the ops team or on-call developers. 

You don't want be called everytime something breaks,
hence you should be writing the log as clearly as possible *for them*.
You can even ask them to review your log files, they may have interesting insights... 

In my opinion, you should be aiming the logs for the operation team.
I'll be writing more on that in a future blog post.

### Test your log! 

Logging is just like any other production code, it needs to be tested.

* Are the log messages *correct*?
* Are the log messages *clear and short*?
* Are the log messages *Spamming*?
* Are the log messages *Sufficient*?
* Are the log messages *Secure*? (Should not contain any sensative information)
* Conformity to other log messages in your app/process.

When starting out you want to consult others, 
the log may look good to you, but horrible for others. 

### Maximize Information While Minimizing Logging

As i stated above, in my opinion you should be aiming the logs to help
the operation team. That means that you should minimize logging
and instead you should write more data to other instruments that are less verbose.

The log should be used as a last resort. Maximize the usage of Graphs,
Pie charts and other visual aids.

These tools must support a timeline, you will find out interesting stuff:

* Why is there a network peak at exactly 18:54 everyday?
* Is it regular that this process is running at 100% cpu?
* The new manager asked to stop everything and work late hours
  to investigate a bug that appears to exist for several years.

I witnessed all the above, could have easily been resolved with a basic timeline presentation.

Don't choose such a tool that does not give you a good timeline.

---

## Summing up

Logging is an important part of your project,
just like with code you should have a clear methodology and peer review the output. 

I couldn't find any silver bullet for logging (again, just like with code),
but I think I've noticed repeating patterns that are harmful/useful
which I'm going to continue writing about in future posts.
