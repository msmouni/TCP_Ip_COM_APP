/****************************************************************************************/
/****************************************************************************************/
/***************                                                          ***************/
/***************            TCP/Ip Chat application : Client              ***************/
/***************                                                          ***************/
/***************                 Author : SMOUNI Marwan                   ***************/
/***************                                                          ***************/
/****************************************************************************************/
/****************************************************************************************/

#include <iostream>
#include <string>
#include <cstring>
using namespace std;

#include "Client_TCP.h"

/* definition of the constants which are used */
#define MAXLGMSG 1024
#define LNGHOST 128




int main () {

    char s[LNGHOST];

    cout << "Host name? " ; cin >> s;

    /* Client_TCP class instance (port,@) */
    Client_TCP channel(18002, s);


    /* Display Server informations */
    channel.Display();

    /* Connecting to the server */
    channel.Connet_to_server ();

    channel.Welcome();

    /* input buffer*/
    char str_in[MAXLGMSG];
    /* output buffer*/
    char str_out[MAXLGMSG];

    /* Asking user to press "Q" to quit */
    while (strcmp ("Q", str_out) != 0){

        /* clean the buffer */
        for (int i=0;i<MAXLGMSG;i++){
            str_in[i]='\0';
            str_out[i]='\0';
        } 

        channel.SendMsg(str_out, sizeof(str_out));
        channel.RecvMsg(str_in, sizeof(str_in));

    }

    // Disconnect
    channel.Disconnet_from_server ();


}
