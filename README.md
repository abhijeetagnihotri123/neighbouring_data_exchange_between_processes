# neighbouring_data_exchange_between_processes
A dummy matrix data is divided equally among processes and the rows are exchanged between the neighbouring rows.
Why MPI_Send and receive wirk with b[N] but not with int *b;
like  
int b[N];
MPI_Recv(b,N,MPI_INT,Source,TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE) works

but int *b;
MPI_Recv(b,N,MPI_INT,SOURCE,TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE) throws SEG fault.

