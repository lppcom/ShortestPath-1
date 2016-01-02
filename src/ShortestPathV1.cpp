#include <iostream>
#include <fstream>
#include <algorithm>
#include <mpi.h>
#include <math.h>
#include <sys/time.h>
#include "Timer.h"
using namespace std;
#define SMINF 999999

void floydsAlgorithm(int rank, int *data, int N, int start, int count){

	int k,i,j,k_here;
	int ij,ik;

	int rowk[N];

	for (k=0;k<N;k++) {

		int owner = (int) ceil(k/count);
		if (rank == owner) {
			for(j=0;j<N;j++)
				rowk[j]=data[k*N + j];
		}

		MPI_Bcast(&k, 1, MPI_INT, owner, MPI_COMM_WORLD);
		MPI_Bcast(rowk,N,MPI_INT,owner,MPI_COMM_WORLD);
		for(i=start;i<start+count;i++){
			for(j=0;j<N;j++){

				ij = i * N + j;
				ik = i * N + k;

				if(i == j){
					data[ij] = 0;
				}else{
					if(data[ij] == 0) data[ij] = SMINF;
					data[ij]= min(data[ij], data[ik]+rowk[j]);
				}
			}
		}
	}
}
