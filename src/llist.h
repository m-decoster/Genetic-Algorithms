//
//  llist.h
//  DA3-Project
//
//  Created by Mathieu De Coster
//  Copyright (c) 2014 Mathieu De Coster. All rights reserved.
//

#ifndef __DA3_Project__llist__
#define __DA3_Project__llist__

#include <stdlib.h>

typedef struct organism Organism;

typedef struct ll_node
{
    Organism* data;
    struct ll_node* next;
} llNode;

/**
 * Initializeer een gesorteerde gelinkte lijst
 * @param root (out) De node die het begin van de lijst moet worden
 * @param data De data die root moet bevatten
 */
void llInit(llNode* root, Organism* data);

/**
 * Free een volledige gelinkte lijst (data wordt niet gefreed!)
 * @param root Het begin van de lijst (wordt geen free() op toegepast!)
 */
void llFree(llNode* root);

/**
 * Voeg een organisme toe aan de gesorteerde lijst
 * @param root (in/out) Pointer naar het begin van de lijst
 * @param data Het organisme om toe te voegen
 */
void llAdd(llNode** root, Organism* data);

/**
 * Vraag een element op aan de hand van een index
 * @param root Het begin van de lijst
 * @param index De index waarop het element zit
 * @return Het element op plaats index, of het laatste in de lijst
 */
Organism* llGet(llNode* root, unsigned int i);

#endif /* defined(__DA3_Project__llist__) */
