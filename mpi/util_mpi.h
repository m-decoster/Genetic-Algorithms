//
//  util_mpi.h
//  DA3-Project
//
//  Created by Mathieu De Coster
//  Copyright (c) 2014 Mathieu De Coster. All rights reserved.
//

#ifndef __DA3_Project__util__
#define __DA3_Project__util__

#define SQUARE(x) ((x) * (x))

typedef struct population Population;
typedef struct point Point;

/**
 * Genereer een random floating point getal tussen twee waarden
 * @param min De minimum waarde
 * @param max De maximum waarde
 */
float randf(float min, float max);

/**
 * Controleer of een punt in de veelhoek ligt
 * @param point Het punt om te controleren
 * @return 1 indien het punt in de veelhoek ligt, anders 0
 */
unsigned int pointInPoly(Point* point);

/**
 * Controleer of twee floating point getallen dicht genoeg bijeen liggen
 * om gelijk te kunnen zijn
 * @param f1 Getal 1
 * @param f2 Getal 2
 * @return 1 als ze gelijk zijn, anders 0
 */
unsigned int floatEquals(float f1, float f2);

#endif /* defined(__DA3_Project__util__) */
