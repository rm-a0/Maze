# Overview
This program is designed to solve triangular mazes using right hand rule, left hand rule and BFS algorithm.\
Solve = find path out of the maze

# Maze  
The maze is saved in txt file where first line contains number of rows and columns and maze represented by matrix of numbers.
Each number represents one cell in a maze. When converted to binary and read from the back it gives us information about borders.

For example number 5 (bin 101) tells us that it has borders on the left and middle side of the triangle and the right side is free.
<pre>
1x2^0 + 0x2^1 + 1x2^2 = 1 + 0 + 4 = 5
left    right   middle
</pre>
0 = no border\
1 = border

# How to use
<pre>
Compile the program using:
<\pre>
gcc -std=c11 -Wall -Wextra -Werror maze.c -o maze

Run the program using:
<pre>
$ ./maze --help                      (display instructions)
$ ./maze --test file.txt             (test if map in the file is valid)
$ ./maze --rpath R C file.txt        (solve the maze using right hand rule)
$ ./maze --lpath R C file.txt        (solve the maze using left hand rule)
$ ./maze --shortest R C file.txt     (solve the maze using BFS algorithm)
</pre>
