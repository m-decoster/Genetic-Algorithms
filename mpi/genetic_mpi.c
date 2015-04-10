//
//  genetic_mpi.c
//  DA3-Project
//
//  Created by Mathieu De Coster
//  Copyright (c) 2014 Mathieu De Coster. All rights reserved.
//

#include "genetic_mpi.h"
#include "memory_mpi.h"
#include "util_mpi.h"
#include "constants_mpi.h"
#include "fitness_mpi.h"
#include "selection_mpi.h"

void initOrganism(Organism* organism)
{
    organism->fitness = 0.0f;
    CMALLOC(organism->points, sizeof(Point), gNumPoints);
}

void freeOrganism(Organism* organism)
{
    if(organism)
    {
        free(organism->points);
        free(organism);
    }
}

void initPopulation(Population* population)
{
    CMALLOC(population->organisms, sizeof(llNode), 1);
    llInit(population->organisms, NULL);
    population->size = POPULATION_SIZE;
}

void freePopulation(Population* population)
{
    llNode* cur = population->organisms;
    while(cur != NULL)
    {
        freeOrganism(cur->data);
        llNode* temp = cur;
        cur = cur->next;
        free(temp);
    }
}

void mutate(Organism* organism)
{
    if(rand() % ORGANISM_MUTATION_CHANCE == 0)
    {
        for(unsigned int i = 0; i < gNumPoints; ++i)
        {
            if(rand() % POINT_MUTATION_CHANCE == 0)
            {
                Point p = organism->points[i];
                p.x *= randf(0.0f, MAX_POINT_MUTATION);
                p.y *= randf(0.0f, MAX_POINT_MUTATION);
                if(pointInPoly(&p))
                {
                    // Muteer enkel als het punt in de veelhoek blijft
                    organism->points[i] = p;
                }
            }
        }
    }
}

void breed(Population* population)
{
    // Selecteer organismen om kinderen te krijgen
    // MPI_Implementatie: dit wordt verdeeld over de verschillende processen
    Organism** parents;
    Organism** children;

    int numProcesses;
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    
    unsigned int offspringSize = population->size / OFFSPRING_RATIO;
    unsigned int parentSize = offspringSize * 2;
    
    // Sanity check: vermijd deling door 0
    if(population->size / (float)OFFSPRING_RATIO < 1.0f)
    {
        return;
    }
    
    CMALLOC(parents, sizeof(Organism*), parentSize);
    CMALLOC(children, sizeof(Organism*), offspringSize);
    
    batchSelection(population, parents, parentSize, BREED_BATCH_SIZE, 0.9f);
    
    unsigned int* isParent;
    CCALLOC(isParent, sizeof(unsigned int), parentSize); // 1 als een ouder reeds in een koppel is
    
    for(unsigned int i = 0; i < offspringSize; ++i)
    {
        // Selecteer twee ouders
        unsigned int parent1Index;
        do
        {
            parent1Index = (unsigned int)rand() % parentSize;
        } while(isParent[parent1Index]);
        isParent[parent1Index] = 1;
        unsigned int parent2Index;
        do
        {
            parent2Index = (unsigned int)rand() % parentSize;
        } while(isParent[parent2Index]);
        isParent[parent2Index] = 1;

        // Maak een kind (huidige implementatie: 1 point crossover)
        Organism* child;
        CMALLOC(child, sizeof(Organism), 1);
        initOrganism(child);
        
        unsigned int crossOverIndex = (unsigned int)rand() % gNumPoints;
        for(unsigned int j = 0; j < crossOverIndex; ++j)
        {
            child->points[j].x = parents[parent1Index]->points[j].x;
            child->points[j].y = parents[parent1Index]->points[j].y;
        }
        for(unsigned int j = crossOverIndex; j < gNumPoints; ++j)
        {
            child->points[j].x = parents[parent2Index]->points[j].x;
            child->points[j].y = parents[parent2Index]->points[j].y;
        }

        mutate(child);
        fitness(child);
        
        children[i] = child;
    }

    // Voeg alle kinderen toe
    for(unsigned int i = 0; i < offspringSize; ++i)
    {
        llAdd(&population->organisms, children[i]);
    }
    population->size += offspringSize;

    free(parents);
    free(children);
    free(isParent);
}

void killOrganisms(Population* population)
{
    unsigned int numberOfDeaths = 0;
    
    // Dood enkele willekeurig geselecteerde organismen
    unsigned int deathRowSize = POPULATION_SIZE / DEATH_RATIO;
    
    Organism** deathRow;
    CMALLOC(deathRow, sizeof(Organism*), deathRowSize);
    
    //uniformSamplingSelection(population, deathRow, deathRowSize);//, DEATH_BATCH_SIZE, 0.5f);
    batchSelection(population, deathRow, deathRowSize, DEATH_BATCH_SIZE, 0.5f);
    for(unsigned int i = 0; i < deathRowSize; ++i)
    {
        llNode* cur = population->organisms;
        llNode* prev = NULL;
        while(cur != NULL)
        {
            if(cur->data == deathRow[i])
            {
                ++numberOfDeaths;
                freeOrganism(cur->data);
                if(prev != NULL)
                {
                    prev->next = cur->next;
                    free(cur);
                    cur = prev->next;
                }
                else
                {
                    // cur was begin van de lijst
                    prev = population->organisms->next;
                    population->organisms = prev;
                    free(cur);
                    if(prev != NULL)
                    {
                        cur = prev->next;
                    }
                    else
                    {
                        // We mogen stoppen: als prev NULL is zijn we aan het einde van de lijst
                        cur = NULL;
                    }
                }
            }
            else
            {
                prev = cur;
                cur = cur->next;
            }
        }
    }
    
    free(deathRow);
    
    population->size -= numberOfDeaths;
}

Point* gPolygon;
unsigned int gPolyLen;
unsigned int gNumPoints;
MPI_Datatype mpi_pointType;
