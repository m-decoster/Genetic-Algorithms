//
//  constants_mpi.h
//  DA3-Project
//
//  Created by Mathieu De Coster
//  Copyright (c) 2014 Mathieu De Coster. All rights reserved.
//

#ifndef DA3_Project_constants_h
#define DA3_Project_constants_h

#define POPULATION_SIZE 500 // Begingrootte van de populatie
#define COMM_IT_LIMIT 200 // Na COMM_IT_LIMIT iteraties wordt er gecommuniceerd tussen processen
#define COMM_SWAP_COUNT 100 // Het aantal organismen dat wordt doorgegeven
#define MAX_ITERATIONS 5 // COMM_IT_LIMIT * MAX_ITERATIONS = Het totaal aantal iteraties dat het algoritme loopt

#define POINT_MUTATION_CHANCE 5 // 1 / kans dat een punt muteert
#define ORGANISM_MUTATION_CHANCE 10 // 1 / kans dat een organisme muteert
#define MAX_POINT_MUTATION 1.5f // Maximale factor waarmee een punt kan gemuteerd worden
#define OFFSPRING_RATIO 10 // population->size / OFFSPRING_RATIO is het aantal kinderen
#define DEATH_RATIO 10 // population->size / DEATH_RATIO is het aantal doden
#define DEATH_BATCH_SIZE 10 // # selecties per batch
#define BREED_BATCH_SIZE 10 // # selecties per batch

#define SEND_POINTS 0x1 // MPI Message Points
#define SEND_POINTS_BACK 0x2 // MPI Message Points Back
#define SEND_FITNESS_BACK 0x3 // MPI Message Fitness Back
#define SEND_FROM_MASTER 0x4 // MPI Message van master proces
#define SEND_DATA_PLEASE 0x5 // Vraag een proces om data te sturen naar 0
#define SEND_STOP 0x6 // Wordt verstuurd naar slaves die moeten stoppen
#define SEND_FITNESS 0x6 // MPI Message Fitness

#endif
