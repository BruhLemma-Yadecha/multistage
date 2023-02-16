#include <stdio.h>

float floatArraySum(int elements, float arr[])
{
    float sum = 0;
    for (int i = 0; i < elements; i++)
    {
        sum += arr[i];
    }
    return sum;
}

void floatArrayPrinter(int elements, float arr[])
{
    for (int i = 0; i < elements; i++)
    {
        printf("| %f ", arr[i]);
    }
    printf("|\n");
}

void wordRepeater(char *targetString, char *repeatedWord, int first_number, int last_number)
{
    char *buffer = malloc(strlen(repeatedWord) + 4); // +4 for safety
    for (int i = first_number; i < last_number + 1; i++)
    {
        if (i != first_number)
        {
            sprintf(buffer, "%s%d,", repeatedWord, i);
            strcat(targetString, buffer);
        }
        else
        {
            sprintf(targetString, "%s%d", repeatedWord, first_number);
            strcat(targetString, ",");
        }
    }
    free(buffer);
}

void csvRowBuilder(char *target, float arr[], int number_of_elements)
{
    char *buffer = malloc(16); // +1 for the comma.
    for (int i = 0; i < number_of_elements; i++)
    {
        if(i != 0)
        {
            sprintf(buffer, "%f,", arr[i]);
            strcat(target, buffer);
        }
        else
        {
            sprintf(target, "%f,", arr[0]);
        }
    }
    free(buffer);
}