//xrepcim00
//15.11.2023

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_LENGTH 101

//struct containing matrix
typedef struct {
    int rows;
    int cols;
    unsigned char *cells;
} Map;

enum Triangle {
    LEFT,
    RIGHT,
    MIDDLE
};

//function that converts ascii value of digits
void charToDigit(int *value)
{
    *value = *value - '0';
}

//function that reads data from the file
void scanFile(int *rows, int *cols)
{
    char line[MAX_LENGTH];
    int num1;
    int num2;

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
            fprintf(stderr, "Invalid");
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "Invalid");
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
void appendToMap(Map *map)
{
    int value;
    int index = 0;

    while((value = fgetc(stdin)) != EOF)
    {
        if (value != ' ' && value != '\t' && value != '\n')
        {
            charToDigit(&value);
            map->cells[index] = (unsigned char)value;
            index ++;
        }
    }

    //when matrix dimensions do not match given parameters, exit with an error
    if (index != map->cols*map->rows)
    {
        fprintf(stderr, "Invalid");
        freeMap(map);
        exit(1);
    }
} 

//function that returns true if there is an obstacle at border of given position
bool isBorder(Map *map, int row, int col, int border)
{
    int cell = map->cells[row*map->cols + col];
    //get a digit from given position(border) from bit representation of cell
    int bit = (cell >> border) & 1;
    //return 1 or 0 (True or False)
    return bit;
}

//function that checks if the content of the file is valid
int testMap(int rows, int cols, Map *map) 
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            //if border is on the right side of the triangle, the next triangle should have border on its left side
            if (isBorder(map, i, j, RIGHT) != isBorder(map, i, j+1, LEFT) && j+1 < cols)
            {
                fprintf(stderr, "Invalid");
                return 1;
            }
            //when i+j is divisible by 2 the triangle is upside down
            else if ((i+j)%2 == 0)
            {
                //if border is at the top of the triangle, the triangle above should have border on the bottom
                if (isBorder(map, i,j, MIDDLE) != isBorder(map, i-1, j, MIDDLE) && i-1 >= 0)
                {
                    fprintf(stderr, "Invalid");
                    return 1;
                }
            }
            else
            {
                //if border is at the bottom of the triangle, the triangle bellow should have border on the top
                if (isBorder(map, i,j, MIDDLE) != isBorder(map, i+1, j, MIDDLE) && i+1 < rows)
                {
                    fprintf(stderr, "Invalid");
                    return 1;
                }
            }
        }
    }
    printf("Valid\n");
    return 0;
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
        //create a map and fill it with data drom a file
        Map *map = createMap(rows, cols);
        appendToMap(map);

        //run test to check if data in the matrix are correct
        testMap(rows, cols, map);

        //free allocated memory
        freeMap(map);
    }
    else if (strcmp(argv[1], rpath) == 0)
    {
        //read from file to get the necessary data
        scanFile(&rows, &cols);
        //create a map and fill it with data drom a file
        Map *map = createMap(rows, cols);
        appendToMap(map);

        printf("TODD2");

        //free allocated memory
        freeMap(map);
    }
    else if (strcmp(argv[1], lpath) == 0)
    {
        printf("TODD3");
    }
    else if (strcmp(argv[1], shortest) == 0)
    {
        printf("TODD4");
    }

    return 0;
}