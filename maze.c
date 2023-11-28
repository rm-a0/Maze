//xrepcim00
//28.11.2023

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

//struct containing coordinates of a position in a matrix 
typedef struct {
    int row;
    int col;
} Position;

//struct containing array of Positions
//front and back are used for indexing array
typedef struct {
    Position* array;
    int front;
    int back;
    int capacity;
} Queue;

enum Triangle {
    LEFT,
    RIGHT,
    MIDDLE
};

//function that displays instructions
void displayInstructions()
{
    printf("\n");
    printf("Input a file containing matrix of numbers\n");
    printf("Type '--test file' to check if data in the matrix are valid\n");
    printf("Type '--rpath R C file' to solve labyrinth using the right hand rule\n");
    printf("Type '--lpath R C file' to solve labyrinth using the left hand rule\n");
    printf("Type '--shortest R C file' to find the shortest path in the labyrinth\n");
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

//function that allocates memory for 2d array of integers
int** createVisited(int rows, int cols)
{
    int** visited = (int**)malloc(rows * sizeof(int*));

    if (visited == NULL)
    {
        fprintf(stderr,"Memory allocation failed\n");
        return NULL;
    }

    for (int i = 0; i < rows; i++)
    {
        visited[i] = (int*)malloc(cols * sizeof(int));

        if (visited[i] == NULL)
        {
            fprintf(stderr,"Memory allocation failed\n");
            for(int j = 0; j < i; j++)
            {
                free(visited[j]);
            }
            free(visited);
            return NULL;
        }
    }
    return visited;
}

//function that gets rid of memory allocated for 2d array
void freeVisited(int** visited, int rows)
{
    for(int i = 0; i < rows; i++)
    {
        free(visited[i]);
    }
    free(visited);
}

//function that allocates memory for 2d array of Positions
Position** createParent(int rows, int cols)
{
    Position** array = (Position**)malloc(rows * sizeof(Position*));

    if (array == NULL)
    {
        fprintf(stderr,"Memory allocation failed\n");
        return NULL;
    }

    for (int i = 0; i < rows; i++)
    {
        array[i] = (Position*)malloc(cols * sizeof(Position));
        if (array[i] == NULL)
        {
            fprintf(stderr,"Memory allocation failed\n");
            for (int j = 0; j < i; j++)
            {
                free(array[j]);
            }
            free(array);
            return NULL;
        }
    }
    return array;
}

//function that gets rid of memory allocated fot 2d array of Positions
void freeParent(Position** array, int rows)
{
    for (int i = 0; i < rows; i++)
    {
        free(array[i]);
    }
    free(array);
}

//function that allocates memory for Queue struct
Queue* createQueue(int capacity)
{
    Queue* queue = (Queue*)malloc(sizeof(Queue));

    if (queue == NULL)
    {
        fprintf(stderr,"Memory allocation failed\n");
        return NULL;
    }

    queue->capacity = capacity;
    queue->front = -1;
    queue->back = -1;

    //allocate memory for array of Positions
    queue->array = (Position*)malloc(capacity * sizeof(Position));

    if (queue->array == NULL)
    {
        fprintf(stderr,"Memory allocation failed\n");
        free(queue);
        return NULL;
    }
    return queue;
}

//fucntion that gets rid of memory allocated for Queue struct
void freeQueue(Queue* queue)
{
    free(queue->array);
    free(queue);
}

//function that allocates memory for Map struct
Map* createMap(int rows, int cols) 
{
    Map *map = (Map*)malloc(sizeof(Map));

    if (map == NULL) 
    {
        fprintf(stderr, "Memorry allocation for the sturct Map failed\n");
        return NULL;
    }

    map->rows = rows;
    map->cols = cols;

    //allocates memory for cells
    map->cells = (unsigned char *)malloc(rows * cols * sizeof(unsigned char));
    if (map->cells == NULL) 
    {
        fprintf(stderr, "Memory allocation for the matrix failed\n");
        free(map);
        return NULL;
    }
    return map;
}

//function that gets rid of memory allocated for Map struct
void freeMap(Map *map)
{
    free(map->cells);
    free(map);
}

//function that appends data to matrix from file
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

//function for processing data from a file and appending them to a Map struct
int processFile(const char *filename, int *rows, int *cols, Map **map) 
{
    //open file
    FILE *file = fopen(filename, "r");
    if (file == NULL) 
    {
        fprintf(stderr, "Error opening file\n");
        return -1;
    }

    //Read from file to get the necessary data
    if (scanFile(rows, cols, file) != 0) 
    {
        fclose(file);
        return -1;
    }

    //Create a Map struct
    *map = createMap(*rows, *cols);
    
    if (*map == NULL) 
    {
        fprintf(stderr, "Error creating map\n");
        fclose(file);
        freeMap(*map);
        fclose(file);
        return -1;
    }

    // Append data to the map
    if (appendToMap(*map, file) != 0) 
    {
        fclose(file);
        freeMap(*map);
        return -1;
    }

    fclose(file);
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
            if (j+1 < cols && isBorder(map, i, j, RIGHT) != isBorder(map, i, j+1, LEFT))
            {
                fprintf(stderr, "Invalid\n");
                return 1;
            }
            //when i+j is divisible by 2 the triangle is upside down
            else if ((i+j)%2 == 0)
            {
                //if border is at the top of the triangle, the triangle above should have border on the bottom
                if (i-1 >= 0 && isBorder(map, i,j, MIDDLE) != isBorder(map, i-1, j, MIDDLE))
                {
                    fprintf(stderr, "Invalid\n");
                    return 1;
                }
            }
            else
            {
                //if border is at the bottom of the triangle, the triangle bellow should have border on the top
                if (i+1 < rows && isBorder(map, i,j, MIDDLE) != isBorder(map, i+1, j, MIDDLE))
                {
                    fprintf(stderr, "Invalid\n");
                    return 1;
                }
            }
        }
    }
    printf("Valid\n");
    return 0;
}

