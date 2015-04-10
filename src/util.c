//
//  util.c
//  DA3-Project
//
//  Created by Mathieu De Coster
//  Copyright (c) 2014 Mathieu De Coster. All rights reserved.
//

#include "util.h"
#include "genetic.h"
#include "fitness.h"
#include <stdlib.h>
#include <math.h>

float randf(float min, float max)
{
    float rnumf = rand() / (float)RAND_MAX; // Number between 0 and 1
    return rnumf * (max - min) + min;
}

/**
 * Controleer of een oneindige rechte vanuit point naar rechts snijdt met een zijde
 * @param point Het begin van de rechte
 * @param edgeStart Het begin van de zijde
 * @param edgeEnd Het einde van de zijde
 * @return 1 als er een snijpunt is, anders 0
 */
unsigned int rayIntersection(Point* point, Point* edgeStart, Point* edgeEnd)
{
    if(point->y > fmaxf(edgeStart->y, edgeEnd->y) || point->y < fminf(edgeStart->y, edgeEnd->y))
    {
        return 0; // Buiten bereik van y-waarden
    }
    if(point->x > fmaxf(edgeStart->x, edgeEnd->x))
    {
        return 0; // Te groot: kan nooit snijden
    }
    
    // We willen weten welk punt van het lijnstuk de kleinste y-waarde heeft
    Point lowest = edgeStart->y < edgeEnd->y ? *edgeStart : *edgeEnd;
    
    // Bereken de rico van het lijnstuk edge
    if(floatEquals(edgeEnd->x, edgeStart->x))
    {
        // Verticaal: rico is oneindig
        return 1;
    }
    float ricoEdge = (edgeEnd->y - edgeStart->y) / (edgeEnd->x - edgeStart->x);
    
    if(floatEquals(ricoEdge, 0.0f))
    {
        // Horizontale zijde (|| wegens beperkte precisie van floating point getallen,
        // we weten al dat de zijde horizontaal is aangezien de rico 0 is)
        return floatEquals(point->y, edgeStart->y) || floatEquals(point->y, edgeEnd->y);
    }
    else if(ricoEdge < 0.0f)
    {
        // Vul 'point' in in de vergelijking van de rechte edge
        return point->y <= lowest.y + ricoEdge * (point->x - lowest.x);
    }
    else // ricoEdge > 0.0f
    {
        return point->y >= lowest.y + ricoEdge * (point->x - lowest.x);
    }
}

unsigned int pointInPoly(Point* point)
{
    // We controleren eerst of point binnen het omliggende vierkant ligt van de veelhoek.
    // Als dit niet zo is kan het ook niet binnen het vierkant liggen en kunnen we ingewikkelde
    // berekeningen vermijden
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
    if(point->x < minX || floatEquals(point->x, minX)
       || point->x > maxX || floatEquals(point->x, maxX)
       || point->y < minY || floatEquals(point->y, minY)
       || point->y > maxY || floatEquals(point->y, maxY))
    {
        return 0;
    }
    
    // Bekijk elke zijde apart en tel het aantal snijpunten op. Als dit oneven is, ligt het punt in de veelhoek.
    unsigned int numIntersections = 0;
    for(unsigned int i = 0; i < gPolyLen; ++i)
    {
        Point edgeStart = gPolygon[i];
        Point edgeEnd = gPolygon[(i + 1) % gPolyLen];
        
        if(rayIntersection(point, &edgeStart, &edgeEnd))
        {
            ++numIntersections;
        }
    }
    
    return numIntersections & 1;
}

unsigned int floatEquals(float f1, float f2)
{
    float eps = 1.0e-10;
    return fabsf(f1 - f2) < eps;
}