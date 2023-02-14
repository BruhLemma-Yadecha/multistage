#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
void vonBraunClock(rocket *r, int currentStage); // Populates the payload array and the totalMass, returns the finished rocket.
float calculateStageMass(float dV, float payload, float fraction, float isp); // Self-explanatory.
void rocketReport(rocket r); // Generates a breakdown of a rocket.

// Constants y Globals
float g = 9.80665;
float increments;
rocket_node* theList;
rocket *lightestRocket;

// Temporary variables
int counter = 0;

int main(void)
{
    rocket base;
    lightestRocket = malloc(sizeof(rocket));
    lightestRocket->totalMass = 3.402823e+38;
    // Base parameters
    base.stages = 2;
    base.totaldV = 10000.0;

    // Payload Parameters
    base.payload[0] = 0.0;
    base.payload[1] = 100.0;
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
    increments = 1.0;

    // Now generate the numbers.
    derFractionator(base, base.totaldV, 0);

    // Now go through the list running vonBraunClock each time to populate the remaining fields.
    rocket_node *pointer = theList;
    for (int i = 0; i < counter; i++)
    {
        vonBraunClock(&pointer->nodeRocket, base.stages - 1);
        float mass = pointer->nodeRocket.totalMass;
        if (mass >= 0)
        {
            if (mass < lightestRocket->totalMass)
            {
                *lightestRocket = pointer->nodeRocket;
            }
        }
        pointer = pointer->next;
    }

    // Best rocket found, generate a rocket report.
    listEater(theList);
    rocketReport(*lightestRocket);

    // Printers
    //listPrinter(theList);
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

void vonBraunClock(rocket *r, int currentStage)
{
    float currentStagePayload = floatArraySum(r->stages, r->payload);
    if (currentStage != 0)
    {
        r->payload[currentStage - 1] = calculateStageMass(r->dV[currentStage], currentStagePayload, r->fraction[currentStage], r->isp[currentStage]);
        vonBraunClock(r, currentStage - 1);
    }
    else
    {
        r->totalMass = calculateStageMass(r->dV[currentStage], currentStagePayload, r->fraction[currentStage], r->isp[currentStage]) + currentStagePayload;
        return;
    }
}

float calculateStageMass(float dV, float payload, float fraction, float isp)
{
    float mass = ((exp((dV) / (isp * g)) - 1) * payload) / (1 - (exp((dV) / (isp * g)))*(1 - fraction));
    return mass;
}

void listPrinter(rocket_node* l)
{
    rocket_node *ptr = l;
    for (int f = 0; f < counter; f++)
    {
        printf("| %f ", ptr->nodeRocket.totalMass);
        floatArrayPrinter(ptr->nodeRocket.stages, ptr->nodeRocket.dV);
        ptr = ptr->next;
    }
}

void rocketReport(rocket r)
{
    printf("\nHere's your ideal rocket:\nIncrements: %f\n", increments);
    printf("Total Mass: %f t\nPayload: %f t\n", r.totalMass, r.payload[r.stages - 1]);
    float massSoFar = 0;
    for (int p = r.stages - 1; p > -1; p--)
    {
        printf("----------Stage %d----------\n", p + 1);
        printf("Payload: %f\n", r.payload[p]);

        if (p != 0)
        {
            printf("Stage Mass: %f\n", r.payload[p - 1] - r.payload[p]);
        }
        else
        {
            printf("Stage Mass: %f\n", r.totalMass - r.payload[0]);
        }
        printf("Stage dV: %f\n", r.dV[p]);
    }
}

void listEater(rocket_node *l)
{
    rocket_node* ptr = l;
    if (ptr != NULL)
    {
        if (ptr->next != NULL)
        {
            listEater(ptr->next);
        }
        else
        {
            free(ptr);
        }
    }
}