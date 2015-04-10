//
//  memory_mpi.h
//  DA3-Project
//
//  Created by Mathieu De Coster
//  Copyright (c) 2014 Mathieu De Coster. All rights reserved.
//

#ifndef DA3_Project_memory_h
#define DA3_Project_memory_h

#include <stdio.h>
#include <stdlib.h>
#include "fileio_mpi.h"

// Dit bestand bevat macro's om geheugen te alloceren en te controleren of dit gelukt is

#define CMALLOC(var, size, count) \
var = malloc((size) * (count)); \
if(!var) { MY_PRINTF(stderr, "CMALLOC: Kon geen geheugen alloceren\n"); exit(1); }

#define CCALLOC(var, size, count) \
var = calloc((count), (size)); \
if(!var) { MY_PRINTF(stderr, "CCALLOC: Kon geen geheugen alloceren\n"); exit(1); }

#define CREALLOC(var, size, count) \
var = realloc(var, (size) * (count)); \
if(!var) { MY_PRINTF(stderr, "CREALLOC: Kon geen geheugen alloceren\n"); exit(1); }

#endif
