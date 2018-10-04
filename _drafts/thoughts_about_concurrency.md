# Thoughts about concurrency 

This post begins with the question "can I implement goroutines in c++?", that's what my brain thought about during my trip. To india... Go figure. 

I wanted to answer this question, so I tried reading about goroutines , but the electricity in Palolem (and with it the wifi) had just died...so I had to work from memory, but YOU should definitely read about it [here] (http//something.com)

Goroutines are normally thought about as light weight threads, but what does it really mean? Well, goroutines have a much smaller stack size when compared to a native thread (2kb vs 1MB). This has a few positive effects of performance:

1. Startup time and context switch times are much faster.
2. We can spawn orders of magnitude more goroutines then threads before running out of memory.

Let's look at the typical example - opening a thread for every concurrent request. This approach does not scale because of the overhead involved with thread management. But with goroutines we can easily open a goroutine per request.

Back to my original question, can I mimic that behavior in c++? I can build a concurrent event loop that will dispatch events to threads from a thread pool. If each task has a payload smaller then the stack size of a goroutine it might even be faster!

But golang has another trick up its sleeve, consider the following example - each of our tasks might block, because of a long DB query for example. What would happen when all the tasks block at the same time? No new tasks will be created and requests won't get served. Our cpus are idle and we can even create new threads. We could increase the thread pool size, but that would have different implications. 

* Async operations exist to solve exactly that problem, but it has its own set of edge cases to keep in mind. 

Golang helps solve this problem. Every time a goroutine is about to block, the scheduler can pause its execution and allow the thread to queue a different task. The operation of rescheduling a goroutine is also simpler then rescheduling a thread not only because there the stack size is much smaller, but also because goroutines don't need to copy all the registers that a thread needs to (I'm not sure how this works, I belive that this is referred to as cooperative scheduling).

Let's get back to non-go world. Even if we had an automatic way of detecting when a function is about to block, switching context via library code will be much harder (I will hopefully try to do that in a later post, in c++) and slower.

How much of that behavior can we mimic, and how easily? If we can do the following, I think that we may solve the above problems:

1. Encapsulating the blocking operations and running them on different threads, and
2. Structuring our code as an ordered sequence of blocking/non-blocking tasks

In the follow-up posts I will attempt to write down the requirements for such a library and by that understand the problem better, implement a library with as little compromise as possible and learn a few things in the process.
