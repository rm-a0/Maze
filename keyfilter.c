//xrepcim00
//28.10.2023

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define MAX_LENGTH 101

//function that converst string to upper case
void toUpper (char *str)
{   
    for (int i = 0; i < (int)strlen(str); i++)
    {
        if (str[i] >= 'a' && str[i] <= 'z')
        {
            str[i] = str[i] - ('a' - 'A');
        }
    }
}

//function that appends arguments from command line to one string
void getInput (int argc, char *argv[], char *searchInput)
{
    //if there is no input create an empty string
    if (argc == 1) 
    {
        strcat(searchInput, "");
    } 
    else 
    {
        for (int i = 1; i < argc; i++) 
        {
            //if there is multi-word input, append all arguments into one string and add spaces between them
            strcat(searchInput, argv[i]);

            //ensures that there is no empty space after the last argument
            if (argc - 1 != i) 
            {
                strcat(searchInput, " ");
            }
        }
    }
    //converst input to upper case
    toUpper(searchInput);
}

//function that checks if there are duplicates in a string
bool dupCheck (char *enableKeys, char *appendKey)
{
    for (int i = 0; i < (int)strlen(enableKeys); i++)
    {
        if (appendKey[0] == enableKeys[i])
        {
            return true;
        }
    }
    return false;
}

//function that sorts string
void sortString (char *str)
{
    int lenght = strlen(str);
    for (int i = 0; i < lenght; i++)
    {
        for (int j = 0; j < lenght - 1; j++)
        {
            if (str[j] > str[j + 1])
            {
                char temp = str[j];
                str[j] = str[j + 1];
                str[j + 1] = temp;
            }
        }
    }
}

//function that compares 2 strings
void compareStrings (char *adressList, char *searchInput, char *enableKeys, char *matchingAdress, int *matchingAdressCounter)
{
    int lenght = strlen(searchInput);
    char appendKey[] = "0";

    
    for (int i = 0; i < lenght + 1; i++)
    {
        if (searchInput[i] == adressList[i])
        {
            continue;
        }
        else
        {
            //checks if the position of i is equal to number of characters in search input
            //this ensures that only consecutive characters are appended
            if(lenght == i)
            {
                appendKey[0] = adressList[i];
                //appends character that is not matching and doesnt have duplicate into enableKeys
                if(!(dupCheck(enableKeys, appendKey)))
                {
                    strcat(enableKeys, appendKey);
                    strcat(matchingAdress, adressList);
                    (*matchingAdressCounter) ++;
                    break;
                }
                //doesn't append duplicated character
                else
                {
                    (*matchingAdressCounter) ++;
                    break;
                }               
            }
            else
            {
                break;
            }
        }
    }
}

int main (int argc, char *argv[])
{
    char searchInput[MAX_LENGTH] = "";
    char adressList[MAX_LENGTH] = "";
    char enableKeys[MAX_LENGTH] = "";
    char matchingAdress[MAX_LENGTH] = "";
    //keeps track of how many matching adresses were found
    int matchingAdressCounter = 0;

    getInput(argc, argv, searchInput);

    while(fgets(adressList, MAX_LENGTH, stdin) != NULL)
    {
        toUpper(adressList);
        compareStrings(adressList, searchInput, enableKeys, matchingAdress, &matchingAdressCounter);
    }

    if (matchingAdressCounter == 1)
    {
        printf("Found: %s", matchingAdress);
    }
    else if (strlen(enableKeys) > 0 && matchingAdressCounter != 1)
    {
        sortString(enableKeys);
        printf("Enable: %s", enableKeys);
    }
    else
    {
        printf("Not found\n");
    }
    return 0;
}