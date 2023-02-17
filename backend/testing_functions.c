#ifndef TESTING_FUNCTIONS_C
#define TESTING_FUNCTIONS_C
#include "./definitions.h"
#include <stdio.h>

void floatArrayPrinter(int elements, float arr[]) // Prints an array of floats.
{
    for (int i = 0; i < elements; i++)
    {
        printf("| %f ", arr[i]);
    }
    printf("|\n");
}

void node_printer(rocket_node* l) // prints the contents of the specified linked list node.
{
    rocket_node *ptr = l;
    printf("| %f ", ptr->nodeRocket.totalMass);
    floatArrayPrinter(ptr->nodeRocket.stages, ptr->nodeRocket.dV);
}
#endif