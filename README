# proglang Designer Programming Language

/*Author: John Hossler*/

****************NOTE***************
Every file assumes that you run the compile program from the root
of this projects directory.
DO NOT cd into any of the subdirectories and try and run scripts in
there. Simply run compile from the root* directory and specify where the
file is RELATIVE to the root* directory.

* Any reference to the root directory means the directory where the
makefile is, i.e. the main directory of this project.

# Contents
## Syntax - here I explain the general syntax of my language
----STATEMENTS
----INITIALIZING VARIABLES
----FUNCTIONS
----OPERATORS
----OBJECTS
----ARRAYS
## Running a program - this describes how to compile and run my code
    on your own test scripts
## One bit adder - this is instructions on how to edit the one bit
    adder program to get different results

### Text

## Syntax

To make things simpler, I will generalize some things. When I say
statements, I mean that it is a series of individual statements. I will
go over statements soon. When you see "... statements ...", that refers
the series of statements that make up a statement list.

When I use the *optional* keyword, that means that what is after, up
until the matching */optional* keyword, is entirely optional and does
not need to be included to parse correctly.

The "\" preceding a line means that line is roughly my code, with
a few keywords to help generalize the structure.

Now, to get into the syntactic sugar.

----STATEMENTS

The entire program is just a series of statements.
A statement can be one of three things:

if statement:
\if(cond) {
\   ... statements ...
\} *optional* else *optional* if(cond) */optional* {
\   ... statements ...
\} */optional*

while statement:
\while(cond) {
\   ... statements ...
\}

OR

an expression followed by a semicolon, example:
\1 + 5;

----INITIALIZING VARIABLES

The syntax for initializing a variable is simple:

\var id *optional* = expression *\optional*

Variables always start with an alphanumeric number, but can have
digits inside of them.

A special variable, used mainly for variadic functions, is '@', which
can also begin a variable name.

----FUNCTIONS

In my language, all of my functions are anonymous functions. In order
to save a function to call later, you have to assign it to a variable,
but you do not have to assign it to a variable in order to call it!

Example:
\function () {
\   ... statements ...
\}

It is important to note that these are also expressions, so when writing them in a program, you must finish the statement with a semicolon!

Another important note, the last thing the function does is what the
function will return. Here is an example program of a function that
simply returns its parameter:

/(function(x) { x; })(3);

This simply returns 3!

Another way to call a function, as mentioned earlier, is to assign it
to a variable and then call that variable like a function.

Example:
\var f = function(x) { x; };
\f(3);

This also returns 3, but the syntax looks a bit cleaner because you
are using an identifier instead of creating and calling a function
on the fly.

----OPERATORS

Data isn't very useful if you don't do anything with it, so I included
several builtin operators that you can do things with!
Note* these operators do not have precedence builtin. If
you want your complicated expression to return something useful
and precise, you may want to encapsulate the sub-expressions with
parenthesis.

Here is a list of operators my language offers:

Mathematical operators

+: Simple binary addition
-: Can be unary to represent negative numbers, but it is also
    a binary subtraction operator
*: Multiplication
/: Division
**: Exponentiation
%: Modulo
+=: Increment
-=: Decrement
=: Assign

Conditional operators

<: Less than
>: Greater than
<=: Less than or equal to
>=: Greater than or equal to
==: Strict equality
!=: Strict inequality
===: Pointer equality
!==: Pointer inequality
&&: And
||: Or
^: Xor

----OBJECTS

In my language, objects are special in that they are really just
environments which contain a list of variables and their corresponding
value.

In order to access these variables inside the environment, I provide
the DOT operator. Here is an example of a function which returns
its environment and a way to access the variables inside.

\var f = function(x) { this; };
\var y = f(2);
\println(y.x);
\y.x = 3;
\println(y.x);

In the first print statement, y.x is 2, but after changing the variables
value with y.x = 3, the second print statement displays it as 3. This
can get much more complicated with storing functions in objects that
operate upon the data in their environment, allowing you to create
complex structures like trees!

----ARRAYS

I don't like arrays all that much, but they do have good use, so I added
them anyway (also because I was required to have them).

You can create arrays on the fly, or you can do something a bit more
useful and use my builtin "array()" function, which takes one argument,
an integer, and creates an array of that integer size.

To create on the fly and access specific indexes in the array:

\var y = {1,2,3};
\println(y[2]);
\y[2] = 4;
\println(y[2]);

As you can see, you can both access and change points in the arrays
memory with the syntax id[int].

----COMMENTS

All I currently support is single line comments, though multi-line
comments will probably be added soon, as they aren't difficult.

A comment currently is a single line comment. "//" marks the beginning
of a comment on a line, and the rest of that line will be ignored.

\var x = 10;
\// This is a comment!
\println(x);

// This is a comment! will be completely ignored by the lexer.

## Running a program

If you create a script in my language, or want to run one of the ones I
made in testScripts/, simply run the command:

make

After this, you will be able to type `compile <filename>` to run a
program. For every test script that includes another test script, I
included a direct path from the root of this directory, so do not run
compile on a program from within the testScripts/ directory, as this
will cause issues.

Example of testing the dictionary:

compile testScripts/testDictionary.sux

My tests in my makefile are exactly as specified in the dpl.html:

make cat-error1
make run-error1
make cat-error2
make run-error2
make cat-error3
make run-error3
make cat-arrays
make run-arrays
make cat-conditionals
make run-conditionals
make cat-recursion
make run-recursion
make cat-iteration
make run-iteration
make cat-functions
make run-functions
make cat-dictionary
make run-dictionary

## One bit adder

In order to change the values, simply open up the file
"testScripts/testOneBitAdder.sux". Under this, I have a comment
specifying where you can change the values of the two input wires.
The output from this program specifies the state of the output wire and
the carry out wire.

To run the one bit adder, simply run (from the root directory of this
project):

compile testScripts/testOneBitAdder.sux

or, you can use the makefile command:

make cat-problem
make run-problem

This will display the program that tests the One Bit adder. To see
the code for the actual adder, and how I implemented it, see the file
"testScripts/fullAdder.sux".

This will report the final state of the output wire and the carry out
wire.
