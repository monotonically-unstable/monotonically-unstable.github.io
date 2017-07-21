---
layout: english-post
title:  "Battling Encodings - Part 2"
date:   2016-08-19 23:59:00
categories: programming
comments: true
author: "Dor Avraham"
---

<a name="Title">[Encoding In The Modern Era](#Title)
===================================

In my [previous blog post]({% post_url 2016-07-23-battling_encodings_pre_internet %}) I talked about Encoding in general and mentioned specific encodings used in the pre internet Era.

Now I want to talk about how it's done in the real world. As a reminder, I mentioned two popular encoding families used for non-English languages. These families had the problem of being regional, they would not allow two different regions to communicate (such as Japan and French).

<a name="Unicode">[Unicode](#Unicode)
-------------

Let's start with the most common mistake - **Unicode is not an encoding**, even Bart knows that:

![Bart knows that Unicode is not an Encoding]({{site-url}}/assets/bart_writes_unicode_not_encoding.jpg)

Now that that's out of our way, what **is** Unicode?

Well, Unicode is a pre defined mapping that maps characters (or more specifically "symbolic names") to unique integers called **Code Points**.  Unicode now contains 1,112,06 symbols (as of 2016... according to wikipedia...)

> Unicode started from an earlier attempt called UCS (Universal Character Set) to standardize a "perfect" 2 byte length encoding for 2^16 = 65,536 characters. It soon became very clear that it would not be enough characters

So... How is it different from encoding? Encoding tells us how to write a character to file (encode it) and how to read it back (decode it), it also tells us how to keep that character saved in memory. Unicode tells us how to name that character in a standardized unambiguous way.

As the internet exploded, this issue was a burning topic. As it seems, it's not that easy to even talk about Encoding. For example, is this "ß" (German letter often written as "ss" by English speakers) a character? Or is it two characters? What about "קֻ" (The Hebrew letter "ק" with notation called Niqqud), is it a single character? What about some letters in the Arabic language that appear differently when written at the end of a word? Emoji?

Unicode gives an answer to all these questions, anything defined as a code point is a character. Now, we can give Encodings another definition - they translate Code Points to binary.

Because Unicode code points are unambiguous, a lot of tools support code points as input, for example - you could use the code points value directly in html using the prefix "&#",  like so "&#9824", or sometimes even the symbolic name "&#spades", which should both result in the :spades: emoji.

<a name="UTF-8">[UTF-8](#UTF-8)
-------------

We still did not solve the previous post's problem - how can we view French, German and Japanese letters at the same time without working too hard and keeping backwards compatibility with ASCII?

ISO-8859-* family was a family of fixed length encodings, meaning each character "costs" a known amount of bytes (1 byte in the case of ISO).  Being able to only store 1 byte for each character forced ISO to split into a regional encoding system to support as many characters as possible.

UTF-8 solved both the backwards compatibility issue and the problem of not being limited to a small number of symbols by being a **Variable Length Code**. This means that each character does **not** take a known amount of bytes to encode.

In fact, UTF-8 solved this problem so well, that as for 2016 it accounts for 87% of all digital texts (according to wikipedia.. bla bla disclaimers bla bla...). How so? Well, most texts did not require any change to become UTF-8 as they are written in English and UTF-8 is indeed ASCII compatible. It is also very space conservative and handles different endianness.

So, as you may have guessed if you read the previous blog post, basically UTF-8 uses the 8th bit in an ASCII byte to represent weather the next byte is part of the same character or if it's a new character.

> The description above is over simplified. For the first byte, the 8th bit is indeed an indicator, but as for the following bytes, the story is a bit messier. UTF-8 is limited to 4 bytes and is not optimal in terms of space, but it packs other features in return. For example you could start reading at an arbitrary byte and know if it's the first byte or a character or not. You can find more information in the specification, but really - it's quite boring...

<a name="UTF-16_UTF-32">[UTF-16 and UTF-32](#UTF-16_UTF-32)
-------------

Even though UTF-8 is obviously more popular, I feel like this would not be a complete post without mentioning UTF-16 and UTF-32. These Encodings also provide a solution for representing all possible code points. These encodings are not ASCII compatible and unfortunately you can still find people using them... mostly because of bad defaults.

UTF-16 is also a variable length encoding only that it grows by two bytes at a time. It evolved from a UCS-2 (fixed length encoding where each character costs 2 bytes) proposal. It later became a default encoding for some Windows text API and the default encoding used in Java.

UTF-32 is a fixed length encoding where each character takes exactly 4 bytes of memory, which is very wasteful in most cases.

Both of these encodings provide a nice problem of "nullability". You could not simply iterate byte by byte over a UTF-16 or UTF-32 string and look for a null terminator as these encodings allow for the 2nd (or 3rd and 4th) bytes to be "null" (the representation of 0 in binary). So you need to give special care when iterating over strings encoded in UTF-16 or UTF-32 (iterate using [wide characters](https://en.wikipedia.org/wiki/Wide_character)).

----

This was the second part of a three part blog post about Encoding. In the [next post]({% post_url 2016-09-11-battling_encodings_practical%}) I'll cover some real world examples and techniques for handling encoding without loosing your frikin' mind.

If you enjoyed reading, let me know in a comment below. If you didn't enjoy reading you should also let me know in a comment below :)
