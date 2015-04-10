//
//  fileio_mpi.h
//  DA3-Project
//
//  Created by Mathieu De Coster
//  Copyright (c) 2014 Mathieu De Coster. All rights reserved.
//

#ifndef DA3_Project_fileio_h
#define DA3_Project_fileio_h

#include "point_mpi.h"
#include <mpi.h>

/**
 * Lees de coordinaten in van de veelhoek
 * @param filename Naam van het bestand
 * @param pointCount (out) Het aantal punten in de veelhoek
 * @return Een lijst van punten
 */
Point* readCoords(const char* filename, unsigned int* pointCount);

/**
 * Gebruik deze macro om te printen met MPI:
 * Zo wordt enkel geprint als we proces 0 zijn
 * @param stream stdout / stderr
 */
#define MY_PRINTF(stream, ...) \
{ \
    int my_id; \
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id); \
    fprintf(stream, __VA_ARGS__); \
}

#endif
