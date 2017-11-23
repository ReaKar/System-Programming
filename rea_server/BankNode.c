/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include"BankNode.h"
#include"TransactionNode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BankNodePtr BankNode_Create(char * name, float amount) {
    BankNodePtr t = (BankNodePtr) malloc(sizeof (BankNode));
    strcpy(t->name, name);
    t->amount = amount;
    t->next = NULL;
    t->trans_ptr = NULL;

    return t;
}

void BankNode_Destroy(BankNodePtr t) {
    TransactionNodePtr prosorinos = NULL;
    TransactionNodePtr victim = NULL;

    prosorinos = t->trans_ptr;

    while (prosorinos != NULL) {
        victim = prosorinos;
        prosorinos = prosorinos->next;
        TransactionNode_Destroy(victim);
    }
    free(t);
}