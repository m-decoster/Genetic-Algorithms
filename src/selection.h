//
//  selection.h
//  DA3-Project
//
//  Created by Mathieu De Coster on 14/10/14.
//  Copyright (c) 2014 Mathieu De Coster. All rights reserved.
//

#ifndef __DA3_Project__selection__
#define __DA3_Project__selection__

#include "genetic.h"

/**
 * Pas stochastic universal sampling selection toe op een populatie
 * @param population De populatie
 * @param selection (out) De geselecteerde organismen
 * @param selectionSize Het aantal te selecteren organismen
 */
void uniformSamplingSelection(Population* population, Organism** selection, unsigned int selectionSize);

/**
 * Pas selectie (eigen algoritme) toe op een populatie
 * @param population (in/out) De populatie
 * @param selection (out) De geselecteerde organismen
 * @param selectionSize Grootte van de selectie
 * @param batchSize Aantal selecties in 1 batch
 * @param upperThreshold The upper threshold for the accumulated fitness
 */
void batchSelection(Population* population, Organism** selection, unsigned int selectionSize, unsigned int batchSize, float upperThreshold);

#endif /* defined(__DA3_Project__selection__) */
