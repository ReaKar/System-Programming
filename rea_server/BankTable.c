/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "BankTable.h"
#include "BankNode.h"
#include "TransactionNode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

int hashing(char * name, int size) {
    char ch[50];
    strcpy(ch, name);
    int length = strlen(name);
    int i, sum = 0;
    for (i = 0; i < length; i++) {
        sum += (int) ch[i];
    }
    return sum % size;
}

BankTablePtr BankTable_Create(int hashsize) {
    int i;
    BankTablePtr t = (BankTablePtr) malloc(sizeof (BankTable));
    t->hash_size = hashsize;
    t->hash_ptr = (BankNodePtr*) malloc(hashsize * sizeof (BankNodePtr));
    for (i = 0; i < hashsize; i++) {
        t->hash_ptr[i] = NULL;
    }

    t->mutexes_ptr = (pthread_mutex_t*) malloc(hashsize * sizeof (pthread_mutex_t));

    for (i = 0; i < hashsize; i++) {
        pthread_mutex_init(&t->mutexes_ptr[i], 0);
    }

    return t;
}

void BankTable_Destroy(BankTablePtr t) {
    int i;
    for (i = 0; i < t->hash_size; i++) {
        BankNodePtr prosorinos = NULL;
        BankNodePtr victim = NULL;

        prosorinos = t->hash_ptr[i];

        while (prosorinos != NULL) {
            victim = prosorinos;
            prosorinos = prosorinos->next;
            BankNode_Destroy(victim);
        }
    }

    for (i = 0; i < t->hash_size; i++) {
        pthread_mutex_destroy(&(t->mutexes_ptr[i]));
    }

    free(t->mutexes_ptr);
    free(t->hash_ptr);
    free(t);
}

int BankTable_Search_Account(BankTablePtr t, char *name) {

    BankNodePtr prosorinos;
    int position = hashing(name, t->hash_size);
    for (prosorinos = t->hash_ptr[position]; prosorinos != NULL; prosorinos = prosorinos->next) {
        if (strcmp(prosorinos->name, name) == 0) {
            return 1;
        }
    }
    return 0;
}

int BankTable_Add_Account(BankTablePtr t, float amount, char * name, float delay) {
    int position;
    int flag;
    BankNodePtr prosorinos;

    position = hashing(name, t->hash_size);
    printf("To position %d \n", position);

    sleep(delay);
    
    prosorinos = t->hash_ptr[position];

    flag = BankTable_Search_Account(t, name);
    if (flag == 0) {
        BankNodePtr account = BankNode_Create(name, amount);
        account->next = prosorinos;
        t->hash_ptr[position] = account;
        return 1;
    } else {
        return 0;
    }
}

void BankTable_Print(BankTablePtr t) {
    int i;

    TransactionNodePtr prosorini;

    for (i = 0; i < t->hash_size; i++) {
        BankNodePtr prosorinos = NULL;

        prosorinos = t->hash_ptr[i];

        while (prosorinos != NULL) {
            float balance = prosorinos->amount;

            balance = balance + BankTable_Total_In_Amount(t, prosorinos->name);
            balance = balance - BankTable_Total_Out_Amount(t, prosorinos->name);

            printf("vertex(%s) = %lf , %lf \n", prosorinos->name, prosorinos->amount, balance);

            for (prosorini = prosorinos->trans_ptr; prosorini != NULL; prosorini = prosorini->next) {
                if (prosorini->state == SENT) {
                    printf("(%s,%f) ", prosorini->banknode_ptr->name, prosorini->amount);
                }
            }

            prosorinos = prosorinos->next;
            printf("\n");
        }
    }
}

int BankTable_Add_Transaction(BankTablePtr t, float amount, char *name1, char *name2, float delay) {
    int position1 = hashing(name1, t->hash_size);
    int position2 = hashing(name2, t->hash_size);
    BankNodePtr logariasmos1;
    BankNodePtr logariasmos2;
    BankNodePtr prosorinos;
    TransactionNodePtr n1_to_n2;
    TransactionNodePtr n2_to_n1;






    sleep(delay);

    int flag1, flag2;
    flag1 = BankTable_Search_Account(t, name1);
    flag2 = BankTable_Search_Account(t, name2);

    if (flag1 == 1 && flag2 == 1) {

        for (prosorinos = t->hash_ptr[position1]; prosorinos != NULL; prosorinos = prosorinos->next) {
            if (strcmp(prosorinos->name, name1) == 0) {
                logariasmos1 = prosorinos;
            }
        }
        for (prosorinos = t->hash_ptr[position2]; prosorinos != NULL; prosorinos = prosorinos->next) {
            if (strcmp(prosorinos->name, name2) == 0) {
                logariasmos2 = prosorinos;
            }
        }
        float balance1 = logariasmos1->amount;
        float balance2 = logariasmos1->amount;

        balance1 = balance1 + BankTable_Total_In_Amount(t, logariasmos1->name);
        balance1 = balance1 - BankTable_Total_Out_Amount(t, logariasmos1->name);

        balance2 = balance2 + BankTable_Total_In_Amount(t, logariasmos2->name);
        balance2 = balance2 - BankTable_Total_Out_Amount(t, logariasmos2->name);

        if (balance1 > amount || balance1==amount) {
            n1_to_n2 = TransactionNode_Create(amount, SENT, logariasmos2);
            n2_to_n1 = TransactionNode_Create(amount, GET, logariasmos1);


            n1_to_n2->next = logariasmos1->trans_ptr;
            n2_to_n1->next = logariasmos2->trans_ptr;

            logariasmos1->trans_ptr = n1_to_n2;
            logariasmos2->trans_ptr = n2_to_n1;

            return 1;
        } else {
            printf("Error!To diathesimo ypoloipo tou logarismou einai %lf \n v", balance1);
            return 0;
        }
    } else {
        printf("Error!oi logariasmoi p dwsate den uparxoun\n");
        return 0;
    }




}

