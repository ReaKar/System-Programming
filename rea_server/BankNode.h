/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BankNode.h
 * Author: ubundistas
 *
 * Created on May 15, 2016, 10:42 PM
 */

#ifndef BANKNODE_H
#define BANKNODE_H

#define NAME_SIZE 50

typedef struct BankNode {
    char name[NAME_SIZE];
    float amount;
    struct BankNode * next;

    struct TransactionNode * trans_ptr;

} BankNode;

typedef BankNode * BankNodePtr;

BankNodePtr BankNode_Create(char * name, float amount);
void BankNode_Destroy(BankNodePtr t);

#endif /* BANKNODE_H */

