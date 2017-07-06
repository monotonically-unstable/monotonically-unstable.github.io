---
layout: english-post
title:  "Logging And History"
date:   2017-07-01 22:11:16
categories: programming logging
comments: false
---

# History of Logs

When developing software, we have a very specific term - 'log' which is used very often (mostly when errors occur). Logs are an inherent part of our software, and yet I feel that we are not giving them enough attention in terms of quality and teaching how to write good logs.

[In another post]({% post_url 2017-06-07-my_logging_practices %}), I describe some practices that help to produce better logs. This post talks about what is the purpose of logs, and their history.

## Origin of the Name

To understand what a log is and how to write good logs, I suggest taking a step back and take a look at our history. The word 'log' (with this meaning) comes from medieval time maritime world. The Navy has many traditions that originate in the old ages and some are still in practice today. 

So it will not surprise you that when [Spock logs his events](http://memory-alpha.wikia.com/wiki/Ship%27s_log,_USS_Enterprise_(NCC-1701)) to the "Ship's log" - he is using a term that is borrowed from the old naval world.

> "Ship's Log, Stardate 5148.7, First Officer Spock reporting. The Captain's course change has taken us deep into the Romulan Neutral Zone."

Back to history. Suppose you're trying to navigate from Italy to Spain. That's quite a complex task. When sailing, there is nothing around you other than blue water, so how do you know where you are? 

> Today, it's easy - one look at the GPS, and you have a pinpoint location.

Without electronics, the instruments and techniques for naval navigations were sparse. Let's review one technique, which is backtracking and correction. Suppose say you know your course and speed, and let's say you write that data down (log it) every few minutes. Now you can tell if you are heading in the right direction and get a rough estimate on how your path looks like. You could even backtrack your journey if you think you've made a mistake.

The place where you write your speed and course is [the Logbook](https://en.wikipedia.org/wiki/Logbook). The ship's logbook used to contain important events that happened during the journey (sounds familiar?), in addition to the navigation data. The name 'logbook' got its name from the instrument used to measure the speed - [the 'Chip Log'](https://en.wikipedia.org/wiki/Chip_log).

![]({{site-url}}/assets/chip_log.jpg)
> Photo Credits: Tim Rast

A primitive Chip log is a heavy piece of wood (hence the name) that was attached to a long rope. The rope is no ordinary rope - At every known interval, it had a Knot on it. When thrown into the sea, the rope would be pulled with the log (the Log is heavy and remains relatively still) and the sailors would measure 28 seconds and count the number of knots that have been pulled. (This is also where the standard [unit of measure knots](https://en.wikipedia.org/wiki/Knot_(unit)) comes from!) Now, with basic algebra, you can estimate your speed.

"Log" may also refer to a simple yet very old item - a journal. Journals were (and still are) a very important tool for scientific progress. When a scientist discovers something, or she has an idea, she logs it to a journal. Why does she log it? First, for data keeping, so it won't get lost. But also, so she can share it with other colleagues or her future self for further study of the subject.

> Journals also have other purposes. For example, journals are sometimes used for legal applications or for progress tracking. They are also used as historical documentations.

## The Present

Logbooks have existed for centuries, and are still in use today. Every ship is legally obliged to keep a logbook with entries such as who is onboard and when was the last equipment checks done. Unlike what is expected, the logbook does not keep navigation history, that is the job of the navigation system.

Another form of the logbook, that might be more familiar, is the ["Flight Recorder"](https://en.wikipedia.org/wiki/Flight_recorder), AKA the "Blackbox". The term Blackbox is also in use in other industries, but the term is borrowed from the avionics industry.

The black box is only used when accidents occur and contain all the information that the airplane/vehicle can collect using its sensors. Also, all the information it contains is gathered automatically, measurements such as altitude, height, and air pressure, but it also collects all the audio from the cockpit.

## Back to software

So what does all of that have to do with your software you ask? Well, here are a few takes on what we may learn from history:

0. A log should be managed like any other part of your system. Managed means that it's not only looked at when disaster strikes! Managed means that you are regularly checking your logs to see that WHEN disaster strikes - You are prepared! Managed means that you have a clear methodology of how to log ("log everything" is not a logging methodology by the way), that you monitor using automation and peer reviews. It should be part of your design.
0. Logs should be observed regularly. Just like the chip log or the logbook - you could have looked at it only when disaster struck, but that would probably be too late. Instead, you should look at it regularly, learn from it, evolve your software from it, take preventive actions according to what you see in the log. 
0. Not everything should be written to the log. The logbook does not contain the information from the navigation system, nor does it contain the number of dolphins you saw during the sail. Don't spam your log or you'll have to find a needle in a haystack. Tooling only help to some degree. 
0. *Noise* should be reduced by using other systems - just as navigation data is no longer written to the logbook, evolution occurred in software logs too. Some of the data we used to track into the log, we now track in other ways such as monitoring metrics (either applicative or not).
0. Log rotation - I haven't mentioned it, but logbooks were not kept in the ship for eternity. After a logbook ran out of pages, the master started a new logbook, while keeping the previous copy for inspection.
0. Logs may vary - Different applications have different needs. If you are building an application that alerts when your dog desires a snack, it probably has different logging requirements then an application used in hospitals or the avionics industry.

I believe that logs are not done evolving. The direction I see the industry moving to is automating more and focusing less on the text. Instead, there will be (there already is) more focus on visual aids such as graphs and pie charts. Machine learning will surely be used more for log analysis (and perhaps an AI will also decide what to write to the log?), as we are generating more and more data as time goes by, and humans are bad at processing huge amounts of data.

So, To answer the question we started with

> 'What is the purpose of logging?'

I think that the logs have two main purposes. The number one, de facto purpose of logging is error tracing.

But by looking at how we used logs so far in history, and how it is conceived outside of software; We can see that logs are also used for future analysis of your business process.

Therefore, you should keep that in mind that logging is for either error tracing or analysis of the business process. These conditions actually cover most of the cases, but what most people miss is how these messages are written. Instead of letting me know that `a file was saved successfully`, it should be more business related - maybe: `The backup finished successfully`, or `User settings are now ready for use`.