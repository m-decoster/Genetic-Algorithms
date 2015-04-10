//
//  maxdist_mpi.c
//  DA3-Project
//
//  Created by Mathieu De Coster
//  Copyright (c) 2014 Mathieu De Coster. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <mpi.h>
#include "fileio_mpi.h"
#include "genetic_mpi.h"
#include "constants_mpi.h"
#include "memory_mpi.h"
#include "util_mpi.h"
#include "fitness_mpi.h"

float gMaxFitness;
unsigned int gEndingIterations;
unsigned int gTotalIterations;
Organism* gBestOrganism;

/**
 * Voer het algoritme uit
 */
void maxdist_slave(int rank);
void maxdist_master();

int main(int argc, char* argv[]) {
    if(argc != 3)
    {
        fprintf(stderr, "Gebruik: %s <Aantal Punten> <Bestand>\n", argv[0]);
        return 1;
    }

    int result = MPI_Init(&argc, &argv);
    if(result != MPI_SUCCESS)
    {
        fprintf(stderr, "Kon MPI niet initialiseren\n");
        return 2;
    }
    
    gNumPoints = atoi(argv[1]);
    gPolygon = readCoords(argv[2], &gPolyLen);
    
    gMaxFitness = 0.0f;
    gEndingIterations = 0;
    gTotalIterations = 0;
    gBestOrganism = NULL;

    // Definieer een type voor Point
    const int     nBlocks = 2;
    int           blockLengths[2] = { 1, 1 };
    MPI_Datatype  types[2] = { MPI_FLOAT, MPI_FLOAT };
    MPI_Aint      offsets[2];

    offsets[0] = offsetof(Point, x);
    offsets[1] = offsetof(Point, y);

    MPI_Type_create_struct(nBlocks, blockLengths, offsets, types,
            &mpi_pointType);
    MPI_Type_commit(&mpi_pointType);

    // Voer uit
    int mpi_id;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_id);
    if(mpi_id == 0)
    {
        maxdist_master();
    }
    else
    {
        maxdist_slave(mpi_id);
    }
    
    free(gPolygon);

    MPI_Finalize();
    
    return 0;
}

void maxdist_slave(int rank)
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
    unsigned int iterations = 0;
    while(!stop)
    {
        ++iterations;

        breed(&population);
        killOrganisms(&population);

        if(iterations == COMM_IT_LIMIT)
        {
            iterations = 0;

            // Wacht tot master klaar is om data te ontvangen
            char reqtype;
            MPI_Status status;
            MPI_Recv(&reqtype, 1, MPI_CHAR, 0, SEND_FROM_MASTER, MPI_COMM_WORLD, &status);

            stop = reqtype == SEND_STOP;
            
            if(!stop)
            {
                // Vorm de data om naar een lijst van punten en van fitness
                Point* points;
                CMALLOC(points, sizeof(Point), POPULATION_SIZE * gNumPoints);
                float* fitnesses;
                CMALLOC(fitnesses, sizeof(float), POPULATION_SIZE);

                llNode* cur = population.organisms;
                unsigned int index = 0;
                while(cur)
                {
                    for(unsigned int p = 0; p < gNumPoints; ++p)
                    {
                        points[index * gNumPoints + p] = cur->data->points[p];
                    }
                    fitnesses[index] = cur->data->fitness;

                    ++index;
                    cur = cur->next;
                }

                // Verstuur de data
                MPI_Send(points, POPULATION_SIZE * gNumPoints, mpi_pointType,
                        0, SEND_POINTS, MPI_COMM_WORLD);
                MPI_Send(fitnesses, POPULATION_SIZE, MPI_FLOAT,
                        0, SEND_FITNESS, MPI_COMM_WORLD);

                // Ontvang de aangepaste data
                MPI_Status status;
                MPI_Recv(points, POPULATION_SIZE * gNumPoints, mpi_pointType,
                        0, SEND_POINTS_BACK, MPI_COMM_WORLD, &status);
                MPI_Recv(fitnesses, POPULATION_SIZE, MPI_FLOAT,
                        0, SEND_FITNESS_BACK, MPI_COMM_WORLD, &status);

                // Maak een nieuwe populatie aan met deze nieuwe data
                freePopulation(&population);
                initPopulation(&population);
                for(unsigned int i = 0; i < POPULATION_SIZE; ++i)
                {
                    Organism* o;
                    CMALLOC(o, sizeof(Organism), 1);
                    initOrganism(o);

                    for(unsigned int p = 0; p < gNumPoints; ++p)
                    {
                        o->points[p] = points[i * gNumPoints + p];
                    }
                    o->fitness = fitnesses[i];

                    llAdd(&population.organisms, o);
                }

                // Cleanup
                free(fitnesses);
                free(points);
            }
        }
    }

    freePopulation(&population);
}

