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

//function that converts ascii value of digits
void toNum(int *value)
{
    *value = *value - 48;
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
        //scan first two numbers from first line
        if (sscanf(line," %d %d", &num1, &num2) == 2)
        {
            *rows = num1;
            *cols = num2;
        }
        //exit code when scan wasn't successful
        else
        {
            fprintf(stderr, "Failed to read data from the first line");
            exit(1);
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

//function used while writing code(delete later)
void printMap(Map *map)
{
    for (int i = 0; i < map->rows; i++)
    {
        for (int j = 0; j < map->cols; j++)
        {
            printf("%d ", map->cells[i * map->cols + j]);
        }
        printf("\n");
    }
}

//function that appends data to matrix from stdin
void appendToMap(int rows, int cols, Map *map)
{
    int value;
    int index = 0;

    while((value = fgetc(stdin)) != EOF)
    {
        if (value != ' ' && value != '\t' && value != '\n')
        {
            toNum(&value);
            map->cells[index] = (unsigned char)value;
            index ++;
        }
    }
} 

bool isBorder(int row, int col, Map *map, int border)
{
    printf("TODO");
}
//function that checks if the content of the file is valid
void testMap(int rows, int cols, Map *map) 
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            isBorder(i, j, map, map->cells[i * map->cols + j]);
        }
    }
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

    //if there are no arguments in command line exit with an error
    if (agrc < 2)
    {
        fprintf(stderr, "No arguments given");
        return 1;
    }
    
    //display instructions when '--help' is the user input
    if (strcmp(argv[1], help) == 0) 
    {
        printf("\n");
        printf("Input a file containing matrix of numbers\n");
        printf("Type '--test <file' to check if data in the matrix are valid\n");
        printf("Type '--rpath R C <file' to solve labyrinth using the right hand rule\n");
        printf("Type '--lpath R C <file' to solve labyrinth using the left hand rule\n");
        printf("Type '--shortest R C <file' to find the shortest path in the labyrinth\n");
        printf("\033[3mR C = row and collumn you want to start on\033[0m\n");
        printf("\n");
    }
    else if (strcmp(argv[1], test) == 0) 
    {
        //read from file to get the necessary data
        scanFile(&rows, &cols);
        printf("Rows: %d Cols: %d\n", rows, cols); //delete later

        //create a map and fill it with data drom a file
        Map *map = createMap(rows, cols);
        appendToMap(rows, cols, map);
        printMap(map);
        
        //run test to check if data in the matrix are correct
        testMap(rows, cols, map);

        //free allocated memory
        freeMap(map);
    }

    return 0;
}