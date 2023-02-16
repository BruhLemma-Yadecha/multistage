// This includes functions to enable multistage2.c to accept CSV input and output to CSV
#include <stdlib.h>
#include "./definitions.h"
#include "./multistage_calculators.c"

#define SECTION_LENGTH 128
#define CSV_BUFFER 128

void csv_columngen(char *target, float arr[], int number_of_elements);
float csvLineTrimmer(char *source, int indexOfFirstDesiredCharacter, int trimmedFromEnd);

int rocket_inputFromCSV(rocket *titan, FILE *input_stream)
{
    float increments;
    
    // Extracting increment
    char *line1 = malloc(CSV_BUFFER);
    fgets(line1, CSV_BUFFER, input_stream);
    char *line1_fixed = malloc(CSV_BUFFER);
    for(int i = 14; i < strlen(line1) - 4; i++)
    {
        if (i != 14)
        {
            line1_fixed[i - 14] = line1[i];
        }
        else
        {
            sprintf(line1_fixed, "%c", line1[i]); // This fixes garbage values showing up as a result of malloc.
        }
    }
    increments = atof(line1_fixed);
    free(line1);
    free(line1_fixed);
    
    /*
    for(int i = 0; i < strlen(line2); i++)
    {
        printf("[%d] %c\n", i, line2[i]);
        
    }
    */

    // Extracting stages
    char *line2 = malloc(CSV_BUFFER);
    fgets(line2, CSV_BUFFER, input_stream);
    titan->stages = (int)csvLineTrimmer(line2, 7, 4);
    free(line2);

}

float csvLineTrimmer(char *source, int indexOfFirstDesiredCharacter, int trimmedFromEnd)
{
    char *fixed_version = malloc(strlen(source));
    for(int i = indexOfFirstDesiredCharacter; i < strlen(source) - trimmedFromEnd; i++)
    {
        if (i != indexOfFirstDesiredCharacter)
        {
            fixed_version[i - indexOfFirstDesiredCharacter] = source[i];
        }
        else
        {
            sprintf(fixed_version, "%c", source[i]); // This fixes garbage values showing up as a result of malloc.
        }
    }
    float result = atof(fixed_version);
    free(fixed_version);
    return result;
}

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

    free(buffer);
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