void maxdist_master()
{
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // We zullen output verzamelen van alle processen
    Point** allPoints;
    float** allFitnesses;

    CMALLOC(allPoints, sizeof(Point*), size - 1);
    CMALLOC(allFitnesses, sizeof(float*), size - 1);

    for(int i = 1; i < size; ++i)
    {
        CMALLOC(allPoints[i - 1], sizeof(Point), POPULATION_SIZE * gNumPoints);
        CMALLOC(allFitnesses[i - 1], sizeof(float), POPULATION_SIZE);
    }

    unsigned int stop = 0, iterations = 0;
    while(!stop)
    {
        // Controleer of we moeten stoppen
        // TODO: "goede oplossing"
        if(++iterations >= MAX_ITERATIONS)
        {
            char reqtype = SEND_STOP;
            for(int i = 1; i < size; ++i)
            {
                MPI_Send(&reqtype, 1, MPI_CHAR, i, SEND_FROM_MASTER, MPI_COMM_WORLD);
            }
            stop = 1;
            break;
        }

        // Vraag de slave processen om output
        for(int i = 1; i < size; ++i)
        {
            char reqtype = SEND_DATA_PLEASE;
            MPI_Send(&reqtype, 1, MPI_CHAR, i, SEND_FROM_MASTER, MPI_COMM_WORLD);
            
            MPI_Status status;
            MPI_Recv(allPoints[i - 1], POPULATION_SIZE * gNumPoints, mpi_pointType, i,
                    SEND_POINTS, MPI_COMM_WORLD, &status);
            MPI_Recv(allFitnesses[i - 1], POPULATION_SIZE, MPI_FLOAT, i,
                    SEND_FITNESS, MPI_COMM_WORLD, &status);
        }

        // Wissel COMM_SWAP_COUNT organismen tussen processen i en i + 1
        // (wrap)
        for(int i = 1; i < size; ++i)
        {
            int indexFirst = i - 1;
            int indexSecond = i == size - 1
                ? 1
                : i;
            for(int swap = 0; swap < COMM_SWAP_COUNT; ++swap)
            {
                // Wissel het organisme op een random index
                Point* p1 = allPoints[indexFirst];
                Point* p2 = allPoints[indexSecond];

                float* f1 = allFitnesses[indexFirst];
                float* f2 = allFitnesses[indexSecond];

                unsigned int randomIndex = rand() % POPULATION_SIZE;

                for(unsigned int p = 0; p < gNumPoints; ++p)
                {
                    unsigned int pointIndex = randomIndex * gNumPoints + p;
                    Point temp = p1[pointIndex];
                    p1[pointIndex] = p2[pointIndex];
                    p2[pointIndex] = temp;
                }
                float temp = f1[randomIndex];
                f1[randomIndex] = f2[randomIndex];
                f2[randomIndex] = temp;
            }
        }

        // Stuur de slave processen hun nieuwe input
        for(int i = 1; i < size; ++i)
        {
            MPI_Send(allPoints[i - 1], POPULATION_SIZE * gNumPoints, mpi_pointType, i,
                    SEND_POINTS_BACK, MPI_COMM_WORLD);
            MPI_Send(allFitnesses[i - 1], POPULATION_SIZE, MPI_FLOAT, i,
                    SEND_FITNESS_BACK, MPI_COMM_WORLD);
        }
    }

    // Print de oplossing
    // We overlopen hiertoe alle organismen en selecteren het beste
    int bestPopulationIndex = 0;
    int bestOrganismIndex = 0;
    float bestFitness = 0.0f;
    for(int i = 1; i < size; ++i)
    {
        for(int o = 0; o < POPULATION_SIZE; ++o)
        {
            if(allFitnesses[i - 1][o] > bestFitness)
            {
                bestPopulationIndex = i - 1;
                bestOrganismIndex = o;
                bestFitness = allFitnesses[i - 1][o];
            }
        }
    }
    MY_PRINTF(stdout, "%f\n", bestFitness);
    for(int p = 0; p < gNumPoints; ++p)
    {
        Point point = allPoints[bestPopulationIndex][bestOrganismIndex * gNumPoints + p];
        MY_PRINTF(stdout, "%f %f\n", point.x, point.y);
    }

    // Cleanup
    for(int i = 1; i < size; ++i)
    {
        free(allFitnesses[i - 1]);
        free(allPoints[i - 1]);
    }
    free(allFitnesses);
    free(allPoints);
}
