/****************************************************************************************/
/****************************************************************************************/
/***************                                                          ***************/
/***************     Implementation of the server class : Server_TCP      ***************/
/***************                                                          ***************/
/***************                 Author : SMOUNI Marwan                   ***************/
/***************                                                          ***************/
/****************************************************************************************/
/****************************************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

using namespace std;

/* definition of the constants which are used */

#define MAXLGMSG 1024 	/* max size of messages */
#define LNGHOST 128 	/* maximum hostname size */
#define LNGPSEUDO 128   /* max size of pseudo */

#define SOCK_ERR        -1      /* erreur fonctions sockets */
#define PORT_SERVEUR    58003   /* port number */

/* include the header file */
#include "Server_TCP.h"

// Additional utility functions

void bye(int s)
{
    cout << "C-> Salut !\n" << endl;
    close(s);
    exit(0);
}

/* procedure for decoding and printing IP address */
/* use : print_adr(e_a.sin_addr.s_addr);   */
void print_adr(u_long adr_host)
{
    /* address decoding */

    u_long adr, adr1, adr2, adr3, adr4;

    adr=adr_host;
    adr1= adr / 0x001000000; adr = adr - (adr1 * 0x001000000) ;
    adr2= adr / 0x000010000; adr = adr - (adr2 * 0x000010000) ;
    adr3= adr / 0x000000100; adr = adr - (adr3 * 0x000000100) ;
    adr4= adr ;

    cout << adr4 << "." << adr3 << "." << adr2 << "." << adr1;
}




// Attributs (protected):
/*
	char buffer [MAXLGMSG];
	int size;
	int s_d;  // Socket descriptor id
	struct sockaddr_in s_a; // Server @
	char host_s[LNGHOST]; // Host name
	int port; // Server port number
	struct hostent *s_h; // Host entry desc
    int com[5]; // Communication sockets id
    int n_c;  //number of clients
    struct sockaddr_in list_c_a[5]; 	// list : Socket address of the clients (@IP, port)
	char list_pseudo[5][LNGPSEUDO];   // list : {"PSEUDO1",  "PSEUDO2"  ...}
*/

// port number, host name
Server_TCP::Server_TCP (int p, char* S) {

    // Parameters
    port=p;
    strcpy(host_s, S);
    n_c=0;

    // Initialisation of host entry
    if ((s_h = gethostbyname(host_s)) == NULL) {
        perror("gethostbyname");
        exit(1);
    } 

    // Initialisation of server address
    bzero((char *) &s_a, sizeof(s_a));
    bcopy(s_h->h_addr, (char *)&s_a.sin_addr, s_h->h_length);
    s_a.sin_family = s_h->h_addrtype;
    s_a.sin_port = port /* SERVER port */ ;

    // Creation of the Socket
    if ((s_d = socket(AF_INET, SOCK_STREAM, 0)) == SOCK_ERR) {
        perror("C-> socket");
        bye(s_d);
    }
    cout << "Socket creation... ok!" << endl;

    // Binding
    cout << "Status Server mode (bind)... " << endl;
    if (bind(s_d, (struct sockaddr *)&s_a, sizeof(s_a)) == SOCK_ERR) { 
        perror("S-> bind");
        bye(s_d);
    }

    //Start listening
    listen(s_d,5);

}; // End of server constructor


/* connection to the client */
void Server_TCP::Connet_to_client ( ){
    struct hostent *h_emetteur; 	// host entry of the client machine 
    struct sockaddr_in c_a; 	// Socket address of the client (@IP, port)
    

    int lc_a = sizeof(c_a);

    int psd;
    char pseudo[LNGPSEUDO];  


    cout << "Waiting for client..." << endl;

    /* Accepting the client's connection */
    if ((com[n_c] = accept(s_d, (struct sockaddr *)&c_a, (socklen_t *)&lc_a)) <0){
        perror("S-> accept");
        bye(s_d);
  	}
    else {
        h_emetteur = gethostbyaddr(&c_a.sin_addr,sizeof(&c_a.sin_addr),c_a.sin_family);
        cout << "Connected to : " << endl;
        cout << "S-> client port : " << c_a.sin_port << endl;
        cout << "S-> client host : " << h_emetteur->h_name << endl;
        cout << "S-> client IP @ : " ;
        print_adr(c_a.sin_addr.s_addr);
        cout<< endl << endl;

        cout << "Wainting for client to send PSEUDO ..." <<endl;
        
        if ((psd = recv(com[n_c],pseudo,LNGPSEUDO,0)) >0 ){
            
            strcpy(list_pseudo[n_c], pseudo); 

        }

        /* saving of the address of the connected client */
        list_c_a[n_c]=c_a;
        n_c=n_c+1;
    }

}


/* receiving and then broadcasting messages */
int Server_TCP::RecvMsg (char* e_t){

    int le_t;

    int nbr_msg=0;

    int cmpt;

    char id_pseudo[LNGPSEUDO];
    

    while ( (nbr_msg==0)&(n_c>0) ){
        for (int i=0;i<n_c;i++){

            fcntl(com[i], F_SETFL, O_NONBLOCK); /* Change the socket into non-blocking state	*/

            /* Receive the message from client "com[i]" */
            if ((le_t = recv(com[i],e_t,MAXLGMSG,0)) >0 ){

                strcpy(id_pseudo, list_pseudo[i]);

                cout << "(Number of Clients :" << n_c << " ) : " << id_pseudo << " ( port: "<< list_c_a[i].sin_port << ", @: ";
                print_adr(list_c_a[i].sin_addr.s_addr);
                cout<< ")  : >>>" << e_t <<endl;
                nbr_msg=nbr_msg+1;

                /* Broadcasting the pseudo and the messages of client "com[i]" */
                for (int j=0;j<n_c;j++){
                    if (j!=i){
                        send(com[j],id_pseudo,MAXLGMSG,0);
                        send(com[j],e_t,MAXLGMSG,0);
                    }
                }
            }
            else if (le_t==0){
                /* the client "com[i]" socket is interrupted */
                cout << ">>>( port: "<< list_c_a[i].sin_port << ", @: ";
                print_adr(list_c_a[i].sin_addr.s_addr);
                cout<< ")  :  IS NOT CONNETED "<<endl;

                close(com[i]);
                
                //Clear the client "com[i]" from the list
                for (cmpt = i; cmpt < n_c ; cmpt++){
                    com[cmpt]=com[cmpt+1];
                    list_c_a[cmpt]=list_c_a[cmpt+1];
                    n_c= n_c-1;
                }
            }

        }
    }


    return(le_t); // returns the number of char effectively received 
};

// Display Server Info
void Server_TCP::Display() {
    cout << endl << "INFO ABOUT SERVER :" << endl;
    cout << "Port Server: " << port << endl << "Host_s: " << host_s << endl;
    cout << "(Number of Clients :" << n_c << " )" << endl;
}

void Server_TCP::Welcome() {
    cout << endl;
    cout << "********************************************************" << endl;
    cout << "********************************************************" << endl;
    cout << "**********                                    **********" << endl;
    cout << "**********     WELCOME TO THE CHAT SERVER     **********" << endl;
    cout << "**********                                    **********" << endl;
    cout << "********************************************************" << endl;
    cout << "********************************************************" << endl;
    cout << endl;
}



