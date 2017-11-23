/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: ubundistas
 *
 * Created on May 15, 2016, 7:56 PM
 */
#include <stdio.h>
#include <sys/types.h>      /* sockets */
#include <sys/socket.h>      /* sockets */
#include <netinet/in.h>      /* internet sockets */
#include <netdb.h>          /* gethostbyaddr */
#include <unistd.h>          /* fork */  
#include <stdlib.h>          /* exit */
#include <ctype.h>          /* toupper */
#include<string.h>
#include <stdlib.h>
#include"BankNode.h"
#include"BankTable.h"
#include"TransactionNode.h"
#include "ProducerConsumer.h"
#include <pthread.h>

BankTablePtr t = NULL;
ProducerConsumerQueuePtr p = NULL;

char *a1 = "add_account";
char *b2 = "add_transfer";
char *c3 = "add_multi_transfer";
char *d4 = "print_balance";
char *e5 = "print_multi_balance";
char *f6 = "print";
char *g7 = "exit";
char *aa = "aa";
char *at = "at";
char *am = "am";
char *pb = "pb";
char *mb = "mb";

char *OK = "OK";
char *ER = "ER";
char *ERROR = "Error.  Unknown command";

void perror_exit(char *message);

void * main_worker_thread(void * s) {
    int newsock;
    int flag;
    int counter = 0;
    int counter2 = 0;
    int i;
    float amount;
    float delay;
    float f;
    int size_name;
    int size_name2;
    char type[2];
    char name[50];
    char name2[50];
    int distinctnames;
    int * index;

    while (1) { // gia kathe pelati ...
        printf("Worker wait ... \n");
        newsock = ProducerConsumerQueue_remove(p); //afairw apo thn oura
        printf("Worker woke ... newsock = %d \n", newsock);


        while (read(newsock, type, 2) == 2) { // gia kathe eentoli...
            if (strncmp(type, aa, 2) == 0 || strncmp(type, at, 2) == 0 || strncmp(type, am, 2) == 0 || strncmp(type, pb, 2) == 0 || strncmp(type, mb, 2) == 0) {
                if (strncmp(type, aa, 2) == 0) { //add_account
                    read(newsock, &amount, sizeof (amount));
                    read(newsock, &size_name, sizeof (size_name));
                    read(newsock, name, size_name);
                    read(newsock, &delay, sizeof (delay));

                    name[size_name] = '\0';

                    delay = delay / 1000;

                    printf("name= %s \n", name);

                    BankTable_Lock_Account(t, name);
                    flag = BankTable_Add_Account(t, amount, name, delay);
                    size_name = strlen(name);
                    BankTable_Unlock_Account(t, name);

                    if (flag == 1) {
                        write(newsock, OK, 2); //vazoume sto pipe to ok wste na t lavei o client
                        write(newsock, &size_name, sizeof (size_name));
                        write(newsock, name, size_name);
                        write(newsock, &amount, sizeof (amount));
                    } else {
                        write(newsock, ER, 2); //omoiws me t er
                        write(newsock, &size_name, sizeof (size_name));
                        write(newsock, name, size_name);
                        write(newsock, &amount, sizeof (amount));
                    }
                }

                if (strncmp(type, at, 2) == 0) { // add_transfer
                    read(newsock, &amount, sizeof (amount));
                    read(newsock, &size_name, sizeof (size_name));
                    read(newsock, name, size_name);
                    read(newsock, &size_name2, sizeof (size_name2));
                    read(newsock, name2, size_name2);
                    read(newsock, &delay, sizeof (delay));

                    name[size_name] = '\0';
                    name2[size_name2] = '\0';
                    printf("name= %s \n", name);
                    printf("name= %s \n", name2);
                    delay = delay / 1000;

                    BankTable_Lock_Accounts(t, name, name2);
                    flag = BankTable_Add_Transaction(t, amount, name, name2, delay);
                    BankTable_Unlock_Accounts(t, name, name2);

                    size_name = strlen(name);
                    size_name2 = strlen(name2);

                    if (flag == 1) {
                        write(newsock, OK, 2); //vazoume sto pipe to ok wste na t lavei o client
                        write(newsock, &size_name, sizeof (size_name));
                        write(newsock, name, size_name);
                        write(newsock, &size_name2, sizeof (size_name2));
                        write(newsock, name2, size_name2);
                        write(newsock, &amount, sizeof (amount));
                        write(newsock, &delay, sizeof (delay));
                    } else {
                        write(newsock, ER, 2);
                        write(newsock, &size_name, sizeof (size_name));
                        write(newsock, name, size_name);
                        write(newsock, &size_name2, sizeof (size_name2));
                        write(newsock, name2, size_name2);
                        write(newsock, &amount, sizeof (amount));
                        write(newsock, &delay, sizeof (delay));
                    }
                }

                if (strncmp(type, am, 2) == 0) { //add_multi_transfer
                    read(newsock, &amount, sizeof (amount));
                    read(newsock, &size_name, sizeof (size_name));
                    read(newsock, name, size_name);
                    name[size_name] = '\0';

                    read(newsock, &counter, sizeof (counter)); // counter: arithmos logariasmwn proorismou
                    read(newsock, &delay, sizeof (delay));
                    counter2 = counter + 1; // counter2: sunolikos arithmos logariasmwn (source + destinations)

                    char ** names = malloc(counter2 * sizeof (char*)); //??????
                    for (i = 0; i <= counter; i++) {
                        names[i] = malloc(50);
                    }

                    delay = delay / 1000;
                    printf("name= %s \n", name);

                    for (i = 0; i < counter; i++) {
                        read(newsock, &size_name2, sizeof (size_name2));
                        read(newsock, name2, size_name2);
                        name2[size_name2] = '\0';
                        strcpy(names[i], name2);
                        printf("To name2 einai %s \n",names[i]);
                    }
                    strcpy(names[counter], name); //vazw sthn teleutaia thesh to logariasmo source
                   // printf("to name einai %s",name[counter]);
                    
                    index = BankTable_Lock_Multiple_Accounts(t, names, counter2, &distinctnames); //??
                 //to distinct names dn tha eprepe n nai deikths???/
                    int exist = 0;

                    if (counter * amount <= BankTable_PrintBalance(t, name)) {
                        flag = 1; // to upoloipo xrhmatwn ftanei na dwsw se olous ts logariasmous proorismou

                        for (i = 0; i <= counter; i++) {
                            
                            exist = exist + BankTable_Search_Account(t, names[i]);
                        }
                    } else {
                        flag = 0;
                    }

                    if (exist == counter2 && flag == 1) { //an oloi oi logariasmoi uparxoun k exw arketo upoloipo

                        for (i = 0; i < counter; i++) {
                            BankTable_Add_Transaction(t, amount, name, names[i], delay);
                        }
                        BankTable_Unlock_Multiple_Accounts(t, index, distinctnames);

                        size_name = strlen(name);

                        write(newsock, OK, 2); //vazoume sto pipe to ok wste na t lavei o client
                        write(newsock, &size_name, sizeof (size_name));
                        write(newsock, name, size_name);
                        write(newsock, &amount, sizeof (amount));
                        write(newsock, &delay, sizeof (delay));
                    } else {
                        BankTable_Unlock_Multiple_Accounts(t, index, distinctnames);
                        
                        write(newsock, ER, 2); 
                        write(newsock, &size_name, sizeof (size_name));
                        write(newsock, name, size_name);
                        write(newsock, &amount, sizeof (amount));
                        write(newsock, &delay, sizeof (delay));
                    }
                    
                }

                if (strncmp(type, pb, 2) == 0) { // print_balance
                    read(newsock, &size_name, sizeof (size_name));
                    read(newsock, name, size_name);
                    name[size_name] = '\0';

                    BankTable_Lock_Account(t, name);
                    f = BankTable_PrintBalance(t, name);
                    BankTable_Unlock_Account(t, name);

                    size_name = strlen(name);
                    if (f > 0 || f==0) { //akoma k an t upoloipo einai 0 stelnw ok
                        write(newsock, OK, 2); //vazoume sto pipe to ok wste na t lavei o client
                        write(newsock, &size_name, sizeof (size_name));
                        write(newsock, name, size_name);
                        write(newsock, &f, sizeof (f));
                    } else {
                        write(newsock, ER, 2); 
                        write(newsock, &size_name, sizeof (size_name));
                        write(newsock, name, size_name);
                        write(newsock, &amount, sizeof (amount));
                    }
                }

                if (strncmp(type, mb, 2) == 0) { // multi_balance
                    read(newsock, &counter, sizeof (counter));
                    char ** names = malloc(counter * sizeof (char *)); 
                    for (i = 0; i < counter; i++) {
                        names[i] = malloc(50);
                    }

                    // read all names
                    for (i = 0; i < counter; i++) {
                        read(newsock, &size_name, sizeof (size_name));
                        read(newsock, name, size_name);
                        name[size_name] = '\0';
                        strcpy(names[i], name);
                    }

                    // lock all accounts ...
                    index = BankTable_Lock_Multiple_Accounts(t, names, counter, &distinctnames);

                    int exist = 0;
                    for (i = 0; i < counter; i++) {
                        exist = exist + BankTable_Search_Account(t, names[i]);
                    }

                    if (exist == counter) {
                        write(newsock, OK, 2); //vazoume sto pipe to ok wste na t lavei o client
                        for (i = 0; i < counter; i++) {
                            f = BankTable_PrintBalance(t, names[i]);
                            size_name = strlen(names[i]);
                            write(newsock, &size_name, sizeof (size_name));
                            write(newsock, names[i], size_name);
                            write(newsock, &f, sizeof (f));
                        }
                    } else {
                        write(newsock, ER, 2); 

                        for (i = 0; i < counter; i++) {
                            size_name = strlen(names[i]);
                            write(newsock, &size_name, sizeof (size_name));
                            write(newsock, names[i], size_name);
                        }
                    }

                    // unlock all accounts ...
                    BankTable_Unlock_Multiple_Accounts(t, index, distinctnames);
                }
            } else {
                //an h entolh den einai mia apo autes p prepei stelnnw ERROR k string
                size_name = strlen(ERROR);
                write(newsock, &size_name, sizeof (size_name));
                write(newsock, ERROR, size_name);
            }
        }

        close(newsock);
    }

    return NULL;
}

