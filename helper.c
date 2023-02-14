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