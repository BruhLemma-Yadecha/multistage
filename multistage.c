// This multistager attempts to solve for the ideal rocket
// Using numgen to do a two-stage rocket.
#include <stdio.h>
#include <math.h>
#include "helper.c"

float rocketmancer(int stages, int currentStage, float dV[], float payload[], float isp[], float fraction[]);
void numgen(int numbers, float value, float increments, int step, float arr[], float initialValue, float payload[], float isp[], float fraction[]);
float calculateStageMass(float dV, float payload, float fraction, float isp);
void cleanPayload(int n, float arr[]);

typedef struct rocket
{
    float dv[3];
    float payload[3];
    float isp[3];
    float fraction[3];
}rocket;

float g = 9.80665;
float currentLowest = -1;
int numgenCounter = 0;
int numgenMax = 0;

int main(void)
{
    int stages = 3;
    float dV = 10000.0;
    float payload[3] = {0.0, 0.0, 100.0};
    float isp[3] = {300.0, 400.0, 450.0};
    float fraction[3] = {0.9, 0.9, 0.9};
    float increments = 100.0;

    // Calculate numgenMax so we know when to stop the program.
    numgenMax = (int)(pow(dV/increments, stages - 1) + 1);

    float dvFractions[3];
    numgen(stages, dV, increments, 1, dvFractions, dV, payload, isp, fraction);
}

float rocketmancer(int stages, int currentStage, float dV[], float payload[], float isp[], float fraction[])
{
    // In this case isp[0] == isp of the 1st stage.
    float currentStagePayload = floatArraySum(stages, payload);
    if (currentStage != 0)
    {
        payload[currentStage - 1] = calculateStageMass(dV[currentStage], currentStagePayload, fraction[currentStage], isp[currentStage]);
        rocketmancer(stages, currentStage - 1, dV, payload, isp, fraction);
    }
    else
    {
        float mass = calculateStageMass(dV[currentStage], currentStagePayload, fraction[currentStage], isp[currentStage]) + currentStagePayload;
        if (mass >= 0)
        {
            if (currentLowest == -1)
            {
                currentLowest = mass;
            }
            if (mass < currentLowest)
            {
                currentLowest = mass;
                //printf("| %f ", mass);
                //floatArrayPrinter(stages, dV);
            }
        }
        if (numgenCounter == numgenMax)
        {
            printf("Your ideal rocket weighs %f t", currentLowest);
            return mass;
        }
    }
}

float calculateStageMass(float dV, float payload, float fraction, float isp)
{
    float mass = ((exp((dV) / (isp * g)) - 1) * payload) / (1 - (exp((dV) / (isp * g)))*(1 - fraction));
    return mass;
}

void numgen(int numbers, float value, float increments, int step, float arr[], float initialValue, float payload[], float isp[], float fraction[])
{
    int runs = value / increments;
    for (int i = 0; i < runs + 1; i++)
    {
        arr[step - 1] = value - increments*i;

        if (step != numbers)
        {
            numgen(numbers, value - arr[step - 1], increments, step + 1, arr, initialValue, payload, isp, fraction);
        }
        else
        {
            if (floatArraySum(step, arr) == initialValue)
            {
                numgenCounter++;
                cleanPayload(numbers, payload);
                rocketmancer(numbers, numbers - 1, arr, payload, isp, fraction);
            }
        }
    }
}

void cleanPayload(int n, float arr[])
{
    // Sets all elements but the topmost one to zero.
    for (int i = 0; i < n - 1; i++)
    {
        arr[n - i - 2] = 0.0;
    }
}