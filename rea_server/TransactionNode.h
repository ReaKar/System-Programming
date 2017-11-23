/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TransactionNode.h
 * Author: ubundistas
 *
 * Created on May 15, 2016, 11:26 PM
 */

#ifndef TRANSACTIONNODE_H
#define TRANSACTIONNODE_H
#include"BankNode.h"

#define GET '+'
#define SENT '-' 

typedef struct TransactionNode {
    float amount;
    char state;
    struct TransactionNode * next;
   
    struct BankNode * banknode_ptr;    
} TransactionNode;

typedef TransactionNode * TransactionNodePtr;

TransactionNodePtr TransactionNode_Create(float amount, char state, BankNodePtr account);
void TransactionNode_Destroy(TransactionNodePtr t);


#endif /* TRANSACTIONNODE_H */

