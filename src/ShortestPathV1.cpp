/*
 ============================================================================
 Name        : ShortestPath.cpp
 Author      : Siavash Katebzadeh
 Version     : 1
 Copyright   : GPL
 Description : ShortestPath in MPI
 ============================================================================
 */
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

void master(int size,char * file){
	MPI_Status status;

	FILE *I_in;

	ifstream M_in(file, ios::in);
	int N,tmp,index;
	M_in >> N;


	int data[N*N];
	for (int y = 0; y < N; y++)
		for (int x = 0; x < N; x++){
			   M_in >> tmp;
			   data[y*N + x] = tmp;
		}


	MPI_Bcast (&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Bcast (data, N*N, MPI_INT, 0, MPI_COMM_WORLD);

	int count = (int) ceil(N/size);
	int start = N;
	int total = N*N;

	floydsAlgorithm(0,data,N,0,count);

	int t[total];
	for(int p=1;p<size;p++){
		MPI_Recv(&t, total, MPI_INT, p, 0, MPI_COMM_WORLD,&status);
		for(int v=0;v<total;v++){
			data[v] = max(data[v],t[v]);
		}
	}

	for(int i=0;i<N;i++){
		for (int j=0;j<N;j++){
			index = i*N+j;
			if(data[index] == SMINF)
				cout << 0 << ' ';
			else
				cout << data[index] << ' ';
		}
		cout << endl;
	}
}

void slave(int rank,int S){
	int N;
	MPI_Status status;

	MPI_Bcast (&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int num = N;
	int size = N * N;
	int data[size];

	MPI_Bcast (&data, size, MPI_INT, 0, MPI_COMM_WORLD);

	int count = (int) ceil(num/S);
	int start = rank * count;
	if((num * start) + (num * count) > size) count = N - start;

	floydsAlgorithm(rank,data,num,start,count);

	MPI_Send(data,size,MPI_INT,0,0,MPI_COMM_WORLD);
}

int main(int argc, char * argv[]){
	int size, my_rank, len;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	char * file;


	if (my_rank == 0) {
		printf("=========================================================\n");
		printf("Name        : ShortestPath.cpp\n");
		printf("Author      : Siavash Katebzadeh\n");
		printf("Version     : 1\n");
		printf("Copyright   : GPL\n");
		printf("Description : ShortestPath in MPI\n");
		printf("=========================================================\n");
		if (argc != 2) {
			printf("Invalid input! Usage: mpirun -np p ShortestPath file\n");
		} else {
			file = (argv[1]);

		}
	}

	if (my_rank == 0)
	{
		set_clock();
		master(size,file);
		cout << "Time: "<< elapsed_time()<< endl;
	}
	else{
		slave(my_rank,size);
	}
	MPI_Finalize();
}
