// This multistager attempts to solve for the ideal rocket
// Using numgen to do a two-stage rocket.
#include <stdio.h>
#include <math.h>
#include "helper.c"

typedef struct rocket
{
    int stages;
    float totaldV;
    float totalMass;
    float dV[3];
    float payload[3];
    float isp[3];
    float fraction[3];
}rocket;

float rocketmancer(rocket r, int currentStage);
void numgen(rocket target, float initialdV, float increments, int step);
float calculateStageMass(float dV, float payload, float fraction, float isp);
void cleanPayload(int n, float arr[]);

float g = 9.80665;
float currentLowest = -1;
int numgenCounter = 0;
int numgenMax = 0;

int main(void)
{
    rocket base;
    // BAse parameters
    base.stages = 3;
    base.totaldV = 10000.0;
    // Payload Parameters
    base.payload[0] = 0.0;
    base.payload[1] = 0.0;
    base.payload[2] = 90.0;
    // Isp
    base.isp[0] = 300.0;
    base.isp[1] = 350.0;
    base.isp[2] = 400.0;
    // Fraction
    base.fraction[0] = 0.9;
    base.fraction[1] = 0.9;
    base.fraction[2] = 0.9;
    // Simulator increments.
    float increments = 500.0;

    // Calculate numgenMax so we know when to stop the program.
    numgenMax = (int)(pow(base.totaldV/increments, base.stages - 1) + 1);

    numgen(base, base.totaldV, increments, 1);
}

//void numgen(int numbers, float value, float increments, int step, float arr[], float initialValue, float payload[], float isp[], float fraction[])
void numgen(rocket target, float initialdV, float increments, int step)
{
    rocket p = target;
    int runs = p.totaldV / increments;
    for (int i = 0; i < runs + 1; i++)
    {
        p.dV[step - 1] = p.totaldV - increments*i;

        if (step != p.stages)
        {
            rocket stage = p;
            stage.totaldV -= p.dV[step - 1];
            numgen(stage, initialdV, increments, step + 1);
        }
        else
        {
            if (floatArraySum(p.stages, p.dV) == initialdV)
            {
                numgenCounter++;
                cleanPayload(p.stages, p.payload);
                rocketmancer(p, p.stages - 1);
                //floatArrayPrinter(p.stages, p.dV);
            }
        }
    }
}

float calculateStageMass(float dV, float payload, float fraction, float isp)
{
    float mass = ((exp((dV) / (isp * g)) - 1) * payload) / (1 - (exp((dV) / (isp * g)))*(1 - fraction));
    return mass;
}

//float rocketmancer(int stages, int currentStage, float dV[], float payload[], float isp[], float fraction[])
float rocketmancer(rocket r, int currentStage)
{
    // In this case isp[0] == isp of the 1st stage.
    float currentStagePayload = floatArraySum(r.stages, r.payload);
    if (currentStage != 0)
    {
        r.payload[currentStage - 1] = calculateStageMass(r.dV[currentStage], currentStagePayload, r.fraction[currentStage], r.isp[currentStage]);
        rocketmancer(r, currentStage - 1);
    }
    else
    {
        float mass = calculateStageMass(r.dV[currentStage], currentStagePayload, r.fraction[currentStage], r.isp[currentStage]) + currentStagePayload;
        printf("Counter: %i, Max: %i\n", numgenCounter, numgenMax);
        if (mass >= 0)
        {
            if (currentLowest == -1)
            {
                currentLowest = mass;
            }
            if (mass < currentLowest)
            {
                currentLowest = mass;
                printf("| %f ", mass);
                floatArrayPrinter(r.stages, r.dV);
            }
        }
        if (numgenCounter == numgenMax)
        {
            printf("Your ideal rocket weighs %f t", currentLowest);
            return mass;
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