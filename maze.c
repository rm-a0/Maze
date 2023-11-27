//xrepcim00
//23.11.2023

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

void displayInstructions()
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

//function that converts ascii value of digits
void charToDigit(int *value)
{
    *value = *value - '0';
}

//function that reads data from the file
int scanFile(int *rows, int *cols, FILE *file)
{
    char line[MAX_LENGTH];
    int num1;
    int num2;

    if (fgets(line, MAX_LENGTH, file) != NULL)
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
            fprintf(stderr, "Data in file are formated incorrectly\n");
            return 1;
        }
    }
    else
    {
        fprintf(stderr, "Error reading from a file\n");
        return 1;
    }
    return 0;
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
int appendToMap(Map *map, FILE *file)
{
    int value;
    int index = 0;

    while((value = fgetc(file)) != EOF)
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
        return 1;
    }
    return 0;
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

//function that returns value of border that is prefered on the first position
int startBorder(Map *map, int r, int c, int leftright)
{
    //starting borders for right hand rule
    if (leftright == RIGHT)
    {
        if (c == 1 && r%2 == 1)
        {
            return RIGHT;
        }
        else if (c == 1 && r%2 == 0)
        {
            return MIDDLE;
        }
        else if (c == map->cols && (r+c)%2 == 0)
        {
            return MIDDLE;
        }
        else if (c == map->cols && (r+c)%2 == 1)
        {
            return LEFT;
        }
        else if (r == 1)
        {
            return LEFT;
        }
        else if (r == map->rows)
        {
            return RIGHT;
        }
    }
    //starting borders for left hand rule
    else 
    {
        if (c == 1 && r%2 == 1)
        {
            return MIDDLE;
        }
        else if (c == 1 && r%2 == 0)
        {
            return RIGHT;
        }
        else if (c == map->cols && (r+c)%2 == 0)
        {
            return LEFT;
        }
        else if (c == map->cols && (r+c)%2 == 1)
        {
            return MIDDLE;
        }
        else if (r == 1)
        {
            return RIGHT;
        }
        else if (r == map->rows)
        {
            return LEFT;
        }
    }
    return -1;
}

//function that moves in the matrix based on prefered border
void step(int prefBorder, int *r, int *c)
{
    if (prefBorder == LEFT)
    {
        (*c)--;
    }
    else if (prefBorder == RIGHT)
    {
        (*c)++;
    }
    else if (prefBorder == MIDDLE && (*r + *c)%2 == 0)
    {
        (*r)--;
    }
    else
    {
        (*r)++;
    }
}

//function that changes prefered border based on current position and previous border
void changeBorder(int *prefBorder, int r, int c, int leftright)
{
    if (leftright == RIGHT)
    {
        if (*prefBorder == MIDDLE && (r + c)%2 == 0)
        {
            *prefBorder = LEFT;
        }
        else if (*prefBorder == LEFT && (r + c)%2 == 0)
        {
            *prefBorder = MIDDLE;
        }
        else if (*prefBorder == RIGHT && (r + c)%2 == 0)
        {
            *prefBorder = RIGHT;
        }
        else if (*prefBorder == MIDDLE && (r + c)%2 == 1)
        {
            *prefBorder = RIGHT;
        }
        else if (*prefBorder == LEFT && (r + c)%2 == 1)
        {
            *prefBorder = LEFT;
        }
        else if (*prefBorder == RIGHT && (r + c)%2 == 1)
        {
            *prefBorder = MIDDLE;
        }
    }
    else
    {
        if (*prefBorder == MIDDLE && (r + c)%2 == 0)
        {
            *prefBorder = RIGHT;
        }
        else if (*prefBorder == LEFT && (r + c)%2 == 0)
        {
            *prefBorder = LEFT;
        }
        else if (*prefBorder == RIGHT && (r + c)%2 == 0)
        {
            *prefBorder = MIDDLE; //idk
        }
        else if (*prefBorder == MIDDLE && (r + c)%2 == 1)
        {
            *prefBorder = LEFT;
        }
        else if (*prefBorder == LEFT && (r + c)%2 == 1)
        {
            *prefBorder = MIDDLE;
        }
        else if (*prefBorder == RIGHT && (r + c)%2 == 1)
        {
            *prefBorder = RIGHT;
        }
    }
}

//function that changes prefered border anti-clockwise
void turnLeft(int *prefBorder, int r, int c)
{
    if ((r+c)%2 == 1)
    {
        (*prefBorder)--;
    }
    else
    {
        (*prefBorder)++;
    }

    if (*prefBorder < 0)
    {
        *prefBorder = *prefBorder + 3;
    }
    else if(*prefBorder > 2)
    {
        *prefBorder = *prefBorder - 3;
    }
}

