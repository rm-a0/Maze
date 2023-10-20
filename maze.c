//xrepcim00
//17.10.2023

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#define MAX_LENGTH 101

//struct containing matrix
typedef struct {
    int rows;
    int cols;
    unsigned char *cells;
} Map;

//function that checks if the string contains numbers
bool isNum(char str[])
{
    for (int i = 0; str[i] != '\0'; i++)
        {
            //return false if input is not a number or a space
            if (!isdigit(str[i]) && !isspace(str[i]))
            {
                return false;
            }
        }
    return true;
}

//function that reads data from the file
void scanFile(int *rows, int *cols)
{
    char line[MAX_LENGTH];
    int num1;
    int num2;
    
    //if there is no file specified, exit function and display error
    if (isatty(STDIN_FILENO))
    {
	    fprintf(stderr, "No file specified\n");
	    exit(1);
    }

    if (fgets(line, MAX_LENGTH, stdin) != NULL)
    {
        //if there are characters other than numbers in the file , exit function and display error
        if(!isNum(line))
        {
            fprintf(stderr, "The file contains characters that are not allowed\n");
            exit(1);
        }
        else
        {
            //scan first two numbers from first line
            if (sscanf(line," %d %d", &num1, &num2))
            {
                *rows = num1;
                *cols = num2;
            }
            //exit code when scan wasn't successful
            else
            {
                fprintf(stderr, "Failed to read numbers from a line");
                exit(1);
            }
        }
    }
    else
    {
        fprintf(stderr, "Failed to read data from the file\n");
        exit(1);
    }
}

//funnction that allocates memory for 2d matrix and returns pointer to a Map struct
Map* createMap(int rows, int cols) 
{
    //allocates memory for Map struct
    Map *map = (Map *)malloc(sizeof(Map));

    if (map == NULL) 
    {
        fprintf(stderr, "Memorry allocation for the sturct Map failed\n");
        return NULL;
    }

    map->rows = rows;
    map->cols = cols;

    //allocates memory for 2d matrix cells
    map->cells = (unsigned char *)malloc(rows * cols * sizeof(unsigned char));
    if (map->cells == NULL) 
    {
        fprintf(stderr, "Memory allocation for the matrix failed\n");

        //if second memory allocation fails, free previously allocated memory
        free(map);
        return NULL;
    }
    //return pointer to a struct called Map
    return map;
}

//function that gets rid of all allocated memory
void freeMap(Map *map)
{
    free(map->cells);
    free(map);
}

//function that appends data to matrix from stdin
void appendToMap(Map *map) 
{
    printf("TODO 1\n");
}

//function that checks if the content of the file is valid
void testMap(Map *map) 
{
    printf("TODO 2\n");
}

int main(int agrc, char *argv[])
{
    const char *help = "--help";
    const char *test = "--test";
    const char *rpath = "--rpath";
    const char *lpath = "--lpath";
    const char *shortest = "--shortest";

    int rows;
    int cols; 

    //if there are no arguments in comand line exit with an error
    if (agrc < 2)
    {
        fprintf(stderr, "No arguments given");
        return 1;
    }
    
    //display instructions when '--help' is the user input
    if (strcmp(argv[1], help) == 0) 
    {
        printf("Input a file containing matrix of numbers\n");
        printf("Type '--test < file' to check if data in the matrix are valid\n");
        printf("Type '--rpath R C < file' to solve labyrinth using the right hand rule\n");
        printf("Type '--lpath R C < file' to solve labyrinth using the left hand rule\n");
        printf("Type '--shortest R C < file' to find the shortest path in the labyrinth\n");
        printf("*R C = row and collumn you want to start on*\n");
    }
    else if (strcmp(argv[1], test) == 0) 
    {
        //read from file to get the necessary data
        scanFile(&rows, &cols);
        printf("Rows: %d Cols: %d\n", rows, cols);

        //create a map and fill it with data drom a file
        Map *map = createMap(rows, cols);
        appendToMap(map);

        //run test to check if data in the matrix are correct
        testMap(map);

        //free allocated memory
        freeMap(map);
    }

    return 0;
}