/****************************************************************************************/
/****************************************************************************************/
/***************                                                          ***************/
/***************     Implementation of the client class : Client_TCP      ***************/
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
#include <sys/select.h>

using namespace std;

/* definition of the constants which are used */

#define MAXLGMSG 1024 	/* max size of messages */
#define LNGHOST 128 	/* maximum hostname size */
#define LNGPSEUDO 128   /* max size of pseudo */

#define SOCK_ERR        -1      /* erreur fonctions sockets */
#define PORT_SERVEUR    58003   /* port number */

/* include the header file */
#include "Client_TCP.h"


/* Additional utility functions */

void bye(int s)
{
    cout << "C-> Salut !\n" << endl;
    close(s);
    exit(0);
}


/* function to know the availability of input on stdin */
bool inputAvailable()  
{
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    return (FD_ISSET(0, &fds));
}

/* Instead of cin>> */
int read_line (char* buff)
{
    int i=0;
    char c;

    do{
        c=getchar();
        if (c != '\n') {buff[i]=c;}
        i++;
    } while (c != '\n');
    buff[i]='\0';
    return --i;
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
	int psd_msg;  // Used to visualize received msgs
*/

// port number; host name
Client_TCP::Client_TCP (int p, char* S) {

    // Parameters
    port=p;
    strcpy(host_s, S);
    psd_msg=0;

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


}; // End of client constructor


void Client_TCP::Connet_to_server (){

    cout<<"Connecting to Server..." << endl;

    /* Connecting to the server socket */
    if (connect(s_d, (struct sockaddr *)&s_a, sizeof(s_a)) < 0) {
        perror("C-> connect");
        bye(s_d);
    }
    else{
        /* Connected successfully */
        char psd[LNGPSEUDO];
        int sz_psd;

        read_line(psd);
        fflush(stdin);

        cout << "Enter Your PSEUDO :";
        sz_psd=read_line (psd);

        /* sending the client's pseudo */
        if ((sz_psd = send(s_d,psd,LNGPSEUDO,0)) == SOCK_ERR) {
            perror("C-> send");
            bye(s_d);
        }
        else
            cout <<"Connected !"<<endl;
    }

    
}

/* Sending a message of a given size  (*MSG, size) */
int Client_TCP::SendMsg (char* s_t, int ls_t) {
    
    int nbeff;

    if (inputAvailable()){
        /* typing the message */
        cout<<">>> Your Message (Press 'Q' to quit):  ";
        read_line (s_t);
        cout<< s_t << endl;

        /* sending the message */
        if ((nbeff = send(s_d,s_t,ls_t,0)) == SOCK_ERR) {
            perror("C-> send");
            bye(s_d);
        }
        else if (nbeff==0) {
            /* the server socket is interrupted */
            cout << ">>> SERVER DOWN !" << endl;
            close(s_d);
        }
    
    }
    return(nbeff); // returns the number of char effectively sent
};


int Client_TCP::RecvMsg (char* in_t, int l_in_t){

    int rec;

    fcntl(s_d, F_SETFL, O_NONBLOCK); /* Change the socket into non-blocking state	*/

    while ((rec=recv(s_d,in_t,MAXLGMSG,0)) >0 ){
        /* the server broadcasts the pseudo of the client who sent the message, followed by his message */
        if (psd_msg == 0){
            cout << " \t \t \t  >>> " << in_t << " : ";
            psd_msg=1;
        }
        else {
            cout << in_t << endl ;
            psd_msg=0;
        }
    };


    return(rec);
}


void Client_TCP::Disconnet_from_server (){

    cout << "***                                                  ***" << endl;
    cout << "*******        Disconnecting from Server...      *******" << endl;
    cout << "**********                                    **********" << endl;
    cout << "*****************                      *****************" << endl;
    cout << "********************************************************" << endl;
    cout << endl;

    close(s_d);
}


/* Display Server Info */
void Client_TCP::Display() {
    cout << " Port Server: " << port << " Host_s: " << host_s << endl;	
}


void Client_TCP::Welcome() {
    cout << endl;
    cout << "********************************************************" << endl;
    cout << "********************************************************" << endl;
    cout << "**********                                    **********" << endl;
    cout << "**********      WELCOME TO THE CHAT ROOM      **********" << endl;
    cout << "**********                                    **********" << endl;
    cout << "********************************************************" << endl;
    cout << "********************************************************" << endl;
    cout << endl;
}

