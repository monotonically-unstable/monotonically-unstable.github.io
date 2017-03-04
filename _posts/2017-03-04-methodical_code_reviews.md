---
layout: english-post
title:  "Methodical Code Reviews"
date:   2017-03-03 22:11:16
categories: programming
comments: true
---

# Methodical Code Reviews

Whenever I trained a new team member on how to do code reviews, I instructed two major rules. In short, they went along these lines:

1. Never make the other person feel bad about his/her work.
2. Never give a review note that is based on a gut feeling.

Let's take a deep dive into the second rule. The rule was a little more complex. Code reviews could reduce bugs [^1], but It's not why we did it. Assuming the code under review was correct (and tested), the reviewer was not looking for bugs but instead she was looking for other issues mainly related to maintainability. Thus a review note could only be given if it answers one of the following:

1. A clear [SOLID](https://en.wikipedia.org/wiki/SOLID_(object-oriented_design)) violation.
2. A well known issue. (for example - [rule of three violation](https://en.wikipedia.org/wiki/Rule_of_three_(C%2B%2B_programming)) or a hidden [design pattern](https://en.wikipedia.org/wiki/Software_design_pattern))
4. Performance or security Issue.
5. Violation of some internal policy or coding standard.
5. Readability issues, typos or naming.

Every rule, other then the last one, can be clearly identified and communicated over. If the reviewer saw some bad code, she had to give arguments which she can backup and not just say "I don't like this part". [^review_problems]

> The reviewer has to be backed up by stuff anyone could easily google online.

- *The purpose of these rules was that we started treating programming more like science and less like voodoo, magic or art. But more on that later.*

The last rule poses a problem. Unlike violations of known rules and principles, Readability can't be measured. This one should be pulled in as a last resort. And unlike the rest of the rules, it could lead to endless arguments over a small piece of code. It is just too ambiguous.

And yet, sometimes the code is just a big mess and you just can't put your finger on it. You can pull out the readability card, but it has a cost.

1. You must offer an alternative.
2. You may have to call a third person on the team to decide.

Giving an alternative is a must. It's part of mentoring and it's part of your obligation as a senior developer. Being able to call a third person has a hidden agenda. It should make you not want to pull out the readability card if you don't have strong arguments.

# Measurements

I think that we should stop treating the code as *beautiful*, *good* or *bad*. But instead it should be **efficient**, **simple** [^cyclomatic_complexity] or it might violate some rules.

We have to start speaking this language if we wish to solve the [Software Crisis](https://en.wikipedia.org/wiki/Software_crisis). As long as we don't measure, as long as we treat our [profession as a form of art](https://www.google.co.il/search?client=opera&q=programming+is+an+art&sourceid=opera&ie=UTF-8&oe=UTF-8), the crisis will continue.

When I started doing these methodical code reviews, it was not because of these reasons. It was because I was young. I was afraid that when opening my mouth and speaking my mind, no one would listen. It was only logical that backing up my review notes with Wikipedia articles and raw data will give an answer to that fear.

[Robert Martin's](https://en.wikipedia.org/wiki/Robert_Cecil_Martin) discussion about clean code sparked this notion into me. Robert starts the discussion with the question - *"What is clean code?"*, to which he tries to answer in various ways. Later, I went to one of [Juval Lowy's](http://www.idesign.net/about) lectures, in which he talked about the software crisis and how as software engineers we need to take our profession much more seriously.

When you use any methodology while working, you are already in a better place. You can measure it (code review effectiveness in this case), You can practice it, and you might even be able to automate it.

Being methodical means you are more scientific. You are not doing voodoo when you do code reviews. And unlike voodoo, you can teach it to others.

Unless of course, you know how to teach voodoo, in that case, please, TEACH ME! 

---

[^review_problems]: Iv'e seen arguments go for hours about specific review notes. All because the review note was not based on data or evidence.

[^1]: [Microsoft published this article](https://www.visualstudio.com/en-us/articles/devopsmsft/code-reviews-usefulness) in which they claim code reviews could find 15% of the bugs, while [wikipedia](https://en.wikipedia.org/wiki/Code_review) states higher numbers, but with no sources.

[^cyclomatic_complexity]: [Cyclomatic complexity](https://en.wikipedia.org/wiki/Cyclomatic_complexity) is used to measure program complexity.