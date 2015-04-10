//
//  constants.h
//  DA3-Project
//
//  Created by Mathieu De Coster
//  Copyright (c) 2014 Mathieu De Coster. All rights reserved.
//

#ifndef DA3_Project_constants_h
#define DA3_Project_constants_h

#define POPULATION_SIZE 1500 // Begingrootte van de populatie
#define ITERATION_THRESHOLD 200 // Als gedurende ITERATION_THRESHOLD iteraties bijna niets verandert aan de maximum fitness, stop dan
#define FITNESS_THRESHOLD 1.0e-5 // "bijna niets" = minder dan FITNESS_THRESHOLD
#define POINT_MUTATION_CHANCE 5 // 1 / kans dat een punt muteert
#define ORGANISM_MUTATION_CHANCE 10 // 1 / kans dat een organisme muteert
#define MAX_POINT_MUTATION 1.5f // Maximale factor waarmee een punt kan gemuteerd worden
#define OFFSPRING_RATIO 10 // population->size / OFFSPRING_RATIO is het aantal kinderen
#define DEATH_RATIO 10 // population->size / DEATH_RATIO is het aantal doden
#define DEATH_BATCH_SIZE 10 // # selecties per batch
#define BREED_BATCH_SIZE 10 // # selecties per batch
#define MAX_ITERATIONS 10000 // Hoge limiet voor aantal iteraties om te voorkomen dat het programma zeer lang blijft lopen

#endif
