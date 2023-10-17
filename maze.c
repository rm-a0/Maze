//xrepcim00
//17.10.2023

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

//funnction that allocates memory for 2d matrix and returns pointer to a Map struct
Map* createMap(int r, int c) 
{
    //allocates memory for Map struct
    Map *map = (Map *)malloc(sizeof(Map));

    if (map == NULL) 
    {
        fprintf(stderr, "Memorry allocation for the sturct Map failed\n");
        return NULL;
    }

    map->rows = r;
    map->cols = c;

    //allocates memory for 2d matrix cells
    map->cells = (unsigned char *)malloc(r * c * sizeof(unsigned char));
    if (map->cells == NULL) 
    {
        fprintf(stderr, "Memory allocation for the matrix failed\n");

        //if second memory allocation fails, free previously allocated memory
        free(map);
        return NULL;
    }

    return map;
}

//function that appends data to matrix from stdin
void appendToMap(Map *map) 
{
    printf("TODO 1\n");
}

//function that checks if the content of the file is valid
int test (void) 
{
    printf("TODO 2\n");
}

int main(int agrc, char *argv[])
{
    const char *str1 = "--help";
    const char *str2 = "--test";
    const char *str3 = "--rpath";
    const char *str4 = "--lpath";
    const char *str5 = "--shortest";

    int rows;
    int cols; 

    //display instructions when '--help' is the user input
    if (strcmp(argv[1], str1) == 0) {
        printf("Input a file containing matrix of numbers\n");
        printf("Type '--test < file' to check if data in the matrix are valid\n");
        printf("Type '--rpath R C < file' to solve labyrinth using the right hand rule\n");
        printf("Type '--lpath R C < file' to solve labyrinth using the left hand rule\n");
        printf("Type '--shortest R C < file' to find the shortest path in the labyrinth\n");
        printf("*R C = row and collumn you want to start on*\n");
    }
    else if (strcmp(argv[1], str2) == 0) {
        //create a map and fill it with data drom a file
        Map *map = createMap(rows, cols);
        appendToMap(map);

        //run test to check if data in the matrix are correct
        test();

        //free allocated memory
        freeMap(map);
    }

    return 0;
}