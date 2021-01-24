/****************************************************************************************/
/****************************************************************************************/
/***************                                                          ***************/
/***************       Declaration of the server class : Server_TCP       ***************/
/***************                                                          ***************/
/***************                 Author : SMOUNI Marwan                   ***************/
/***************                                                          ***************/
/****************************************************************************************/
/****************************************************************************************/


#include <netinet/in.h>

/* definition of the constants which are used */
#define MAXLGMSG 1024
#define LNGHOST 128
#define LNGPSEUDO 128


class Server_TCP {
protected:
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


public:
	// port number; host name
	Server_TCP (int , char*); 

    //Connet to client
    void Connet_to_client ( );

	// Message receive of a given size
	int RecvMsg (char*);

	// Server_UDP display
    void Display();

	void Welcome(); 
};