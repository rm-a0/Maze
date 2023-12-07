# Overview
This program is designed to solve triangular mazes using right hand rule, left hand rule and BFS algorithm.__
Solve = find path out of the maze

# Maze  
The maze is saved in txt file where first line contains number of rows and columns and maze represented by matrix of numbers.
Each number represents one cell in a maze. When converted to binary and read from the back it gives us information about borders.

For example number 5 (bin 101) tells us that it has borders on the left and middle side of the triangle and the right side is free.

1x2^0 + 0x2^1 + 1x2^2 = 1 + 0 + 4 = 5
left    right   middle

0 = no border
1 = border

# How to use
Compile the program using:

_gcc -std=c11 -Wall -Wextra -Werror maze.c -o maze_

Run the program using:

_$ ./maze --help_ (display instructions)__
_$ ./maze --test file.txt_ (test if map in the file is valid)__
_$ ./maze --rpath R C file.txt_ (solve the maze using right hand rule)__
_$ ./maze --lpath R C file.txt_ (solve the maze using left hand rule)__
_$ ./maze --shortest R C file.txt_ (solve the maze using BFS algorithm)__
