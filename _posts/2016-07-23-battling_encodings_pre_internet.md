---
layout: english-post
title:  "Battling Encodings - Part 1"
date:   2016-07-22 23:59:00
categories: programming
---

Encoding. One of these things you expect to be working like magic. While that might be a nice fantasy, in practice it's just the opposite; Encoding problems are great party poopers and they show up when you least expect them.

In this post. I'll try to give a quick review of what i believe is a fundamental basis that every programmer should know when facing encoding issues.

---

<a name="The_basics">[The basics](#The_basics)
==============================================

We all know that text is just an array of bytes in the memory, but so is everything else. The technique of representing text in memory is called **Encoding**. Let's review the simplest way to represent text in memory.

Suppose you have the following text - "Hello". You could simply choose an arbitrary number to represent each letter (and we already know how to represent integers in binary

+ 'H' = 72
+ 'e' = 101
+ 'l' = 108
+ 'o' = 111

 As you probably know, these numbers are not that arbitrary, I pulled them out of an ASCII table.

![ascii_table](http://www.asciitable.com/index/asciifull.gif)

As you can see, ASCII maps each 'symbol' to a well agreed upon number. This way these memory chunks representing could be saved to disks and used by two different programs with minimal headache. A symbol is not only a letter, but it could be a 'space', 'line break' or even a 'beep' which makes your computer beep when it is being read by some software. (Please not the difference between capital and lower cased letters - 'a' is not the same as 'A')

So now I can finally tell you that a **string** is just an array of bytes which have a certain encoding.

> **Remember - A string without an encoding is meaningless**

While ASCII was working for most English speaking users, it was not a full solution even in the domain of the English language. For example the '£' (pound) symbol is not part of the ASCII standard. This lead to the creation of some ASCII flavors which were just a little different from the original ASCII.

Let's talk a bit about the memory representation. An ASCII character is represented using 7 bits in memory, not 8 unlike the common misunderstanding. This allows packing 128 characters to the standard.
These days ASCII is transmitted over 8 bit bytes thus making the 8th bit of that byte be undefined.
This is important because it allowed various extensions of ASCII to be created without breaking existing code.

These types of encodings are known as **ASCII compatible Encoding**, And they are the subject of the next chapter.

> A small tip: most editors separate between the encoding with which you view the document and the encoding with which you write the document (You may be seeing text encoded using one encoding, but when you type letters, they are being coded with a different encoding). Remember this and it may save you and your colleagues precious time.

<a name="Pre Internet Era">[Pre Internet Era](#Pre Internet Era)
==============================================
So, ASCII was O.K, as long as you were in the USA. If your native language wasn't English, you probably wanted to write using non-English letters... how... surprising....

Let's focus on one of the most common families of encodings that came to solve this problem - **"ISO-8859-*"**. The ISO standard defines several encodings, each meant to be used at a different geographical area of the world. For example:

* Iso8859-1 - Latin (Western Europe) alphabet
* Iso8859-2 - Latin (Eastern Europe) alphabet
* Iso8859-6 - Latin/Arabic
* Iso8859-8 - Latin/Hebrew

There are 16 types of encodings in the ISO family and this rocked! It solved the problem just right.
The ISO encodings used the undefined 8th undefined bit in each ASCII character, this allows ISO encodings to be ASCII compatible. It is also very space conservative, just like ASCII it takes one byte only to represent a symbol.

ISO encodings were not the first non-ASCI encodings to appear on the market, but these encodings made a lot of impact on the market. Another example of a family of encodings is the windows encodings family, such as "windows-1252" or "windows-1255".

Let's talk a bit about "windows-1252". "windows-1252" is very similar to "ISO-8859-1" and it's the older one of the two. At the time, it was the most popular encoding, as it was used by default by all of windows components, but it was never standardized.

> Actually, this encoding was so wide-used and similar to ISO-8859-1, that most programs had bugs in them treating the two encodings the same. As of 2016, we are still dealing with this problem as HTML5 defines "ISO-8859-1" to be treated as "windows-1252"!

A few more words on these kind of encodings. The two examples above are fixed length ASCII compatible encodings. That means:

* Each character takes the same amount of bytes (1 byte)
* Any English character has the exact same value as if it were written in ASCII

That means that these encodings are compact and that any text written in English, is easily instantly convertible between any two ASCII compatible encodings.

---

This is it for this post. On the next post I'm going to cover the modern approach for battling Encoding with stuff like Unicode, UTF-8 and how not - Emoji :)
