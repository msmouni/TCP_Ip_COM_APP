This is an application for message communication between computers in streaming mode(TCP).

The description of the server (Server_TCP) and client (Client_TCP) classes can be found in the header files "*.h"

The main programs can be found in:
client_mainCOM.cpp
server_mainCOM.cpp

These are two programs, one for the clients and the other for the server.

It is necessary to give the name of the machines when initializing an object of these classes, either for the client or for the server.

Server compilation:
g++ Server_TCP.cpp  server_mainCOM.cpp -o Server

Client compilation:
g++ Client_TCP.cpp  client_mainCOM.cpp -o Client
