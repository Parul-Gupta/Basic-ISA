# Basic-ISA
This repository contains implementations of Signed and Unsigned Addition, Subtraction of 32-bit integers as well as code for converting a floating point number (written in IEEE 754) format to a 32-bit integer and vice-versa.
Input file(given as a command-line argument) contains the assembly instructions to for the above mentioned operations.
Output file contains the corresponding results and the state of the flag register, which includes the carry bit, the overflow bit, the zero bit and the negative bit. 

compile the code as:
	gcc prog1.c 
run the code as:
	./a.out input
Here, the 'input' file contains the test cases.

This is a code to add and subtract signed and unsigned integers and the value of the flag tells the various conditions.
the code also converts floatinf point numbers into integers and vice-versa.

Input is read from a file given by the user as a command line argument, and outputs are appended in a file named 'output' which is created in the program itself.

There are 6 commands based on 2 basic functions: add_uint and negate.

add_uint takes 2 unsigned integers as arguments and simply adds them, and then gives the result as an unsigned integer.
It also updates the flag according to whether the result(when read as signed int) is -ve,zero or out of range(overflow and carry). 
negate gives the 2's complement of the input unsigned integer and returns it as unsigned integer.

ADD command adds 2 ints and gives result using add_uint function.It treats the parameters and the result as unsigned int. It gives error on updation of carry flag.

SUB command performs ADD on first number and negate(2nd number).It treats the result as signed int and reports error for overflow.

ADDS command also adds 2 ints and gives result using add_uint function but it treats the parameters and result as signed ints.It gives error on overflow.

SUBS command performs ADDS on 1st no. and negate(2nd no.).It reports error in case of overflow.

ITOF converts integer into float(sign,exponent and mantissa).
FTOI converts float to integer and gives error in case of overflow.  

