---
layout: english-post
title:  "Battling Encodings - Part 3"
date:   2016-09-10 23:59:00
categories: programming
comments: true
author: "Dor Avraham"
---

<a name="Title">[Encoding In The Real World](#Title)
====================================================

So far I've been writing about what [Encoding is, how it evolved and covered some specific encodings]({% post_url 2016-08-21-battling_encodings_internet_era %}). Now I wish to share some practical knowledge of how to deal with Encoding problems once you see try to encounter them.

<a name="Rule of thumb">[Rule of thumb](#Rule_of_thumb)
-------------------------------------------------------
Remember this, from the first part of this blog post?

Remember - A string without an encoding is meaningless
{: .blockquote }

If there is one thing to remember from this blog post, it's this. Whenever you see a dangling string out there in the wild, stop. Ask yourself, in what encoding is that string encoded in?

This issue comes up a lot when dealing with legacy code. Hours had been spent over hard-coded strings written to files with the wrong encoding, or non-portable code reading strings from file without expecting any particular encoding. That's not even the worst of it. When you have to deal with 3rd party legacy code that you have to keep supporting, it sometimes sends strings with different encodings over the same field!

So just remember, if you see a string, approach with caution. (Especially if you are told **"Yeah we save all the strings in Unicode"**, that's when you remember that men ARE allowed to cry. #4 on the list of "When men are allowed to cry")

Another handy rule is to apply the reversed rule for files. Any file could be read using any Encoding. A file is not bound to a specific encoding and it could make sense on several encodings at the same time (All ASCII compatible encodings for example)
{: .blockquote }

<a name="Dealing_with_it">[Dealing with it](#Dealing_with_it)
=============================================================

Now that you have entered this dark dark place, let's look at a few things that could help you shine your way out.

<a name="Guidelines">[Guidelines](#Guidelines)
==============================================

1.  Never use the defaults

	Most programming languages allow you to easily read strings from files without specifying an Encoding.

	`reason:` This creates non-portable code because such defaults may depend on environment variables and will act differently from computer to computer.

2. Keep your code ASCII encoded, Non-ASCII goes to configuration files

	Unless there is a very good reason - your code should only contain ASCII characters.

	`reason 1:` Strings encoded into the code have higher risk of changing by chance (via refactoring or copy-pasting).

	`reason 2:` Non ASCII data is usually used for UI and should not sit with the business logic. It also has the tendency to change more often, so you want it in configuration any way.

	If you must write Non ASCII characters, write Using Unicode numeric code points.

	`for example, the unichar class in python`

3. Choose a default for your project

	`reason:` When everyone are working with a specific encoding, it's easy to discuss over the issue and identify problematic points. You can also make global changes a lot easier.

4. Only treat Encoding on input and output

	`reason:` following the last item, if you standardized your encoding for the project, you should only use different encodings on the end points of your program. It also helps to avoid scrambling two encodings together.
	These should be enforced and not based on defaults (as item #1 states)

5. Use UTF-8 whenever possible

	`reason:` UTF-8 is ASCII compatible, compact and most importantly, it's very popular. Most modern libraries support UTF-8 out of the box.

6. Specify the encoding inside the file, not the program

	for example, in XML files you have the header `<?xml version="1.0" encoding="UTF-8"?>` which notifies the parser how to treat the file.

	`reason:` This allows you to transfer the files between systems and improves readability. It's also easier to change that way.


<a name="Editors">[Use multiple editors](#Editors)
-------------------------------------


While using your favorite IDE will certainly make you write code faster and browse it faster, it might not be optimal for dealing with encoding issues. It might hide away the issues or it may make it worst.

### Vim

Normally, I use vim as my editor, but whenever I need to deal with encoding problems - I prefer to avoid it. Vim is great, but it has too many settings to get you confused.

first, there is

	:set encoding=

But then you find out that there is

	:set fileencoding=

Which obviously does not do the same thing. Now combine it with the fact that you need to reload the file after changing the encoding settings and it could drive you mad.
{: .blockquote }

Basically, that's what the settings do:

1. `:set encoding` - the displayed encoding
2. `:set fileencoding` - controls the encoding in which you write

Note that after changing `encoding` you need to reload the file (`:e` will do the work).
There is another feature of vim that I recommend turning off. Vim is capable of "guessing" which Encoding you are seeing and change the display accordingly. It only tries to guess from a predefined list of Encodings which is defined using... `:set fileencoding` (why vim? why?)

I recommend setting `:set fileencoding=utf8` or another specific encoding in your vimrc file. It makes it harder to read sometimes, but at least you are consistent and when you see some gibberish you know something is wrong with the file (instead of seeing everything OK and not knowing that the file should be changed).

### Notepad++

The problem with vim is - it does not keep things simple. The great thing about notepad++ is that it keeps things simple. You just go to the menu and choose the encoding in which you view and edit the files. Especially when working on windows, this is my to-go editor to view non-ASCII files.

### Eclipse And Visual Studio

I'm only mentioning these two for completeness, I do not recommend using them when trying to analyze encoding problems.
{: .blockquote }

These are the two major IDE's at my work place. You should always be on the lookout when saving files via these two.

Eclipse makes it a lot easier to tell what you are looking at. Visual Studio on the other hand makes your life hard. Both of them use some environment variables to determine the defaults. This means that they may act differently on two different computers.

Eclipse has a nice feature that allows it to edit different files using different encodings and save these settings to the project file as well as project specific and workspace specific settings.

<a name="Other">[Worth To Mention](#Worth_To_Mention)
-----------------------------------------------------

### Python
Let's explore some python code:

	>>> some_string = u'shalom'
	>>> some_string = u'\u1234abc'
	>>> some_string = unichar(1234) + u'abc'

These, are a Unicode encoded string. Python saves each character as 16 or 32 bit integer, which allows it to represent the Unicode code point directly for every character.

Python forces you to use the correct encoding by not allowing strings to contain non ASCII characters implicitly. Any API that involves string construction enforces these rules. By not allowing defaults that "just work" out of the box, python enforces the first item in the list above.

Now, what happens with this kind of code?

`>>> some_string = u'你好，世界'`

If you followed my rule of thumb, this should raise some red flags. These are non ASCII characters, declared to be Unicode... but how are they encoded?

Remember - A string without an encoding is meaningless
{: .blockquote }

Python defaults to UTF-8 for the Unicode constructor. This is a problem if your editor defaults to ISO-8859-* or windows-12*.

Now, if you really must keep these strings in your code, add this to the head of the file:

`# -*- coding: utf-8 -*-	`  (remember item #6 on the list of guidelines?)

This will cause the python interpreter to treat the strings as utf-8 and will inform your co workers (and future you) that this file should only contain utf-8 encoded characters (unless you want gibberish).

### Java

Java takes a similar approach. All strings are in UTF-16. This makes things simpler.

### Csharp

That's funny. C# allows you to declare classes using Unicode characters. I once saw classes with Hebrew names. I wonder if it also supports emoji as class names? :pray:

---

So, that's it about encoding. I think this blog post should give you a solid ground for handling encoding issues that you may run into.
