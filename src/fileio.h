//
//  fileio.h
//  DA3-Project
//
//  Created by Mathieu De Coster
//  Copyright (c) 2014 Mathieu De Coster. All rights reserved.
//

#ifndef DA3_Project_fileio_h
#define DA3_Project_fileio_h

#include "point.h"

/**
 * Lees de coordinaten in van de veelhoek
 * @param filename Naam van het bestand
 * @param pointCount (out) Het aantal punten in de veelhoek
 * @return Een lijst van punten
 */
Point* readCoords(const char* filename, unsigned int* pointCount);

#endif
