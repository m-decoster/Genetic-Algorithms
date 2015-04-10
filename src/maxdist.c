//
//  maxdist.c
//  DA3-Project
//
//  Created by Mathieu De Coster
//  Copyright (c) 2014 Mathieu De Coster. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fileio.h"
#include "genetic.h"
#include "constants.h"
#include "memory.h"
#include "util.h"
#include "fitness.h"

float gMaxFitness;
unsigned int gEndingIterations;
unsigned int gTotalIterations;
Organism* gBestOrganism;

/**
 * Voer het algoritme uit
 */
void maxdist();

int main(int argc, const char * argv[]) {
    if(argc != 3)
    {
        fprintf(stderr, "Gebruik: %s <Aantal Punten> <Bestand>\n", argv[0]);
        return 1;
    }
    
    gNumPoints = atoi(argv[1]);
    gPolygon = readCoords(argv[2], &gPolyLen);
    
    gMaxFitness = 0.0f;
    gEndingIterations = 0;
    gTotalIterations = 0;
    gBestOrganism = NULL;
    
    maxdist();
    
    free(gPolygon);
    
    return 0;
}

/**
 * Controleer of er een oplossing is
 * @param population De populatie
 * @return 1 als er een oplossing is, anders 0
 */
static int solution(Population* population)
{
    ++gTotalIterations;
    
    float bestFitness = 0.0f;
    
    llNode* cur = population->organisms;
    unsigned int i = 0;
    while(cur)
    {
        if(bestFitness < cur->data->fitness)
        {
            bestFitness = cur->data->fitness;
            gBestOrganism = cur->data;
        }
        
        cur = cur->next;
        ++i;
    }
    
    if(fabsf(gMaxFitness - bestFitness) < FITNESS_THRESHOLD)
    {
        ++gEndingIterations;
    }
    else
    {
        gEndingIterations = 0;
    }
    gMaxFitness = bestFitness;
    
    return gEndingIterations == ITERATION_THRESHOLD || gTotalIterations == MAX_ITERATIONS;
}

void maxdist()
{
    // Kies een beginpopulatie
    Population population;
    initPopulation(&population);
    
    // Bepaal het inliggende vierkant van de veelhoek
    float minX = gPolygon[0].x, minY = gPolygon[0].y;
    float maxX = minX, maxY = minY;
    for(unsigned int i = 1; i < gPolyLen; ++i)
    {
        if(gPolygon[i].x < minX)
        {
            minX = gPolygon[i].x;
        }
        else if(gPolygon[i].x > maxX)
        {
            maxX = gPolygon[i].x;
        }
        
        if(gPolygon[i].y < minY)
        {
            minY = gPolygon[i].y;
        }
        else if(gPolygon[i].y > maxY)
        {
            maxY = gPolygon[i].y;
        }
    }
    
    // Genereer organismen at random tussen [[minX, minY], [maxX, maxY]], met alle punten in de veelhoek
    for(unsigned int i = 0; i < POPULATION_SIZE; ++i)
    {
        Organism* organism;
        CMALLOC(organism, sizeof(Organism), 1);
        initOrganism(organism);
        
        for(unsigned int j = 0; j < gNumPoints; ++j)
        {
            do {
                organism->points[j].x = randf(minX, maxX);
                organism->points[j].y = randf(minY, maxY);
            } while(!pointInPoly(&organism->points[j]));
        }
        
        fitness(organism); // Bereken de fitnesswaarde van het organisme en sla ze op
        llAdd(&population.organisms, organism);
    }
    
    // Zoek een oplossing
    unsigned int stop = 0;
    while(!stop)
    {
        breed(&population);
        killOrganisms(&population);
        
        stop = solution(&population);
    }
    
    // Print de oplossing
    printf("%f\n", gMaxFitness);
    Point* bestOrganismPoints = gBestOrganism->points;
    for(unsigned int i = 0; i < gNumPoints; ++i)
    {
        printf("%f %f\n", bestOrganismPoints[i].x, bestOrganismPoints[i].y);
    }
    
    // Einde
    freePopulation(&population);
}