void * main_master_thread(void * s) {
    int sock;
    struct sockaddr_in server, client;
    socklen_t clientlen = sizeof(client);
    struct sockaddr *serverptr = (struct sockaddr *) &server;
    struct sockaddr *clientptr = (struct sockaddr *) &client;
    int port = *((int*) s);
    int newsock;

    /* Create socket */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror_exit("socket");

    server.sin_family = AF_INET; /* Internet domain */
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port); /* The given port */

  

    /* Bind socket to address */
    if (bind(sock, serverptr, sizeof (server)) < 0)
        perror_exit("bind");

    /* Listen for connections */
    if (listen(sock, 40) < 0) {
        perror_exit("listen");
    }

    printf("Master: Listening for connections to port %d\n", port);

    while (1) {
        printf("Master: Wait ... \n");
        if ((newsock = accept(sock, clientptr, &clientlen)) < 0) { //dexetai sundesh alla k pelath?
            perror_exit("accept");
        }

        printf("Master: Accepted connection\n");
        printf("Master: Insert to queue: %d  \n", newsock);

        ProducerConsumerQueue_insert(p, newsock); //eisagw sthn oura molis mpei sthn oura
    }                                             //tote uparxei kati n vgalei t worker thread k ksupnaei

}

int main(int argc, char** argv) {
    int i;
    
    int port; 
    int total_worker_threads;
    int maxsize ;
    pthread_t master_tid;
    pthread_t * worker_tid;

    int HashTableEntries = 10;
    maxsize=atoi(argv[6]);  //magethos ouras

    t = BankTable_Create(HashTableEntries); 
    p = ProducerConsumerQueue_Create(maxsize);

    if (argc != 7) {
        printf("Please  give  port  number ,thread pool size and queue size\n");
        exit(1);
    }

    port = atoi(argv[2]);
    total_worker_threads = atoi(argv[4]);

    worker_tid = malloc(total_worker_threads * sizeof (pthread_t)); 

    pthread_create(&master_tid, NULL, main_master_thread, &port); //edw to port einai int kanei?? dn ithele deikth??

    for (i = 0; i < total_worker_threads; i++) {
        pthread_create(&worker_tid[i], NULL, main_worker_thread, NULL);
    }

  

    

    printf("Server is waiting for master thread to terminate ... \n");
    pthread_join(master_tid, NULL);

    for (i = 0; i < total_worker_threads; i++) {
        pthread_join(worker_tid[i], NULL);
    }



    return (EXIT_SUCCESS);
}

void perror_exit(char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}


