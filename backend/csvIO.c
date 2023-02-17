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
void remove_spaces(char* s);

float rocket_inputFromCSV(rocket *titan, FILE *input_stream)
{
    float increments;
    int cutoff = titan->stages - 1; // This is a bit hacky but if stages or dV give you issues change this.
    char line[CSV_BUFFER];
    
    // Extracting increment
    fgets(line, CSV_BUFFER, input_stream);
    remove_spaces(line);
    increments = csvLineTrimmer_singleValue(line, 14, cutoff);
    
    // Extracting stages
    fgets(line, CSV_BUFFER, input_stream);
    remove_spaces(line);
    titan->stages = (int)csvLineTrimmer_singleValue(line, 7, cutoff);

    // Extract totaldV
    fgets(line, CSV_BUFFER, input_stream);
    remove_spaces(line);
    titan->totaldV = csvLineTrimmer_singleValue(line, 8, cutoff);

    // Payload
    fgets(line, CSV_BUFFER, input_stream);
    csvLineTrimmer_manyValues(line, titan->payload, 8, titan->stages);
    
    // Isp
    fgets(line, CSV_BUFFER, input_stream);
    csvLineTrimmer_manyValues(line, titan->isp, 4, titan->stages);

    // Fraction
    fgets(line, CSV_BUFFER, input_stream);
    csvLineTrimmer_manyValues(line, titan->fraction, 9, titan->stages);
   
    return increments;
}

float csvLineTrimmer_singleValue(char *source, int indexOfFirstDesiredCharacter, int trimmedFromEnd)
{
    char fixed_version[strlen(source)];
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
    return result;
}

void csvLineTrimmer_manyValues(char *source, float arr[], int indexOfFirstDesiredCharacter, int howManyValues)
{
    char usable_string[CSV_BUFFER];
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
}

void rocket_csvRowGenerator(rocket source, char *targetString)
{
    int s = source.stages;
    float stage_mass[source.stages];
    float *stage_mass_pointer = stage_mass;

    // Generate stage masses
    stageMassDistribution(source, stage_mass_pointer, s);

    // Initialise each column group
    float total_mass = source.totalMass;

    sprintf(targetString, "%f,", total_mass); // First add the total mass forcefully to avoid garbage values.

    csv_columngen(targetString, stage_mass, s); // Append stage masses

    csv_columngen(targetString, source.dV, s); // Append dV values

    targetString[strlen(targetString) - 1] = 0;
}

void csv_columngen(char *target, float arr[], int number_of_elements)
{
    char buffer[CSV_BUFFER];
    for (int i = 0; i < number_of_elements; i++)
    {
        sprintf(buffer, "%f,", arr[i]);
        strcat(target, buffer);
    }
}

void wordRepeater(char *targetString, char *repeatedWord, int first_number, int last_number)
{
    char buffer[strlen(repeatedWord) + 4]; // +4 is arbitrary, just works. The higher the better as this is a buffer.
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
}

void remove_spaces(char* s) // From https://stackoverflow.com/questions/1726302/remove-spaces-from-a-string-in-c
{
    char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while (*s++ = *d++);
}
