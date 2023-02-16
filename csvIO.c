// This includes functions to enable multistage2.c to accept CSV input and output to CSV
#include "./definitions.h"
#include "./multistage_calculators.c"

#define SECTION_LENGTH 128

void csv_columngen(char *target, float arr[], int number_of_elements);

void rocket_csvRowGenerator(rocket source, char *targetString)
{
    int s = source.stages;
    float stage_mass[source.stages];
    float *stage_mass_pointer = stage_mass;

    // Generate stage masses
    stageMassDistribution(source, stage_mass_pointer, s);

    // Initialise each column group
    char *buffer = malloc(SECTION_LENGTH); // This is for every concatenation.

    sprintf(targetString, "%f", source.totalMass); // First add the total mass forcefully to avoid garbage values.
    csv_columngen(targetString, stage_mass, s); // Append stage masses
    csv_columngen(targetString, source.dV, s); // Append dV values
    targetString[strlen(targetString) - 1] = NULL;

}

void csv_columngen(char *target, float arr[], int number_of_elements)
{
    char *buffer = malloc(16); // +1 for the comma.
    for (int i = 0; i < number_of_elements; i++)
    {
        sprintf(buffer, "%f,", arr[i]);
        strcat(target, buffer);
    }
    free(buffer);
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
