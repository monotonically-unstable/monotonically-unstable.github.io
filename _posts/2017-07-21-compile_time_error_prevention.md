---
layout: english-post
title:  "Compile time error prevention"
date:   2017-07-20 22:11:16
categories: programming design
comments: false
author: "Dor Avraham"
---

# Compile time error prevention

## Expectations

Normally in code when writing an API, we want to communicate to users (and maintainers), the purpose of our function parameters. To achieve this we use common tools, such as:

1. Proper naming (we name parameters, functions, and classes)
2. The type system (the class, return, and parameter types)
3. Documentation (if you are lucky enough to find any)

While most of the time these tools are good enough, I would like to discuss the times when these tools fail miserably.
These failures occur because of a mismatch between reality and expectation. The trivial example in most programming languages would be the simple arithmetical calculation of 5/2; Most people would expect the result to be 2.5. But give that to a programmer and he will probably tell you that the answer depends on the language.

> In C++ doing that division would result in int division. In javascript, this would result in floating point division. While in python, it depends on which version you are using; python2.7 would result in int division while python3 would result in floating point division.

This is a basic failure of meeting expectations. We wired our programmer's brain so tightly to avoid this mismatch of expectation so this seems like no big deal. Let's look at more examples:

    void sleep(double time);

Did the author mean time in seconds? maybe milliseconds? Again we can see the same pattern, I bet that most readers would argue that this is better:

    void sleep(double seconds);

And I would have to agree. It is better. But there's still plenty of room for error (I've seen enough sleep functions used incorrectly). But even without errors, we now need another function with a different signature for using `sleep` parameterized with milliseconds. This leads to writing more code, more documentation, and to some [very popular questions on stackoverflow](https://stackoverflow.com/questions/4184468/sleep-for-milliseconds). So I would claim that we did not solve the problem.

Furthermore, we can still see a mismatch in the implementation (This could also be a mismatch in the documentation). Consider the following:

    // In Header: void sleep(double `seconds`);
    // In Source: void sleep(double `milliseconds`) { ... }

There are more pitfalls to this variation, but I feel that this example is well understood. This is also a solved problem in c++11, you would write `std::this_thread::sleep_for(2s)`, other languages solved it differently or left it unsolved.

Expectation mismatch appears in more common places. Such one place is the object life time. This constructor expects a string:

    Person::Person(const std::string& name) { ... } 

Do you know what is the expected lifetime of the `name` parameter just by looking at the object? The `Person` class may keep a reference to the string which could result in a dangling reference. Garbage collection solves this specific issue, but of course not all of it. These kinds of problems belong to the resource allocation category and could also happen with events that you forgot to unregister or a file you closed too early; so your program is not protected even if it's garbage collected.

To close up the list of examples I'll present the final example which is also the main topic for this writeup.

Suppose we have the following declaration:

    void deliver_kitty_to(double latitude, double longitude);

Pretty straight forward. The code that used it was something along the lines of:

    ...

    const auto& owner = db.getOwnerOfCat("garfield");

    deliver_kitty_to(owner.longitude(), owner.latitude());

    ...

HOLY SHIT.

---

## Discoveries

To understand the implications of that simple mistake, you need to have context. We discovered it in a 20 years old legacy system in the navy.

This piece of code was no joking matter. Once discovered the managers ordered a complete analysis of the code (20 years old code-base) and the developers discovered 33 mismatches of such.

> Thank god no kitty got to the wrong destination.

The problem was not with the names nor was it with the documentation. The actual problem with that function was with the type-system.

> There was another problem, and that was test coverage. But as that problem could also get repeated in the tests, I'm not satisfied with just better tests. I want the problem solved from the root.

After realizing that the problem was not lat/long specific, we started seeing it in other places too. Meters got converted to yards, km/h to knots and other fallacies (even int to bool).

---

## Solutions

We already established that better naming and/or documentation could not solve this problem alone. We even tried it. A big refactor to our coordinations conversion library was made and lectures about correct usage of that code were given to the relevant people. That didn't help.

To solve this problem, let's first assume that we want the following syntax:

    void deliver_kitty_to(Latitude lat, Longitude xlong);

Now we just need to find a way to make that faulty code line not compile.

We can't use a typedef because typedefs are just aliases to the same underlying type; thus allowing implicit conversions. The trick is to create new types for lat/long with explicit constructors, like so:

    class Longitude
    {
    public:
        explicit Longitude(double xlong);

    private:
        double xlong;
    };

Will that solve the problem? Yes. The faulty code will not compile. But these classes require more functionality before we can use them.

    class Longitude
    {
    ...
        double value() const;
        void operator==(Longitude other) const;
        void operator!=(Longitude other) const;

        // Basically lots more methods
    ...
    };

Are we done? Nope. There's too much code duplication between the classes. We need such a class for Lat/Long, Meters/Feet/Yards and Km per hour/Knots and any other type.

    template <class Tag>
    class Numeric
    {
    public:
        explicit constexpr NumericValue(double value)
        : _value{value}
        {
        }

        constexpr double value() const {return _value;}
        explicit constexpr operator double() const {return value();}
        
        ... // operators and other methods

    private:
        double _value;
    };

    struct LongitudeTag {};
    struct LatitudeTag {};

    using Longitude = Numeric<LongitudeTag>;
    using LatitudeTag = Numeric<LatitudeTag>;


This is the basic idea, it's very similar to how std::chrono works. You can see the full implementation we chose [in the units library](https://github.com/saroad2/units/tree/master/units_cpp). The library ready for use and already saved us a few bugs and increases our accuracy for units conversion by several factors! 

Sagi took the basic idea I presented here to the extreme and he is now in the process of migrating his work to GitHub. The units library gives good examples of how to write safer code and I recommend you take a look at it if you want to dive deeper into the subject. We would, of course, appreciate any feedback.

You may also want to note the [boost units Library](http://www.boost.org/doc/libs/1_64_0/doc/html/boost_units/Examples.html#boost_units.Examples.DimensionExample) that we found not suitable for our needs, as it was not as easily customizable. The boost library works differently and does more template metaprogramming magic. The syntax for boost is (taken from the docs):

    quantity<length> L = 2.0*meters;                     // quantity of length
    quantity<energy> E = kilograms*pow<2>(L/seconds);    // quantity of energy

This problem is not C++ specific, and it's actually more easy to solve (in my opinion) in C++ because of the strong template mechanism. I'd like to give you one last reference to the F# [units of measure Library](https://docs.microsoft.com/en-us/dotnet/fsharp/language-reference/units-of-measure) which is pure awesomeness.

> TODO: write about ariana5 or other NASA events

To recap, I encourage you to use strong types whenever possible. This could save you a severe (need I remind you kittens being delivered to the wrong place?) production failure and a lot of headache during development. The example I gave for the Lat/Long implementation is simple and small enough that you can start using it today at small scale and migrate slowly with time.