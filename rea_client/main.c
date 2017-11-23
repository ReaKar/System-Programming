/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: ubundistas
 *
 * Created on May 16, 2016, 2:49 AM
 */

#include  <stdio.h>
#include  <sys/types.h>
#include  <sys/socket.h>
#include  <netinet/in.h>
#include  <unistd.h>

#include  <netdb.h>
#include  <stdlib.h>
#include  <string.h>

char *a1 = "add_account";
char *b2 = "add_transfer";
char *c3 = "add_multi_transfer";
char *d4 = "print_balance";
char *e5 = "print_multi_balance";
char *f6 = "sleep";
char *g7 = "exit";
char *aa = "aa";
char *at = "at";
char *am = "am";
char *pb = "pb";
char *mb = "mb";

char *OK = "OK";
char *ER = "ER";

void perror_exit(char *message);

int main(int argc, char** argv) {
    int sock;
    int port;
    FILE * file_ptr;
    char bStr[1000];
    char *file[50];
    char name[50];
    char *ptr;
    char *bla;
    int j, i;
    float amount;
    int size_name;
    int size_name2;
    char name2[50];
    float delay;

    char recv[3];
    recv[2] = '\0';
    int counter = 0;

    struct hostent *rem;
    struct sockaddr_in server;
    struct sockaddr *serverptr = (struct sockaddr *) &server;

    if (argc != 7) {
        printf("Please  give  host  name  and  port  number and file\n");
        exit(1);
    } else {
        file_ptr = fopen(argv[6], "r");

        if ((file_ptr = fopen(argv[6], "r")) == NULL) {
            printf("Error! opening file \n");
            return 0;
        }
        //
        /*  Create  socket  */
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            perror_exit("socket");

        /*  Find  server  address  */
        if ((rem = gethostbyname(argv[2])) == NULL) {
            herror("gethostbyname");
            exit(1);
        }

        port = atoi(argv[4]);

        /* Convert  port  number  to  integer */
        server.sin_family = AF_INET;
        /*  Internet  domain  */
        memcpy(& server.sin_addr, rem ->h_addr, rem ->h_length);
        server.sin_port = htons(port);

        /*  Server  port  */
        /*  Initiate  connection  */
        if (connect(sock, serverptr, sizeof (server)) < 0)
            perror_exit("connect");

        printf("Connecting  to %s port %d\n", argv[4], port);

        printf("dwste tis entoles  :  \n");

        //        file_ptr = stdin;

        while (fgets(bStr, 1000, file_ptr)) {
            j = 0;

            ptr = strtok(bStr, " \n");
            while (ptr != NULL) {
                file[j] = ptr;

                printf("%s\n", file[j]);

                j++;

                ptr = strtok(NULL, " \n");

            }

            if (j == 0) {
                continue;
            }
            if (strcmp(file[0], a1) == 0 || strcmp(file[0], b2) == 0 || strcmp(file[0], c3) == 0 || strcmp(file[0], d4) == 0 || strcmp(file[0], e5) == 0 || strcmp(file[0], f6) == 0 || strcmp(file[0], g7) == 0) {
                if (strcmp(file[0], a1) == 0) { //add_account
                    if (j != 4 && j != 3) {
                        printf("Error!Dwste sthn add_account ena arxiko poso ena onoma kai proairetika ena delay\n");
                    } else {
                        if (j == 4) { //an o xrhsths exei dwsei delay
                            strcpy(name, file[2]);
                            size_name = strlen(name);
                            amount = atof(file[1]);
                            delay = atof(file[3]);

                            write(sock, aa, 2);
                            write(sock, &amount, sizeof (amount));
                            write(sock, &size_name, sizeof (size_name));
                            write(sock, name, size_name);

                            write(sock, &delay, sizeof (delay));

                            read(sock, recv, 2);
                            read(sock, &size_name, sizeof (size_name));
                            read(sock, name, size_name);
                            read(sock, &amount, sizeof (amount));

                            name[size_name] = '\0';

                            if (strncmp(recv, OK, 2) == 0) {
                                printf("Success.  Account creation (%s:%lf) \n", name, amount);
                            } else {
                                printf("Error.  Account creation failed (%s:%lf) \n", name, amount);
                            }

                        } else { //an o xrhsths den exei dwsei delay
                            strcpy(name, file[2]);
                            size_name = strlen(name);
                            amount = atof(file[1]);
                            delay = 0;

                            write(sock, aa, 2);
                            write(sock, &amount, sizeof (amount));
                            write(sock, &size_name, sizeof (size_name));
                            write(sock, name, size_name);

                            write(sock, &delay, sizeof (delay));

                            read(sock, recv, 2);
                            read(sock, &size_name, sizeof (size_name));
                            read(sock, name, size_name);
                            read(sock, &amount, sizeof (amount));

                            name[size_name] = '\0';

                            if (strncmp(recv, OK, 2) == 0) {
                                printf("Success.  Account creation (%s:%lf) \n", name, amount);
                            } else {
                                printf("Error.  Account creation failed (%s:%lf) \n", name, amount);
                            }
                        }
                    }
                }

                if (strcmp(file[0], b2) == 0) { //add_transfer
                    if (j != 5 && j != 4) {
                        printf("Error!Dwste sthn add_transfer ena poso kai duo onomata logariasmwn\n");
                    } else {
                        if (j == 5) { //an o xrhsths exei dwsei dalay
                            amount = atof(file[1]);
                            strcpy(name, file[2]);
                            size_name = strlen(name);

                            strcpy(name2, file[3]);
                            size_name2 = strlen(name2);

                            delay = atof(file[4]);

                            write(sock, at, 2);
                            write(sock, &amount, sizeof (amount));
                            write(sock, &size_name, sizeof (size_name));
                            write(sock, name, size_name);
                            write(sock, &size_name2, sizeof (size_name2));
                            write(sock, name2, size_name2);

                            write(sock, &delay, sizeof (delay));

                            read(sock, recv, 2);

                            read(sock, &size_name, sizeof (size_name));
                            read(sock, name, size_name);
                            read(sock, &size_name2, sizeof (size_name2));
                            read(sock, name2, size_name2);
                            read(sock, &amount, sizeof (amount));
                            read(sock, &delay, sizeof (delay));

                            name[size_name] = '\0';
                            name2[size_name2] = '\0';

                            if (strncmp(recv, OK, 2) == 0) {
                                printf("Success.  Transfer addition (%s:%s:%lf:%lf) \n", name, name2, amount, delay);
                            } else {
                                printf("Error.  Transfer addition failed (%s:%s:%lf:%lf) \n", name, name2, amount, delay);
                            }

                        } else { //an o xrhsths den exei dwsei delay
                            amount = atof(file[1]);
                            strcpy(name, file[2]);
                            size_name = strlen(name);

                            strcpy(name2, file[3]);
                            size_name2 = strlen(name2);
                            delay = 0;

                            write(sock, at, 2);
                            write(sock, &amount, sizeof (amount));
                            write(sock, &size_name, sizeof (size_name));
                            write(sock, name, size_name);
                            write(sock, &size_name2, sizeof (size_name2));
                            write(sock, name2, size_name2);

                            write(sock, &delay, sizeof (delay));

                            read(sock, recv, 2);

                            read(sock, &size_name, sizeof (size_name));
                            read(sock, name, size_name);
                            read(sock, &size_name2, sizeof (size_name2));
                            read(sock, name2, size_name2);
                            read(sock, &amount, sizeof (amount));
                            read(sock, &delay, sizeof (delay));


                            name[size_name] = '\0';
                            name2[size_name2] = '\0';

                            if (strncmp(recv, OK, 2) == 0) {
                                printf("Success.  Transfer addition (%s:%s:%lf:%lf) \n", name, name2, amount, delay);
                            } else {
                                printf("Error.  Transfer addition failed (%s:%s:%lf:%lf) \n", name, name2, amount, delay);
                            }
                        }
                    }
                }

                if (strcmp(file[0], c3) == 0) { //add_multitransfer
                    if (j < 4) {
                        printf("Error!Dwste sthn addmultitransfer ena poso kai toulaxiston duo onomata logarismwn\n");
                    } else {
                        if (atof(file[j - 1]) > 0) { // an o xrhsths dwsei delay 
                            amount = atof(file[1]);
                            strcpy(name, file[2]);
                            size_name = strlen(name);

                            write(sock, am, 2);
                            write(sock, &amount, sizeof (amount));
                            write(sock, &size_name, sizeof (size_name));
                            write(sock, name, size_name);

                            counter = j - 4;
                            write(sock, &counter, sizeof (counter));
                            delay = atof(file[j - 1]);
                            write(sock, &delay, sizeof (delay));

                            for (i = 3; i <= j - 2; i++) {
                                strcpy(name2, file[i]);
                                size_name2 = strlen(name2);
                                write(sock, &size_name2, sizeof (size_name2));
                                write(sock, name2, size_name2);
                            }


                            read(sock, recv, 2); 
                            read(sock, &size_name, sizeof (size_name));
                            read(sock, name, size_name);
                            read(sock, &amount, sizeof (amount));
                            read(sock, &delay, sizeof (delay));

                            name[size_name] = '\0';


                            if (strncmp(recv, OK, 2) == 0) {
                                printf("Success.  Multi-Transfer addition (%s:%lf:%lf) \n", name, amount, delay);
                            } else {
                                printf("Error.  Multi-Transfer addition failed (%s:%lf:%lf) \n", name, amount, delay);
                            }
                        } else { //an o xrhsths den dwsei delay 
                            amount = atof(file[1]);
                            strcpy(name, file[2]);
                            size_name = strlen(name);
                            write(sock, am, 2);
                            write(sock, &amount, sizeof (amount));
                            write(sock, &size_name, sizeof (size_name));
                            write(sock, name, size_name);
                            counter = j - 3;
                            write(sock, &counter, sizeof (counter));
                            delay = 0;
                            write(sock, &delay, sizeof (delay));

                            for (i = 3; i <= j - 1; i++) {
                                strcpy(name2, file[i]);
                                size_name2 = strlen(name2);
                                write(sock, &size_name2, sizeof (size_name2));
                                write(sock, name2, size_name2);
                            }



                            read(sock, recv, 2); 
                            read(sock, &size_name, sizeof (size_name));
                            read(sock, name, size_name);
                            read(sock, &amount, sizeof (amount));
                            read(sock, &delay, sizeof (delay));

                            name[size_name] = '\0';


                            if (strncmp(recv, OK, 2) == 0) {
                                printf("Success.  Multi-Transfer addition (%s:%lf:%lf) \n", name, amount, delay);
                            } else {
                                printf("Error.  Multi-Transfer addition failed (%s:%lf:%lf) \n", name, amount, delay);
                            }

                        }



                    }
                }

                if (strcmp(file[0], d4) == 0) { //printbalance
                    if (j != 2) {
                        printf("Error!Dwste sthn printbalance ena onoma logariasmou\n");
                    } else {
                        strcpy(name, file[1]);
                        size_name = strlen(name);

                        write(sock, pb, 2);
                        write(sock, &size_name, sizeof (size_name));
                        write(sock, name, size_name);

                        read(sock, recv, 2); 
                        read(sock, &size_name, sizeof (size_name));
                        read(sock, name, size_name);
                        read(sock, &amount, sizeof (amount));

                        name[size_name] = '\0';


                        if (strncmp(recv, OK, 2) == 0) {
                            printf("Success.  Balance (%s:%lf) \n", name, amount);
                        } else {
                            printf("Error. Balance (%s) \n", name);
                        }



                    }
                }
                if (strcmp(file[0], e5) == 0) { //printmultibalance
                    if (j < 3) {
                        printf("Error!Dwste sthn printmultibalance toulaxiston  onomata logariasmwn \n");
                    } else {
                        write(sock, mb, 2);
                        counter = j - 1;
                        write(sock, &counter, sizeof (counter));

                        for (i = 1; i <= j - 1; i++) {
                            strcpy(name, file[i]);
                            size_name = strlen(name);
                            write(sock, &size_name, sizeof (size_name));
                            write(sock, name, size_name);
                        }

                        read(sock, recv, 2);

                     if (strncmp(recv, OK, 2) == 0) {
                            printf("Success. Multi-Balance (");
                            for (i = 1; i <= j - 1; i++) {
                                read(sock, &size_name, sizeof (size_name));
                                read(sock, name, size_name);
                                name[size_name] = '\0';
                                read(sock, &amount, sizeof (amount));
                                printf(" %s:%lf  ", name, amount);
                            }
                            printf(") \n");
                        } else {
                            printf("Error. Multi- Balance (");
                            for (i = 1; i <= j - 1; i++) {
                                read(sock, &size_name, sizeof (size_name));
                                read(sock, name, size_name);
                                name[size_name] = '\0';
                                printf(" %s : ", name);
                            }
                            printf(") \n", name);
                        }
                    }
                }
                if (strcmp(file[0], f6) == 0) { //sleep
                    if (j != 2) {
                        printf("Error!Dwste sthn sleep ena time\n");
                    } else {
                        delay = atof(file[1]);
                        delay = delay / 1000;
                        printf("please be waiting %lf s\n", delay);

                        sleep(delay);

                    }
                }
                if (strcmp(file[0], g7) == 0) { //exit
                    if (j != 1) {
                        printf("Error!\n");
                    } else {
                        close(sock);
                        return 0;
                    }
                }
            } else {
                strcpy(name, file[0]);
                write(sock, name, 2);
                read(sock, &size_name, sizeof (size_name));
                read(sock, name, size_name);
                name[size_name] = '\0';
                printf("%s \n", name);

            }

        }
        fclose(file_ptr); 

        while (1) {
            j = 0;


            printf("dwste tis entoles  :  ");
            gets(bStr);

            bla = strtok(bStr, " \n");
            while (bla != NULL) {
                file[j] = bla;

                printf("%s\n", file[j]);

                j++;

                bla = strtok(NULL, " \n");
            }
            if (j == 0) {
                continue;
            }
            if (strcmp(file[0], a1) == 0 || strcmp(file[0], b2) == 0 || strcmp(file[0], c3) == 0 || strcmp(file[0], d4) == 0 || strcmp(file[0], e5) == 0 || strcmp(file[0], f6) == 0 || strcmp(file[0], g7) == 0) {
                if (strcmp(file[0], a1) == 0) { //add_account
                    if (j != 4 && j != 3) {
                        printf("Error!Dwste sthn add_account ena arxiko poso ena onoma kai proairetika ena delay\n");
                    } else {
                        if (j == 4) { //an o xrhsths exei dwsei delay
                            strcpy(name, file[2]);
                            size_name = strlen(name);
                            amount = atof(file[1]);
                            delay = atof(file[3]);

                            write(sock, aa, 2);
                            write(sock, &amount, sizeof (amount));
                            write(sock, &size_name, sizeof (size_name));
                            write(sock, name, size_name);

                            write(sock, &delay, sizeof (delay));

                            read(sock, recv, 2);
                            read(sock, &size_name, sizeof (size_name));
                            read(sock, name, size_name);
                            read(sock, &amount, sizeof (amount));

                            name[size_name] = '\0';

                            if (strncmp(recv, OK, 2) == 0) {
                                printf("Success.  Account creation (%s:%lf) \n", name, amount);
                            } else {
                                printf("Error.  Account creation failed (%s:%lf) \n", name, amount);
                            }

                        } else { //an o xrhsths den exei dwsei delay
                            strcpy(name, file[2]);
                            size_name = strlen(name);
                            amount = atof(file[1]);
                            delay = 0;

                            write(sock, aa, 2);
                            write(sock, &amount, sizeof (amount));
                            write(sock, &size_name, sizeof (size_name));
                            write(sock, name, size_name);

                            write(sock, &delay, sizeof (delay));

                            read(sock, recv, 2);
                            read(sock, &size_name, sizeof (size_name));
                            read(sock, name, size_name);
                            read(sock, &amount, sizeof (amount));

                            name[size_name] = '\0';

                            if (strncmp(recv, OK, 2) == 0) {
                                printf("Success.  Account creation (%s:%lf) \n", name, amount);
                            } else {
                                printf("Error.  Account creation failed (%s:%lf) \n", name, amount);
                            }
                        }
                    }
                }

                if (strcmp(file[0], b2) == 0) { //add_transfer
                    if (j != 5 && j != 4) {
                        printf("Error!Dwste sthn add_transfer ena poso kai duo onomata logariasmwn\n");
                    } else {
                        if (j == 5) { //an o xrhsths exei dwsei dalay
                            amount = atof(file[1]);
                            strcpy(name, file[2]);
                            size_name = strlen(name);

                            strcpy(name2, file[3]);
                            size_name2 = strlen(name2);

                            delay = atof(file[4]);

                            write(sock, at, 2);
                            write(sock, &amount, sizeof (amount));
                            write(sock, &size_name, sizeof (size_name));
                            write(sock, name, size_name);
                            write(sock, &size_name2, sizeof (size_name2));
                            write(sock, name2, size_name2);

                            write(sock, &delay, sizeof (delay));

                            read(sock, recv, 2);

                            read(sock, &size_name, sizeof (size_name));
                            read(sock, name, size_name);
                            read(sock, &size_name2, sizeof (size_name2));
                            read(sock, name2, size_name2);
                            read(sock, &amount, sizeof (amount));
                            read(sock, &delay, sizeof (delay));

                            name[size_name] = '\0';
                            name2[size_name2] = '\0';

                            if (strncmp(recv, OK, 2) == 0) {
                                printf("Success.  Transfer addition (%s:%s:%lf:%lf) \n", name, name2, amount, delay);
                            } else {
                                printf("Error.  Transfer addition failed (%s:%s:%lf:%lf) \n", name, name2, amount, delay);
                            }

                        } else { //an o xrhsths den exei dwsei delay
                            amount = atof(file[1]);
                            strcpy(name, file[2]);
                            size_name = strlen(name);

                            strcpy(name2, file[3]);
                            size_name2 = strlen(name2);
                            delay = 0;

                            write(sock, at, 2);
                            write(sock, &amount, sizeof (amount));
                            write(sock, &size_name, sizeof (size_name));
                            write(sock, name, size_name);
                            write(sock, &size_name2, sizeof (size_name2));
                            write(sock, name2, size_name2);

                            write(sock, &delay, sizeof (delay));

                            read(sock, recv, 2);

                            read(sock, &size_name, sizeof (size_name));
                            read(sock, name, size_name);
                            read(sock, &size_name2, sizeof (size_name2));
                            read(sock, name2, size_name2);
                            read(sock, &amount, sizeof (amount));
                            read(sock, &delay, sizeof (delay));


                            name[size_name] = '\0';
                            name2[size_name2] = '\0';

                            if (strncmp(recv, OK, 2) == 0) {
                                printf("Success.  Transfer addition (%s:%s:%lf:%lf) \n", name, name2, amount, delay);
                            } else {
                                printf("Error.  Transfer addition failed (%s:%s:%lf:%lf) \n", name, name2, amount, delay);
                            }
                        }
                    }
                }

                if (strcmp(file[0], c3) == 0) { //add_multitransfer
                    if (j < 4) {
                        printf("Error!Dwste sthn addmultitransfer ena poso kai toulaxiston duo onomata logarismwn\n");
                    } else {
                        if (atof(file[j - 1]) > 0) { // an o xrhsths dwsei delay 
                            amount = atof(file[1]);
                            strcpy(name, file[2]);
                            size_name = strlen(name);

                            write(sock, am, 2);
                            write(sock, &amount, sizeof (amount));
                            write(sock, &size_name, sizeof (size_name));
                            write(sock, name, size_name);

                            counter = j - 4;
                            write(sock, &counter, sizeof (counter));
                            delay = atof(file[j - 1]);
                            write(sock, &delay, sizeof (delay));

                            for (i = 3; i <= j - 2; i++) {
                                strcpy(name2, file[i]);
                                size_name2 = strlen(name2);
                                write(sock, &size_name2, sizeof (size_name2));
                                write(sock, name2, size_name2);
                            }


                            read(sock, recv, 2); 
                            read(sock, &size_name, sizeof (size_name));
                            read(sock, name, size_name);
                            read(sock, &amount, sizeof (amount));
                            read(sock, &delay, sizeof (delay));

                            name[size_name] = '\0';


                            if (strncmp(recv, OK, 2) == 0) {
                                printf("Success.  Multi-Transfer addition (%s:%lf:%lf) \n", name, amount, delay);
                            } else {
                                printf("Error.  Multi-Transfer addition failed (%s:%lf:%lf) \n", name, amount, delay);
                            }
                        } else { //an o xrhsths den dwsei delay 
                            amount = atof(file[1]);
                            strcpy(name, file[2]);
                            size_name = strlen(name);
                            write(sock, am, 2);
                            write(sock, &amount, sizeof (amount));
                            write(sock, &size_name, sizeof (size_name));
                            write(sock, name, size_name);
                            counter = j - 3;
                            write(sock, &counter, sizeof (counter));
                            delay = 0;
                            write(sock, &delay, sizeof (delay));

                            for (i = 3; i <= j - 1; i++) {
                                strcpy(name2, file[i]);
                                size_name2 = strlen(name2);
                                write(sock, &size_name2, sizeof (size_name2));
                                write(sock, name2, size_name2);
                            }



                            read(sock, recv, 2); 
                            read(sock, &size_name, sizeof (size_name));
                            read(sock, name, size_name);
                            read(sock, &amount, sizeof (amount));
                            read(sock, &delay, sizeof (delay));

                            name[size_name] = '\0';


                            if (strncmp(recv, OK, 2) == 0) {
                                printf("Success.  Multi-Transfer addition (%s:%lf:%lf) \n", name, amount, delay);
                            } else {
                                printf("Error.  Multi-Transfer addition failed (%s:%lf:%lf) \n", name, amount, delay);
                            }

                        }



                    }
                }

                if (strcmp(file[0], d4) == 0) { //printbalance
                    if (j != 2) {
                        printf("Error!Dwste sthn printbalance ena onoma logariasmou\n");
                    } else {
                        strcpy(name, file[1]);
                        size_name = strlen(name);

                        write(sock, pb, 2);
                        write(sock, &size_name, sizeof (size_name));
                        write(sock, name, size_name);

                        read(sock, recv, 2); //vazoume sto pipe to ok wste na t lavei o client
                        read(sock, &size_name, sizeof (size_name));
                        read(sock, name, size_name);
                        read(sock, &amount, sizeof (amount));

                        name[size_name] = '\0';


                        if (strncmp(recv, OK, 2) == 0) {
                            printf("Success.  Balance (%s:%lf) \n", name, amount);
                        } else {
                            printf("Error. Balance (%s) \n", name);
                        }



                    }
                }
                if (strcmp(file[0], e5) == 0) { //printmultibalance
                    if (j < 3) {
                        printf("Error!Dwste sthn printmultibalance toulaxiston  onomata logariasmwn \n");
                    } else {
                        write(sock, mb, 2);
                        counter = j - 1;
                        write(sock, &counter, sizeof (counter));

                        for (i = 1; i <= j - 1; i++) {
                            strcpy(name, file[i]);
                            size_name = strlen(name);
                            write(sock, &size_name, sizeof (size_name));
                            write(sock, name, size_name);
                        }

                        read(sock, recv, 2);

                        if (strncmp(recv, OK, 2) == 0) {
                            printf("Success. Multi-Balance (");
                            for (i = 1; i <= j - 1; i++) {
                                read(sock, &size_name, sizeof (size_name));
                                read(sock, name, size_name);
                                name[size_name] = '\0';
                                read(sock, &amount, sizeof (amount));
                                printf(" %s:%lf  ", name, amount);
                            }
                            printf(") \n");
                        } else {
                            printf("Error. Multi- Balance (");
                            for (i = 1; i <= j - 1; i++) {
                                read(sock, &size_name, sizeof (size_name));
                                read(sock, name, size_name);
                                name[size_name] = '\0';
                                printf(" %s : ", name);
                            }
                            printf(") \n", name);
                        }
                    }
                }
                if (strcmp(file[0], f6) == 0) { //sleep
                    if (j != 2) {
                        printf("Error!Dwste sthn sleep ena time\n");
                    } else {
                        delay = atof(file[1]);
                        delay = delay / 1000;
                        printf("please be waiting %lf s\n", delay);

                        sleep(delay);

                    }
                }
                if (strcmp(file[0], g7) == 0) { //exit
                    if (j != 1) {
                        printf("Error!\n");
                    } else {
                        close(sock);
                        return 0;
                    }
                }
            } else {
                strcpy(name, file[0]);
                write(sock, name, 2);
                read(sock, &size_name, sizeof (size_name));
                read(sock, name, size_name);
                name[size_name] = '\0';
                printf("%s \n", name);

            }

        }


    }
    return (EXIT_SUCCESS);
}

void perror_exit(char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}
