//
//  genetic.h
//  DA3-Project
//
//  Created by Mathieu De Coster
//  Copyright (c) 2014 Mathieu De Coster. All rights reserved.
//

#ifndef __DA3_Project__genetic__
#define __DA3_Project__genetic__

#include "point.h"
#include "llist.h"

typedef struct organism
{
    Point* points;
    float fitness;
} Organism;

/**
 * Maak een nieuw organisme aan
 * @param organism (out) Het organisme
 */
void initOrganism(Organism* organism);
/**
 * Verwijder een organisme
 * @param organism Het organisme
 */
void freeOrganism(Organism* organism);

typedef struct population
{
    llNode* organisms;
    unsigned int size;
} Population;

/**
 * Maak een nieuwe populatie aan
 * @param population (out) De populatie
 */
void initPopulation(Population* population);
/**
 * Verwijder een populatie
 * @param population De populatie
 */
void freePopulation(Population* population);

/**
 * Maak kinderen
 * @param population (in/out) De populatie waarin kinderen worden gemaakt
 */
void breed(Population* population);

/**
 * Dood overbodige of slechte organismen
 * @param population (in/out) De populatie waarin organismen sterven
 */
void killOrganisms(Population* population);

/*
 * Enkele globale variabelen
 */
extern Point* gPolygon;
extern unsigned int gPolyLen;
extern unsigned int gNumPoints;

#endif /* defined(__DA3_Project__genetic__) */