//function that returns true if position is within the matrix
bool isInMap(Map *map, int r, int c)
{
    if (r > map->rows || r < 1 || c > map->cols || c < 1)
    {
        return false;
    }
    return true;
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
    //check if the user input is within the map
    if (!isInMap(map, r, c))
    {
        fprintf(stderr, "Values of R and C are not within the matrix\n");
        return 1;
    }

    int prefBorder;
    prefBorder = startBorder(map, r, c, RIGHT);

    //check if first position is valid
    if(prefBorder == -1)
    {
        fprintf(stderr,"Invalid starting position\n");
        return 1;
    }

    //print first position
    printf("%d,%d\n", r, c);

    //loop that functions only when r and c are within the boundaries of a map
    while(r <= map->rows && c <= map->cols && r > 0 && c > 0) 
    {
        if (isBorder(map, r-1, c-1, prefBorder) == false)
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
    //check if the user input is within the map
    if (!isInMap(map, r, c))
    {
        fprintf(stderr, "Values of R and C are not within the matrix");
        return 1;
    }

    int prefBorder;
    prefBorder = startBorder(map, r, c, LEFT);

    //check if first position is valid
    if(prefBorder == -1)
    {
        fprintf(stderr,"Invalid starting position\n");
        return 1;
    }

    //print first position
    printf("%d,%d\n", r, c);

    //loop that functions only when r and c are within the boundaries of a map
    while(r <= map->rows && c <= map->cols && r > 0 && c > 0) 
    {
        if (isBorder(map, r-1, c-1, prefBorder) == false)
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

//function that returns true if queue is empty
bool isEmpty(Queue* queue)
{
    if (queue->front != -1)
    {
        return false;
    }
    return true;
}

//function that returns true if position matches certain conditions
bool isDestination(Map *map, int r, int c)
{
    if (r == 0 && (r+c)%2 == 0 && !isBorder(map, r, c, MIDDLE))
    {
        return true;
    }
    else if (r == map->rows-1 && (r+c)%2 == 1 && !isBorder(map, r, c, MIDDLE))
    {
        return true;
    }
    else if (c == 0 && !isBorder(map, r, c, LEFT))
    {
        return true;
    }
    else if (c == map->cols-1 && !isBorder(map, r, c, RIGHT))
    {
        return true;
    }
    return false;
}

//function that adds Position into a queue
void enqueue(Queue *queue, Position p)
{
    if(queue->back == (int)queue->capacity - 1)
    {
        fprintf(stderr,"Queue overflow\n");
        return;
    }
    if (queue->front == -1)
    {
        queue->front = 0;
    }

    queue->back++;
    queue->array[queue->back] = p;
}

//function that returns front Position from a queue
Position dequeue(Queue *queue)
{
    if(isEmpty(queue) == true)
    {
        fprintf(stderr,"Queue underflow\n");
        Position p = {-1, -1};
        return p;
    }

    Position p = queue->array[queue->front];
    queue->front++;

    if (queue->front > queue->back)
    {
        queue->front = queue->back - 1;
    }
    return p;
}

//function that prints shortest path
void printPath(Map *map, Position **parent, Position finish)
{
    //create a stack for storing positions
    Position* stack = (Position*)malloc(map->rows * map->cols *sizeof(Position));
    Position current = finish;
    int index = -1;

    while (current.row != -1 && current.col != -1)
    {
        //add positions from finish to start into stack
        index++;
        stack[index] = current;
        current = parent[current.row][current.col];
    }

    //print stack
    while (index >= 0)
    {
        printf("%d,%d\n", stack[index].row + 1, stack[index].col + 1);
        index--;
    }    

    free(stack);
}

//function with BFS algorithm 
void BFS(Map *map, Position start)
{
    //create arrays for storing info about positions
    int **visited = createVisited(map->rows, map->cols);
    Position** parent = createParent(map->rows, map->cols);
    //create queue struct
    Queue* queue = createQueue(map->rows * map->cols);

    //mark every Position in visited array as unvisited
    //initialize non existing parent for every Position
    for (int i = 0; i < map->rows; i++)
    {
        for (int j = 0; j < map->cols; j++)
        {
            visited[i][j] = 0;
            parent[i][j] = (Position) {-1,-1};
        }
    }

    //mark starting position as visited and add it into a queue
    visited[start.row][start.col] = 1;
    enqueue(queue, start);

    while (!isEmpty(queue))
    {
        //get position from the front of the queue
        Position current = dequeue(queue);

        //iterate throught every possible border
        for (int i = 0; i < 3; i++)
        {
            int newRow = current.row;
            int newCol =  current.col;
            step(i, &newRow, &newCol);
            
            if(isInMap(map, newRow+1, newCol+1) && !isBorder(map, current.row, current.col, i) && visited[newRow][newCol] == 0)
            {
                //mark new position as visited and add parent to it
                visited[newRow][newCol] = 1;
                parent[newRow][newCol] = current;

                //create Position struct for new position and add it into a queue 
                Position adjacent = {newRow, newCol};
                enqueue(queue, adjacent);
                
                //check if the position is not destination
                if(isDestination(map, newRow, newCol))
                {
                    printPath(map, parent, adjacent);

                    freeQueue(queue);
                    freeVisited(visited, map->rows);
                    freeParent(parent, map->rows);
                    return;
                }
            }
        }
    }
    //if queue is empty and no path was found print the following
    printf("No path found\n");

    freeQueue(queue);
    freeVisited(visited, map->rows);
    freeParent(parent, map->rows);
}

//function that finds shortest path in a maze
int sPath(Map *map, int r, int c)
{
    //check if the user input is within the map
    if (!isInMap(map, r, c))
    {
        fprintf(stderr, "Values of R and C are not within the matrix");
        return 1;
    }

    //set start position 
    Position start = {r-1, c-1};
    //find shortest path using BFS algorithm
    BFS(map, start);
    return 0;
}

int main(int argc, char *argv[])
{
    const char *help = "--help";
    const char *test = "--test";
    const char *rpath = "--rpath";
    const char *lpath = "--lpath";
    const char *shortest = "--shortest";

    int rows = 0;
    int cols = 0;
    Map *map = NULL;

    //if there are no arguments in command line exit with an error
    if (argc < 2)
    {
        fprintf(stderr, "No arguments given\n");
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
        if (processFile(argv[2], &rows, &cols, &map) == -1)
        {
            fprintf(stderr,"Error processing a file\n");
            return -1;
        }

        //run test to check if data in the matrix are correct
        testMap(rows, cols, map);

        //free allocated memory
        freeMap(map);
    }
    else if (strcmp(argv[1], rpath) == 0)
    {
        //process file and create struct Map from given data
        if (processFile(argv[4], &rows, &cols, &map) == -1)
        {
            fprintf(stderr,"Error processing a file\n");
            return -1;
        }

        //solve map using right hand rule
        rPath(map, atoi(argv[2]), atoi(argv[3]));

        //free allocated memory
        freeMap(map);
    }
    else if (strcmp(argv[1], lpath) == 0)
    {
        //process file and create struct Map from given data
        if (processFile(argv[4], &rows, &cols, &map) == -1)
        {
            fprintf(stderr,"Error processing a file\n");
            return -1;
        }

        //solve map using left hand rule
        lPath(map, atoi(argv[2]), atoi(argv[3]));

        //free allocated memory
        freeMap(map);
    }
    else if (strcmp(argv[1], shortest) == 0)
    {
        //process file and create struct Map from given data
        if (processFile(argv[4], &rows, &cols, &map) == -1)
        {
            fprintf(stderr,"Error processing a file\n");
            return -1;
        }

        //solve map using left hand rule
        sPath(map, atoi(argv[2]), atoi(argv[3]));

        //free allocated memory
        freeMap(map);
    }

    return 0;
}