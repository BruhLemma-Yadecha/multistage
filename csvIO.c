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

float rocket_inputFromCSV(rocket *titan, FILE *input_stream)
{
    float increments;
    char *line = malloc(CSV_BUFFER);
    
    // Extracting increment
    //char *line1 = malloc(CSV_BUFFER);
    fgets(line, CSV_BUFFER, input_stream);
    increments = csvLineTrimmer_singleValue(line, 14, 4);
    //free(line1);
    
    // Extracting stages
    fgets(line, CSV_BUFFER, input_stream);
    titan->stages = (int)csvLineTrimmer_singleValue(line, 7, 4);

    // Extract totaldV
    fgets(line, CSV_BUFFER, input_stream);
    titan->totaldV = csvLineTrimmer_singleValue(line, 8, 4);

    // Payload
    fgets(line, CSV_BUFFER, input_stream);
    csvLineTrimmer_manyValues(line, titan->payload, 8, titan->stages);
    
    // Isp
    fgets(line, CSV_BUFFER, input_stream);
    csvLineTrimmer_manyValues(line, titan->isp, 4, titan->stages);

    // Fraction
    fgets(line, CSV_BUFFER, input_stream);
    csvLineTrimmer_manyValues(line, titan->fraction, 9, titan->stages);
   
    free(line);
    return increments;
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
    float total_mass = source.totalMass;

    sprintf(targetString, "%f,", total_mass); // First add the total mass forcefully to avoid garbage values.

    csv_columngen(targetString, stage_mass, s); // Append stage masses

    csv_columngen(targetString, source.dV, s); // Append dV values

    targetString[strlen(targetString) - 1] = 0;

    free(buffer);
}

void csv_columngen(char *target, float arr[], int number_of_elements)
{
    char *buffer = malloc(16); // +1 for the comma.
    for (int i = 0; i < number_of_elements; i++)
    {
        sprintf(buffer, "%.2f,", arr[i]);
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
