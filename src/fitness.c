//
//  fitness.c
//  DA3-Project
//
//  Created by Mathieu De Coster
//  Copyright (c) 2014 Mathieu De Coster. All rights reserved.
//

#include <math.h>
#include "fitness.h"
#include "util.h"
#include "genetic.h"

float fitness(Organism* organism)
{
    // Zoals gedefineerd in opgave
    float fitSum = 0.0f;
    
    for(unsigned int i = 0; i < gNumPoints; ++i)
    {
        for(unsigned int j = 0; j < gNumPoints; ++j)
        {
            if(i != j)
            {
                float innerSum = sqrt(
                             SQUARE(organism->points[i].x - organism->points[j].x) +
                             SQUARE(organism->points[i].y - organism->points[j].y));
                fitSum += sqrt(innerSum);
            }
        }
    }
    
    organism->fitness = fitSum;
    return fitSum;
}