#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "./definitions.h"
#include "./csvIO.c"
#include "./testing_functions.c"

#define g 9.80665
#define MAX_LENGTH 1024
#define CSV_ROW_LENGTH 1024

// My functions
void derFractionator(rocket r, float dV, int currentStage); // Generates a dV combo to be used to generate a rocket.
void addRocketToList(rocket r); // Used to add a rocket configuration to the program's main linked list. 
void vonBraunClock(rocket *r, int currentStage); // Populates the payload array and the totalMass, returns the finished rocket.
float calculateStageMass(float dV, float payload, float fraction, float isp); // Self-explanatory.
void rocketReport(rocket r); // Generates a breakdown of a rocket.

// Constants y Globals
//float g = 9.80665;
float increments;
rocket_node* theList;
rocket *lightestRocket;

// Temporary variables
int counter = 0;

int main(int argc, char *argv[])
{
    lightestRocket = malloc(sizeof(rocket));
    lightestRocket->totalMass = 3.402823e+38;
    rocket base;
    // Process a CSV file as an input, specified in the command-line arguments
    if (argc == 2)
    {
        FILE *input_csv;
        input_csv = fopen(argv[1], "r");
        increments = rocket_inputFromCSV(&base, input_csv);
        fclose(input_csv);
    }

    // Now generate the numbers.
    derFractionator(base, base.totaldV, 0);

    // Prepare the csv.
    FILE *output_best;
    FILE *output_verbose;
    FILE *output_regular;
    output_best = fopen("output-best.csv", "w");
    output_verbose = fopen("output-verbose.csv", "w");
    output_regular = fopen("output-regular.csv", "w");

    // Write the stage-list & dV list in the format: stage1,stage2,...
    char *stage_list = malloc(MAX_LENGTH);
    char *dV_list = malloc(MAX_LENGTH);
    wordRepeater(stage_list, "stage", 1, base.stages);
    wordRepeater(dV_list, "dV", 1, base.stages);

    // Now form the header of the CSV file.
    fprintf(output_verbose, "Total Mass,%s%s\n", stage_list, dV_list);
    fprintf(output_regular, "Total Mass,%s%s\n", stage_list, dV_list);
    fprintf(output_best, "Total Mass,%s%s\n", stage_list, dV_list);
    free(stage_list);
    free(dV_list);
    
    // Now go through the list running vonBraunClock each time to populate the remaining fields.
    // This will also populate the output CSV file as this loop is destructive.
    rocket_node *pointer = theList;
    
    for (int i = 0; i < counter; i++)
    {
        vonBraunClock(&pointer->nodeRocket, base.stages - 1);
        // Now update the lightest rocket since that's our ultimate output
        float mass = pointer->nodeRocket.totalMass;
        if (mass >= 0)
        {
            // Add a row to the regular CSV
            char *regular_csv_row = malloc(CSV_ROW_LENGTH);
            rocket_csvRowGenerator(pointer->nodeRocket, regular_csv_row);
            fprintf(output_regular, "%s\n", regular_csv_row);
            free(regular_csv_row);

            if (mass < lightestRocket->totalMass)
            {
                *lightestRocket = pointer->nodeRocket;
            }
        }

        // Add a row to the verbose CSV
        char *verbose_csv_row = malloc(CSV_ROW_LENGTH);
        rocket_csvRowGenerator(pointer->nodeRocket, verbose_csv_row);
        fprintf(output_verbose, "%s\n", verbose_csv_row);
        free(verbose_csv_row);

        // Clean up the node
        rocket_node *np = pointer->next;
        free(pointer); // Shrink the list as you go as we already have the data we need.
        pointer = np;
    }

    fclose(output_verbose);
    fclose(output_regular);

    // Best rocket found, generate a rocket report.
    //rocketReport(*lightestRocket);

    // Best rocket found, generate a CSV.
    char *best_csv_row = malloc(CSV_ROW_LENGTH);
    rocket_csvRowGenerator(*lightestRocket, best_csv_row);
    fprintf(output_best, "%s\n", best_csv_row);
    free(best_csv_row);
    fclose(output_best);

    // Cleanup
    free(lightestRocket);
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

void rocketReport(rocket r)
{
    printf("\nHere's your ideal rocket:\nIncrements: %f\n", increments);
    printf("Total Mass: %f t\nPayload: %f t\n", r.totalMass, r.payload[r.stages - 1]);
    
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