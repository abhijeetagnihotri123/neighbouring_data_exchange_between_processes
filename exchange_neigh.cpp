#include <iostream>
#include <mpi.h>
#define N 16
using namespace std;
void assignmatrix(int a[N][N])
{
    for(int i=1;i<=N;i++)
    {
        for(int j=1;j<=N;j++)
        {
            a[i-1][j-1]=i*j;
        }
    }
}
int a[N][N];
int main(int argc,char *argv[])
{   
    int id,numworkers;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numworkers);
    MPI_Comm_rank(MPI_COMM_WORLD,&id);
    MPI_Status status;
    MPI_Request request;
    if(id==0)
    {
        assignmatrix(a);
        MPI_Bcast(a,N*N,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(&numworkers,1,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Send(a[N/numworkers-1],N,MPI_INT,(id+1),1,MPI_COMM_WORLD);
        int b[N];
        MPI_Recv(b,N,MPI_INT,(id+1),1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        for(int i=0;i<N;i++)
        {
            a[N/numworkers-1][i]=b[i];
        }
        for(int i=1;i<numworkers;i++)
        {
            int from=i*N/numworkers;
            int till=from+N/numworkers;
            for(int j=from;j<till;j++)
            {
                MPI_Recv(a[j],N,MPI_INT,i,2,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            }
        }
        for(int i=0;i<N;i++)
        {
            for(int j=0;j<N;j++)
            {
                cout<<a[i][j]<<" ";
            }
            cout<<endl;
        }
    }
    else if(id==(numworkers-1))
    {
        MPI_Bcast(a,N*N,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(&numworkers,1,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Send(a[id*N/numworkers],N,MPI_INT,(id-1),1,MPI_COMM_WORLD);
        int b[N];
        MPI_Recv(b,N,MPI_INT,(id-1),1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        for(int i=0;i<N;i++)
        {
            a[id*N/numworkers][i]=b[i];
        }
        int from=id*N/numworkers;
        int till=from+N/numworkers-1;
        for(int i=from;i<=till;i++)
        {
            MPI_Send(a[i],N,MPI_INT,0,2,MPI_COMM_WORLD);
        }
    }
    else
    {
        MPI_Bcast(a,N*N,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(&numworkers,1,MPI_INT,0,MPI_COMM_WORLD);
        int b[N];
        int from,till;
        from=id*N/numworkers;
        till=from+N/numworkers;
        till--;
        MPI_Recv(b,N,MPI_INT,(id-1),1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Send(a[from],N,MPI_INT,(id-1),1,MPI_COMM_WORLD);
        for(int i=0;i<N;i++)
        {
            a[from][i]=b[i];
        }
        MPI_Send(a[till],N,MPI_INT,(id+1),1,MPI_COMM_WORLD);
        MPI_Recv(b,N,MPI_INT,(id+1),1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        for(int i=0;i<N;i++)
        {
            a[till][i]=b[i];
        }
        for(int i=from;i<=till;i++)
        {
            MPI_Send(a[i],N,MPI_INT,0,2,MPI_COMM_WORLD);
        }
    }
    fflush(stdout);
    MPI_Finalize();
    return 0;
}