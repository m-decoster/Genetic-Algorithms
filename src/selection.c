//
//  selection.c
//  DA3-Project
//
//  Created by Mathieu De Coster on 14/10/14.
//  Copyright (c) 2014 Mathieu De Coster. All rights reserved.
//

#include "selection.h"
#include "memory.h"
#include "fitness.h"
#include "util.h"
#include <math.h>

void uniformSamplingSelection(Population* population, Organism** selection, unsigned int selectionSize)
{
    unsigned int selectionDistance = population->size / selectionSize;
    unsigned int startIndex = (unsigned int)rand() % selectionDistance;
    for(unsigned int i = 0; i < selectionSize; ++i)
    {
        selection[i] = llGet(population->organisms, (startIndex + i * selectionDistance) % population->size);
    }
}

void batchSelection(Population* population, Organism** selection, unsigned int selectionSize, unsigned int batchSize, float upperThreshold)
{
    // Bereid voor door de geaccumuleerde fitnesswaarde te berekenen
    float* acc;
    float accFitness = 0.0f;
    CMALLOC(acc, sizeof(float), population->size);
    llNode* cur = population->organisms;
    unsigned int i = 0;
    while(cur)
    {
        accFitness += cur->data->fitness;
        acc[i] = accFitness;
        
        cur = cur->next;
        ++i;
    }
    // Normaliseer
    for(unsigned int i = 0; i < population->size; ++i)
    {
        acc[i] /= accFitness;
    }
    
    unsigned int index = 0;
    while(index < selectionSize)
    {
        // Selecteer 1 batch
        unsigned int numSelected = 0;
        float threshold = randf(0.0f, upperThreshold);
        
        llNode* cur = population->organisms;
        unsigned int j = 0;
        while(cur && index < selectionSize)
        {
            if(threshold <= acc[j])
            {
                // Selecteer
                selection[index++] = cur->data;
            }
            
            if(numSelected == batchSize)
            {
                break;
            }
            
            cur = cur->next;
            ++j;
        }
    }
    
    free(acc);
}
