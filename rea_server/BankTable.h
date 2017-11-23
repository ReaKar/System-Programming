/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BankTable.h
 * Author: ubundistas
 *
 * Created on May 15, 2016, 8:00 PM
 */

#ifndef BANKTABLE_H
#define BANKTABLE_H

#include <pthread.h>

typedef struct BankTable {
    struct BankNode ** hash_ptr;
    pthread_mutex_t * mutexes_ptr;
    int hash_size;
} BankTable;

typedef struct BankTable * BankTablePtr;

int hashing(char * name, int size);

BankTablePtr BankTable_Create(int hashsize);
void BankTable_Destroy(BankTablePtr t);
int BankTable_Search_Account(BankTablePtr t, char *name);
int BankTable_Add_Account(BankTablePtr t,float amount,char * name,float delay);
void BankTable_Print(BankTablePtr t);
int BankTable_Add_Transaction(BankTablePtr t,float amount, char *name1, char *name2,float delay);

float BankTable_Total_In_Amount(BankTablePtr t, char *name1);
float BankTable_Total_Out_Amount(BankTablePtr t, char *name1);
float BankTable_PrintBalance(BankTablePtr t, char *name1);


void BankTable_Lock_Account(BankTablePtr t, char *name1);
void BankTable_Unlock_Account(BankTablePtr t, char *name1);

void BankTable_Lock_Accounts(BankTablePtr t, char *name1, char *name2);
void BankTable_Unlock_Accounts(BankTablePtr t, char *name1, char *name2);

int * BankTable_Lock_Multiple_Accounts(BankTablePtr t, char ** names, int total_names, int *distinctnames);
void BankTable_Unlock_Multiple_Accounts(BankTablePtr t, int * index, int distinctnames);


#endif /* BANKTABLE_H */

