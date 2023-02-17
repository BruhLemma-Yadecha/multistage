#ifndef DEFINITIONS_H
#define DEFINITIONS_H

typedef struct rocket
{
    int stages;
    float totaldV;
    float totalMass;
    float dV[10];
    float payload[10];
    float isp[10];
    float fraction[10];
}rocket;

typedef struct rocket_node
{
    rocket nodeRocket;
    struct rocket_node* next;
}rocket_node;
#endif