//function that changes prefered border clockwise
void turnRight(int *prefBorder, int r, int c)
{
    if ((r+c)%2 == 1)
    {
        (*prefBorder)++;
    }
    else
    {
        (*prefBorder)--;
    }

    if (*prefBorder < 0)
    {
        *prefBorder = *prefBorder + 3;
    }
    else if(*prefBorder > 2)
    {
        *prefBorder = *prefBorder - 3;
    }
}

//function that solves maze using right hand rule
int rPath(Map *map, int r, int c)
{
    //checks if the user input is correct
    if (r > map->rows || r < 1 || c > map->cols || c < 1)
    {
        fprintf(stderr, "Values of R and C are not within the matrix");
        return 1;
    }

    int prefBorder;
    prefBorder = startBorder(map, r, c, RIGHT);

    //print first position
    printf("%d,%d\n", r, c);

    //loop that functions only when r and c are within boundaries of a map
    while(r <= map->rows && c <= map->cols && r > 0 && c > 0) 
    {
        if (isBorder(map, r-1, c-1, prefBorder) == 0)
        {
            step(prefBorder, &r, &c);
            changeBorder(&prefBorder, r, c, RIGHT);

            //print every position after making step except the last one, which is out of boundaries of a map
            if (r <= map->rows && c <= map->cols && r > 0 && c > 0)
            {
                printf("%d,%d\n", r, c);
            }
        }
        else
        {
            turnLeft(&prefBorder, r, c);
        }
    }
    return 0;
}

//function that solves maze using left hand rule
int lPath(Map *map, int r, int c)
{
    //checks if the user input is correct
    if (r > map->rows || r < 1 || c > map->cols || c < 1)
    {
        fprintf(stderr, "Values of R and C are not within the matrix");
        return 1;
    }

    int prefBorder;
    prefBorder = startBorder(map, r, c, LEFT);

    //print first position
    printf("%d,%d\n", r, c);

    //loop that functions only when r and c are within boundaries of a map
    while(r <= map->rows && c <= map->cols && r > 0 && c > 0) 
    {
        if (isBorder(map, r-1, c-1, prefBorder) == 0)
        {
            step(prefBorder, &r, &c);
            changeBorder(&prefBorder, r, c, LEFT);

            //print every position after making step except the last one, which is out of boundaries of a map
            if (r <= map->rows && c <= map->cols && r > 0 && c > 0)
            {
                printf("%d,%d\n", r, c);
            }
        }
        else
        {
            turnRight(&prefBorder, r, c);
        }
    }
    return 0;
}

void processFile(const char *filename, int *rows, int *cols, Map **map) 
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) 
    {
        fprintf(stderr, "Error opening file\n");
        exit(1);
    }

    //Read from file to get the necessary data
    if (scanFile(rows, cols, file) != 0) 
    {
        fclose(file);
        exit(1);
    }

    //Create a map and fill it with data from the file
    *map = createMap(*rows, *cols);
    
    if (*map == NULL) 
    {
        //Handle the case where map creation fails
        fprintf(stderr, "Error creating map\n");
        fclose(file);
        exit(1);
    }

    // Append data to the map
    if (appendToMap(*map, file) != 0) 
    {
        fclose(file);
        freeMap(*map);
        exit(1);
    }

    fclose(file);
}

int main(int argc, char *argv[])
{
    const char *help = "--help";
    const char *test = "--test";
    const char *rpath = "--rpath";
    const char *lpath = "--lpath";

    int rows = 0;
    int cols = 0;
    Map *map = NULL;

    //if there are no arguments in command line exit with an error
    if (argc < 2)
    {
        fprintf(stderr, "No arguments given");
        return 1;
    }
    
    //display instructions when '--help' is the user input
    if (strcmp(argv[1], help) == 0) 
    {
        displayInstructions();
    }
    else if (strcmp(argv[1], test) == 0) 
    {
        //process file and create struct Map from given data
        processFile(argv[argc-1], &rows, &cols, &map);

        //run test to check if data in the matrix are correct
        testMap(rows, cols, map);

        //free allocated memory
        freeMap(map);
    }
    else if (strcmp(argv[1], rpath) == 0)
    {
        //process file and create struct Map from given data
        processFile(argv[argc-1], &rows, &cols, &map);

        //solve map using right hand rule
        rPath(map, atoi(argv[2]), atoi(argv[3]));

        //free allocated memory
        freeMap(map);
    }
    else if (strcmp(argv[1], lpath) == 0)
    {
        //process file and create struct Map from given data
        processFile(argv[argc-1], &rows, &cols, &map);

        //solve map using left hand rule
        lPath(map, atoi(argv[2]), atoi(argv[3]));

        //free allocated memory
        freeMap(map);
    }

    return 0;
}