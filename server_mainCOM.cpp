/****************************************************************************************/
/****************************************************************************************/
/***************                                                          ***************/
/***************            TCP/Ip Chat application : Server              ***************/
/***************                                                          ***************/
/***************                 Author : SMOUNI Marwan                   ***************/
/***************                                                          ***************/
/****************************************************************************************/
/****************************************************************************************/

#include <iostream>
#include <string>
using namespace std;

#include "Server_TCP.h"

/* definition of the constants which are used */
#define MAXLGMSG 1024
#define LNGHOST 128

int main () {

    /* Host name >>s */
    char s[LNGHOST];

    cout << "Host name? " ; cin >> s;

    /* Server_TCP class instance (port,@) */
    Server_TCP channel(18002, s);

    /* input/output buffer*/
    char buffer[MAXLGMSG];

    /* Number of clients*/
    int nbr_cl;

    cout << "Number of clients? " ; cin >> nbr_cl;
    cout <<endl;

    /* Connect all clients*/
    for (int i=0;i<nbr_cl;i++){
        channel.Connet_to_client();
    }

    /* Display Server informations */
    channel.Display();

    channel.Welcome();

    int nbr=1;

    /* Receive and broadcast loop */
    while (nbr!=0){

        nbr=channel.RecvMsg(buffer);
    }

}
