//
//  fileio_mpi.c
//  DA3-Project
//
//  Created by Mathieu De Coster
//  Copyright (c) 2014 Mathieu De Coster. All rights reserved.
//

#include "fileio_mpi.h"
#include "util_mpi.h"
#include "memory_mpi.h"
#include <stdio.h>

Point* readCoords(const char* filename, unsigned int* pointCount)
{
    Point* points = NULL;
    
    FILE* fp = fopen(filename, "r");
    if(!fp)
    {
        MY_PRINTF(stderr, "Kon coordinaten niet inlezen van bestand %s\n", filename);
        exit(1);
    }
    
    fscanf(fp, "%d", pointCount);
    if(*pointCount <= 0)
    {
        MY_PRINTF(stderr, "Ongeldig aantal punten van de veelhoek");
        exit(1);
    }
    
    unsigned int line = 0;
    CMALLOC(points, sizeof(Point), *pointCount);
    
    while(fscanf(fp, "%f %f", &points[line].x, &points[line].y) != EOF)
    {
        ++line;
    }
    
    fclose(fp);
    
    return points;
}
