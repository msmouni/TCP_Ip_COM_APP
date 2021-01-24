/****************************************************************************************/
/****************************************************************************************/
/***************                                                          ***************/
/***************       Declaration of the client class : Client_TCP       ***************/
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




class Client_TCP {
protected:
	char buffer [MAXLGMSG];
	int size;
	int s_d;  // Socket descriptor id
	struct sockaddr_in s_a; // Server @
	char host_s[LNGHOST]; // Host name
	int port; // Server port number
	struct hostent *s_h; // Host entry desc
	int psd_msg;  // Used to visualize received msgs

public:
	// port number; host name
	Client_TCP (int , char*);

    //Connet to server
    void Connet_to_server ();

	// Message sending of a given size
	int SendMsg (char*, int);

	//Message receiving of a given size
	int RecvMsg (char* , int );

    //Disconnect
    void Disconnet_from_server ();

	// Server_UDP display
    void Display();

	void Welcome(); 
};