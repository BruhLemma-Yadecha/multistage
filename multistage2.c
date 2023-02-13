#include <stdio.h>
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

// My functions
void derFractionator(rocket r, float dV, int currentStage);

// Constants y Globals
float g = 9.80665;
float increments;

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
        floatArrayPrinter(r.stages, r.dV);
    }
}