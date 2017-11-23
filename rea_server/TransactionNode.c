/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "TransactionNode.h"

#include <stdio.h>
#include <stdlib.h>

TransactionNodePtr TransactionNode_Create(float amount, char state, BankNodePtr account) {
    TransactionNodePtr t = (TransactionNodePtr)malloc(sizeof(TransactionNode));
    t->amount = amount;
    t->state = state;
    t->banknode_ptr = account;
   
    t->next = NULL;
    return t;    
}



void TransactionNode_Destroy(TransactionNodePtr t) {
   free(t);
}