//
//  fitness_mpi.h
//  DA3-Project
//
//  Created by Mathieu De Coster
//  Copyright (c) 2014 Mathieu De Coster. All rights reserved.
//

#ifndef __DA3_Project__fitness__
#define __DA3_Project__fitness__

typedef struct organism Organism;

/**
 * Bereken de fitness van een organisme
 * @param organism Het organisme
 * @return De fitnesswaarde
 */
float fitness(Organism* organism);

#endif /* defined(__DA3_Project__fitness__) */
