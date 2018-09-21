"# cs373_prog1" 

Write a program to simulate the execution of a restricted Turing machine. Your program can be written
in Java, C, or C++, and needs to be able to be compiled and executed on the computers in EB-G7 (or a
Linux or Mac computer I have access to). If you do not know Java, C, or C++, you may submit a version
in Python (with my permission). 

Your program will read the definition of the machine from a file (the first command line argument), with
the second command line argument being the string to simulate the machine running on (the input to the
automata), and the third command line argument being the maximum number of transitions to simulate.
The output of your program will be written to standard output. The output will consist of the list of states
the automata transitions through while reading the input string (separated by “->”) followed by either
“accept” or “reject”, depending on wether the automata accepts or rejects the input string.

The states will be numbered between 0 and 1,000 (not necessarily contiguous or in any particular
order). There are three special types of states – the start state (exactly one), the accept state(s) (0 or
more), and the reject state(s) (0 or more). The format of a “state definition” line is:

“state x y” where x is a number in [0, 1000], and y is either null, start, accept, or reject. No state
can be more than one of these (can't be start and accept, start and reject, or accept and reject).

Some examples are:
state 7 start
state 12 accept
state 952 reject

There is no guarantee on the order of the state lines, other than they are at the beginning of the file and
there is one line per state. The input file will be tab delimited (should be easily parsed with Java, C, or
C++). The only state lines that are required are those for the start, accept, and reject states.
The remainder of the file defines the transitions. For this machine, the transition format is “q,a->r,b,x”
where q is the current state that the machine is in, the a is the symbol that the machine reads, the r is
the state that the machine transitions to, the b is the value that the machine writes on top of the a, and
the x is either an L or an R, which tells you to either move back one symbol (L) or to the next symbol (R).
The format of the transitions in the file will be:

transition q a r b x

Since q and r are states, they will be numbers in the range of [0, 1000]. You can assume that a and b
will be digits {0, 1, …, 9}, lower case letters {a, b, …, z} or special characters {$, #, _, %}. And finally x
will be in {L, R}. The “_” for a or b is used to represent the blank space character. This is due to
problems associated with parsing a blank space from an input line, when white space characters are
used to delimit the other values. In my program, when I read a “_” for a or b, I simply replace the “_” with
a blank space. You should assume that to the right or left of the input there are blank space characters. 

The input will be a string, consisting of digits, lower case letters, and special characters. Initially the
machine will be looking at the left most symbol of the input string. The transitions will tell you what
symbol to process next. You should assume that there are blank spaces to the left and right of the input.
You will need to be able to handle cases where you move to the left or right of the input symbols.

If the machine ever transitions to an accept state, then it is to immediately stop and output the sequence
of states that is transitioned through (with “->” between them), followed by a blank space and “accept”.
If the machine ever transitions to a reject state, then it is to immediately stop and output the sequence of
states that is transitioned through (with “->” between them), followed by a blank space and “reject”.
If the machine hasn't entered the accept state or reject state after the number of transitions specified by
the third command line argument, then it is to stop and output the sequence of states that it transitioned
through (with “->” between them), followed by a blank space and “quit”. The largest value for the
maximum number of transitions that I will use is 10,000.

For java, standard input is System.in, standard output is System.out, and standard error is System.err.
For C, standard input is stdin, standard output is stdout, and standard error is stderr.
For C++, standard input is cin, standard output is cout, and standard error is cerr.

E-mail your program (source file(s) and makefile if required attached to the e-mail, I don't want a tar, zip,
rar, jar, or any other package file) to me (david.garrison@binghamton.edu) by 11:59:59pm on the date
due. Your main filename is to be your last name (lower case) followed by “_p1” (for example, my
filename would be “garrison_p1.java”) and the subject of your e-mail is “CS 373 program 1”. If you write
C or C++, your executable file is to be named your last name (lower case) followed by “_p1” (for
example, my executable filename would be “garrison_p1” if I wrote a C or C++ program).

The grading will be based on the percentage of correct results your program gets for my collection of
test automata and test strings and following the directions.

In particular, 20% of the grade will be for following the instructions (filename being your last name in
lower case, subject, attaching files to e-mail, correct executable name) and 80% will be correct results
(check my out put format). If you are using java, I should be able to execute “javac “your last name in
lower case_p1”.java” to compile your program. For C or C++ your should include a makefile so that I can
simply execute “make “your last name in lower case_p1”” to compile and ./”your last name in lower
case_p1” to execute.