float BankTable_Total_In_Amount(BankTablePtr t, char *name1) {
    int position = hashing(name1, t->hash_size);
    BankNodePtr prosorinos;
    TransactionNodePtr prosorini;
    float poso = 0;

    for (prosorinos = t->hash_ptr[position]; prosorinos != NULL; prosorinos = prosorinos->next) {
        if (strcmp(prosorinos->name, name1) == 0) {
            for (prosorini = prosorinos->trans_ptr; prosorini != NULL; prosorini = prosorini->next) {
                if (prosorini->state == GET) {
                    poso += prosorini->amount;
                }
            }
        }
    }
    return poso;
}

float BankTable_Total_Out_Amount(BankTablePtr t, char *name1) {
    int position = hashing(name1, t->hash_size);
    BankNodePtr prosorinos;
    TransactionNodePtr prosorini;
    float poso = 0;

    for (prosorinos = t->hash_ptr[position]; prosorinos != NULL; prosorinos = prosorinos->next) {
        if (strcmp(prosorinos->name, name1) == 0) {
            for (prosorini = prosorinos->trans_ptr; prosorini != NULL; prosorini = prosorini->next) {
                if (prosorini->state == SENT) {
                    poso += prosorini->amount;
                }
            }
        }
    }
    return poso;
}

float BankTable_PrintBalance(BankTablePtr t, char *name1) {
    int i;

    for (i = 0; i < t->hash_size; i++) {
        BankNodePtr prosorinos = NULL;

        prosorinos = t->hash_ptr[i];

        while (prosorinos != NULL) {
            if (strcmp(prosorinos->name, name1) == 0) {
                float balance = prosorinos->amount;

                balance = balance + BankTable_Total_In_Amount(t, name1);
                balance = balance - BankTable_Total_Out_Amount(t, name1);

                printf("vertex(%s) = %lf \n", prosorinos->name, balance);

                printf("\n");
                return balance;
            }
            prosorinos = prosorinos->next;
        }
    }
    return 0;
}

// -------------------------

void BankTable_Lock_Account(BankTablePtr t, char *name1) {
    int position = hashing(name1, t->hash_size);
    pthread_mutex_lock(&t->mutexes_ptr[position]);
}

void BankTable_Unlock_Account(BankTablePtr t, char *name1) {
    int position = hashing(name1, t->hash_size);
    pthread_mutex_unlock(&t->mutexes_ptr[position]);
}

void BankTable_Lock_Accounts(BankTablePtr t, char *name1, char *name2) {
    int position1 = hashing(name1, t->hash_size);
    int position2 = hashing(name2, t->hash_size);


    if (position1 < position2) {
        pthread_mutex_lock(&t->mutexes_ptr[position1]);
        pthread_mutex_lock(&t->mutexes_ptr[position2]);
    }
    if (position2 == position1) {
        pthread_mutex_lock(&t->mutexes_ptr[position1]);
    }
    if (position1 > position2) {
        pthread_mutex_lock(&t->mutexes_ptr[position2]);
        pthread_mutex_lock(&t->mutexes_ptr[position1]);
    }
}

void BankTable_Unlock_Accounts(BankTablePtr t, char *name1, char *name2) {
    int position1 = hashing(name1, t->hash_size);
    int position2 = hashing(name2, t->hash_size);

    if (position1 < position2) {
        pthread_mutex_unlock(&t->mutexes_ptr[position1]);
        pthread_mutex_unlock(&t->mutexes_ptr[position2]);
    }
    if (position2 == position1) {
        pthread_mutex_unlock(&t->mutexes_ptr[position1]);
    }
    if (position1 > position2) {
        pthread_mutex_unlock(&t->mutexes_ptr[position2]);
        pthread_mutex_unlock(&t->mutexes_ptr[position1]);
    }
}

int * BankTable_Lock_Multiple_Accounts(BankTablePtr t, char ** names, int total_names, int *distinctnames) {
    int * index = malloc(total_names*sizeof(int));
    int i,j,k;

    for (i = 0; i < total_names; i++) {
        index[i] = hashing(names[i], t->hash_size);
    }

    for (i = 0; i < total_names; i++) { //afairesh diplotupwn
        for (j = i + 1; j < total_names;) {
            if (index[j] == index[i]) {
                for (k = j; k < total_names; k++) {
                    index[k] = index[k + 1];
                }
                total_names--;
            } else
                j++;
        }
    }

    for (i = (total_names - 1); i >= 0; i--) { //algoritmos taksinomisis
        for (j = 1; j <= i; j++) {
            if (index[j - 1] > index[j]) {
                int temp = index[j - 1];
                index[j - 1] = index[j];
                index[j] = temp;
            }
        }
    }
    
    for (i = 0; i < total_names; i++) {
        pthread_mutex_lock(&t->mutexes_ptr[index[i]]);
    }
    
    *distinctnames = total_names;
    
    return index;
}

void BankTable_Unlock_Multiple_Accounts(BankTablePtr t, int * index, int distinctnames) {
    int i;
    for (i = 0; i < distinctnames; i++) {
        pthread_mutex_unlock(&t->mutexes_ptr[index[i]]);
    }
    
    free(index);
}