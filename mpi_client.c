#include "mpi.h" 
#include <string.h>
#include <stdio.h>

#define MESSAGE "Hey! I'm alive!"
#define MAX_DATA 1024
#define TAG "tag#0$connentry#1002C0C50A37875E0000000000000000$"

int main( int argc, char **argv ) 
{ 
    MPI_Comm server; 
    char buf[MAX_DATA]; 
    char port_name[MPI_MAX_PORT_NAME]; 

    MPI_Init( &argc, &argv ); 
    strcpy(port_name, TAG );

    MPI_Comm_connect( port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &server);
    strcpy(buf, MESSAGE);
    MPI_Send(buf, sizeof(MESSAGE) + 1, MPI_CHAR, 0, 1, server);  
    

    MPI_Comm_disconnect( &server ); 
    MPI_Finalize(); 
    return 0; 

}