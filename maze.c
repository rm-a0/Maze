#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_LENGTH 100

//struct containing matrix
typedef struct {
    int rows;
    int cols;
    unsigned char *cells;
} Map;

//function that checks if the content of the file is valid
int test (void) {
    printf("TODO \n");
}

int main(int agrc, char *argv[])
{
    const char *str1 = "--help";
    const char *str2 = "--test";
    const char *str3 = "--rpath";
    const char *str4 = "--lpath";
    const char *str5 = "--shortest";   

    //display instructions when '--help' is the user input
    if (strcmp(argv[1], str1) == 0) {
        printf("Input a file containing matrix of numbers\n");
        printf("Type '--test < file' to check if content of the file is correct\n");
        printf("Type '--rpath R C < file' to solve labyrinth using the right hand rule\n");
        printf("Type '--lpath R C < file' to solve labyrinth using the left hand rule\n");
        printf("Type '--shortest R C < file' to find the shortest path in the labyrinth\n");
        printf("*R C = row and collumn you want to start on*\n");
    }
    else if (strcmp(argv[1], str2) == 0) {
        test();
    }

    return 0;
}