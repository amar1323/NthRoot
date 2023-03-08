Problem statement
=================
Write a method that returns the nth root of a given positive number.
Method signature
public static double GetNthRoot(double number, int n)
Assume following
·         number >= +1.00
·         root   >= +1
Rules:
·         You can only use 4 basic math operations + - * /
·         You cannot use any Math.* methods

Solution in C++
===============
Solution to the previous problem. The solution is compared to std::pow in term of precision and time.

To compile
==========
./compile.sh