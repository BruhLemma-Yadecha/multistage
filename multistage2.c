#include <stdio.h>
#include <stdlib.h>
#include "./helper.c"

// My types
typedef struct rocket
{
    int stages;
    float totaldV;
    float totalMass;
    float dV[2];
    float payload[2];
    float isp[2];
    float fraction[2];
}rocket;

typedef struct rocket_node
{
    rocket nodeRocket;
    struct rocket_node* next;
}rocket_node;

// My functions
void derFractionator(rocket r, float dV, int currentStage); // Generates a dV combo to be used to generate a rocket.
void addRocketToList(rocket r); // Used to add a rocket configuration to the program's main linked list.
void listPrinter(rocket_node *l); // prints the contents of the primary linked list.
void vonBraunClock(rocket *r) // Populates the payload array and the totalMass, returns the finished rocket.

// Constants y Globals
float g = 9.80665;
float increments;
rocket_node* theList;
rocket lightestRocket;

// Temporary variables
int counter = 0;

int main(void)
{
    rocket base;
    // Base parameters
    base.stages = 2;
    base.totaldV = 10000.0;

    // Payload Parameters
    base.payload[0] = 0.0;
    base.payload[1] = 0.0;
    //base.payload[2] = 90.0;

    // Isp
    base.isp[0] = 300.0;
    base.isp[1] = 350.0;
    //base.isp[2] = 400.0;

    // Fraction
    base.fraction[0] = 0.9;
    base.fraction[1] = 0.9;
    //base.fraction[2] = 0.9;

    // Simulator settings.
    increments = 1000.0;

    // Now generate the numbers.
    derFractionator(base, base.totaldV, 0);

    // Now go through the list running vonBraunClock each time to populate the remaining fields.
    rocket_node *pointer = theList;
    for (int i = 0; i < counter; i++)
    {
        vonBraunClock(pointer->nodeRocket);
        if (pointer->nodeRocket.totalMass < lightestRocket.totalMass)
        {
            lightestRocket = pointer->nodeRocket;
        }
        pointer = pointer->next;
    }

    // Printers
    listPrinter(theList);
}

void derFractionator(rocket r, float dV, int currentStage)
{
    if (currentStage != r.stages - 1)
    {
        int runs = dV / increments;
        for (int i = 0; i < runs + 1; i++)
        {
            r.dV[currentStage] = dV - i*increments;
            derFractionator(r, dV - r.dV[currentStage], currentStage + 1);
        }
    }
    else
    {
        r.dV[currentStage] = dV;
        counter++;
        /*
        printf("%d. ", counter);
        floatArrayPrinter(r.stages, r.dV);
        */
        addRocketToList(r);
    }
}

void addRocketToList(rocket s)
{
    if (theList == NULL)
    {
        //printf("Adding a rocket to the list for the first time...\n");
        theList = malloc(sizeof(rocket_node));
        theList->nodeRocket = s;
        theList->next = NULL;
    }
    else
    {
        //printf("Adding a rocket...\n");

        rocket_node* ptr = theList;
        while (ptr->next != NULL)
        {
            ptr = ptr->next;
        }
        
        rocket_node* newNode = malloc(sizeof(rocket_node));
        ptr->next = newNode;
        if (newNode != NULL)
        {
            newNode->nodeRocket = s;
            newNode->next = NULL;
        }
        else
        {
            printf("MALLOC FAILED!\n");
        }
    }
}

void vonBraunClock(rocket *r)
{

}

void listPrinter(rocket_node* l)
{
    rocket_node *ptr = l;
    for (int f = 0; f < counter; f++)
    {
        floatArrayPrinter(ptr->nodeRocket.stages, ptr->nodeRocket.dV);
        ptr = ptr->next;
    }
}