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