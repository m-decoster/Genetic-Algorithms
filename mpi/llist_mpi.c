//
//  llist_mpi.c
//  DA3-Project
//
//  Created by Mathieu De Coster on 23/10/14.
//  Copyright (c) 2014 Mathieu De Coster. All rights reserved.
//

#include "llist_mpi.h"
#include "genetic_mpi.h"
#include "memory_mpi.h"
#include <assert.h>
#include <stdlib.h>

void llInit(llNode* root, Organism* data)
{
    root->next = NULL;
    root->data = data;
}

void llFree(llNode* root)
{
    llNode* cur = root;
    while(cur != NULL)
    {
        llNode* temp = cur;
        cur = cur->next;
        free(temp);
    }
}

void llAdd(llNode** root, Organism* data)
{
    // Zoek een goed plekje zodat de lijst gesorteerd blijft
    llNode* cur = *root;
    llNode* prev = NULL;
    while(cur != NULL)
    {
        if(!cur->data)
        {
            cur->data = data;
            return;
        }
        
        if(data->fitness > cur->data->fitness)
        {
            prev = cur;
            cur = cur->next;
        }
        else
        {
            // We willen hiertussen
            if(!prev)
            {
                assert(cur == *root); // Zou altijd moeten kloppen in dit geval
                llNode* newRoot;
                CMALLOC(newRoot, sizeof(llNode), 1);
                newRoot->data = data;
                newRoot->next = cur;
                *root = newRoot;
                return;
            }
            else
            {
                llNode* newNode;
                CMALLOC(newNode, sizeof(llNode), 1);
                newNode->data = data;
                newNode->next = cur;
                prev->next = newNode;
                return;
            }
        }
    }

    if(!cur)
    {
        // Einde van de lijst is bereikt
        assert(prev != NULL); // Sanity check, dit zou niet mogen gebeuren
        llNode* newNode;
        CMALLOC(newNode, sizeof(llNode), 1);
        newNode->data = data;
        newNode->next = NULL;
        prev->next = newNode;
    }
}

Organism* llGet(llNode* root, unsigned int i)
{
    llNode* cur = root;
    unsigned int index = 0;
    while(cur)
    {
        if(index == i)
        {
            return cur->data;
        }
        
        ++index;
        if(cur->next != NULL)
        {
            cur = cur->next;
        }
        else
        {
            // Als we aan het einde van de lijst zijn gekomen, geef
            // dan het laatste element terug
            return cur->data;
        }
    }
    
    fprintf(stderr, "Let op: llGet voorbij while-loop!");
    return NULL; // Zou niet mogen gebeuren
}
