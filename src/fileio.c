//
//  fileio.c
//  DA3-Project
//
//  Created by Mathieu De Coster
//  Copyright (c) 2014 Mathieu De Coster. All rights reserved.
//

#include "fileio.h"
#include "util.h"
#include "memory.h"
#include <stdio.h>

Point* readCoords(const char* filename, unsigned int* pointCount)
{
    Point* points = NULL;
    
    FILE* fp = fopen(filename, "r");
    if(!fp)
    {
        fprintf(stderr, "Kon coordinaten niet inlezen van bestand %s\n", filename);
        exit(1);
    }
    
    fscanf(fp, "%d", pointCount);
    if(*pointCount <= 0)
    {
        fprintf(stderr, "Ongeldig aantal punten van de veelhoek");
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
