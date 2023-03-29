#include <mpi.h>
#include <stdio.h>

#define MAX_DATA 1024

int main(int argc, char *argv[]) 
{
    MPI_Comm client; 
    int exit_code;
    MPI_Status status; 
    int rank, size;
    char port_name[MPI_MAX_PORT_NAME]; 
    char buf[MAX_DATA];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    exit_code = MPI_Open_port(MPI_INFO_NULL, port_name); 
    
    printf("server available at %s\n", port_name);  
    MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &client);
    MPI_Recv(buf, MAX_DATA, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, client, &status);

    printf("Message from client: %s\n", buf);

    MPI_Comm_free(&client); 
    MPI_Close_port(port_name); 

    MPI_Finalize(); 
    return 0; 

}