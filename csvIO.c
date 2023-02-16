// This includes functions to enable multistage2.c to accept CSV input and output to CSV
#include <stdlib.h>
#include "./definitions.h"
#include "./multistage_calculators.c"
#include "./testing_functions.c"

#define SECTION_LENGTH 128
#define CSV_BUFFER 128

void csv_columngen(char *target, float arr[], int number_of_elements);
float csvLineTrimmer_singleValue(char *source, int indexOfFirstDesiredCharacter, int trimmedFromEnd);
void csvLineTrimmer_manyValues(char *source, float arr[], int indexOfFirstDesiredCharacter, int howManyValues);

int rocket_inputFromCSV(rocket *titan, FILE *input_stream)
{
    float increments;
    
    // Extracting increment
    char *line1 = malloc(CSV_BUFFER);
    fgets(line1, CSV_BUFFER, input_stream);
    increments = csvLineTrimmer_singleValue(line1, 14, 4);
    free(line1);
    
    // Extracting stages
    char *line2 = malloc(CSV_BUFFER);
    fgets(line2, CSV_BUFFER, input_stream);
    titan->stages = (int)csvLineTrimmer_singleValue(line2, 7, 4);
    free(line2);

    // Extract totaldV
    char *line3 = malloc(CSV_BUFFER);
    fgets(line3, CSV_BUFFER, input_stream);
    titan->totaldV = csvLineTrimmer_singleValue(line3, 8, 4);
    free(line3);

    // Payload
    char *line4 = malloc(CSV_BUFFER);
    fgets(line4, CSV_BUFFER, input_stream);
    csvLineTrimmer_manyValues(line4, titan->payload, 8, titan->stages);
    free(line4);
    floatArrayPrinter(titan->stages, titan->payload);
    /*
    for(int i = 0; i < strlen(line3); i++)
    {
        printf("[%d] %c\n", i, line2[i]);
        
    }
    */

    

}

float csvLineTrimmer_singleValue(char *source, int indexOfFirstDesiredCharacter, int trimmedFromEnd)
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

void csvLineTrimmer_manyValues(char *source, float arr[], int indexOfFirstDesiredCharacter, int howManyValues)
{
    char *usable_string = malloc(CSV_BUFFER);
    for (int i = indexOfFirstDesiredCharacter; i < strlen(source) - 2; i++)
    {
        if (i != strlen(source) - 1)
        {
            usable_string[i - indexOfFirstDesiredCharacter] = source[i];
        }
        else
        {
            usable_string[i - indexOfFirstDesiredCharacter] = source[i];
        }
    }
    strcat(usable_string, ",");

    int element_counter = 0;
    int firstIndex = 0;
    for (int a = 0; a < strlen(usable_string); a++)
    {
        if (usable_string[a] == 44 && element_counter != howManyValues)
        {
            arr[element_counter] = csvLineTrimmer_singleValue(usable_string, firstIndex, strlen(usable_string) - a);
            firstIndex = a + 1;
            element_counter++;
        }
    }
    free(usable_string);
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
