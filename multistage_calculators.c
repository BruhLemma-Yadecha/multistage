#include "./definitions.h"

void stageMassDistribution(rocket r, float arr[], int stages)
{
    for (int p = stages - 1; p > -1; p--)
    {
        if (p != 0)
        {
           arr[p] = r.payload[p - 1] - r.payload[p];
        }
        else
        {
            arr[p] = r.totalMass - r.payload[0];
        }
    }
}

float floatArraySum(int elements, float arr[])
{
    float sum = 0;
    for (int i = 0; i < elements; i++)
    {
        sum += arr[i];
    }
    return sum;
}

float zero_spotter(float arr[], int elements)
{
    float product = 1;
    for (int i = 0; i < elements; i++)
    {
        product = product * arr[i];
    }
    return product;
}