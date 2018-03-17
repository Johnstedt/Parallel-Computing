#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

typedef enum { false, true } bool;

#define N 8

int numSol = 0;
 
/* A utility function to print solution */
void printSolution(int board[N][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            printf(" %d ", board[i][j]);
        printf("\n");
    }
    printf("\n");
}
 
/* A utility function to check if a queen can
   be placed on board[row][col]. Note that this
   function is called when "col" queens are
   already placed in columns from 0 to col -1.
   So we need to check only left side for
   attacking queens */
bool isSafe(int board[N][N], int row, int col)
{
    int i, j;
 
    /* Check this row on left side */
    for (i = 0; i < col; i++)
        if (board[row][i])
            return false;
 
    /* Check upper diagonal on left side */
    for (i=row, j=col; i>=0 && j>=0; i--, j--)
        if (board[i][j])
            return false;
 
    /* Check lower diagonal on left side */
    for (i=row, j=col; j>=0 && i<N; i++, j--)
        if (board[i][j])
            return false;
 
    return true;
}
 
bool solveNQUtil(int board[N][N], int col, int stop)
{

    if (col >= N) {
    	numSol++;
    	//printf("Solution %d\n", numSol);
 		//printSolution(board);
 		return true;
    }
    if(board[stop][0] == 1){
    	return false;
    }

    /* Consider this column and try placing
       this queen in all rows one by one */
    for (int i = 0; i < N; i++)
    {
        /* Check if queen can be placed on
          board[i][col] */
        if ( isSafe(board, i, col) )
        {
            /* Place this queen in board[i][col] */
            board[i][col] = 1;
 
            /* recur to place rest of the queens */
            solveNQUtil(board, col + 1, stop);
            //if ( solveNQUtil(board, col + 1) )
              //  return true;
 
            board[i][col] = 0; 
        }
    }

    return false;
}

bool solveNQ(int startI, int startJ, int stopI, int StopJ)
{
    int board[N][N];

    for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			board[i][j] = 0;
		}
	}
	board[startI][0] = 1;
 
 	solveNQUtil(board, 1, stopI);
    
    return true;
}

int calculateSufficientDepth(int threads, int boardSize) {

  if(threads*5 <= ((boardSize/2) + (boardSize % 2))){
    return 1;
  } else if (threads*5 <= ((boardSize/2)+(boardSize % 2))*boardSize) {
    return 2;
  }
  else { //depth beyond three is ridiculous in all but the most trivial board sizes.
    return 3;
  }
}

int getProblems(int depth, int boardSize){
  if(depth == 1){
    return boardSize/2 + (boardSize % 2) ;
  } 
  else if (depth == 2){
    return ((boardSize/2)+(boardSize % 2))*boardSize ;
  } 
  else {
    return ((boardSize/2)+(boardSize % 2))*boardSize*boardSize ;
  }
}

int main(int argc, char* argv[]) {

//	int n = strtol(argv[1], NULL, 10);

	int my_rank, comm_sz;

  int requiredDepth = calculateSufficientDepth(16, N);
  int problems = getProblems(requiredDepth, N);

  int boards[problems][N][N];

  int j 
  for(int i = 0; i < problems; i++){
    for(int j = (depth-1); j > -1; j--){
      if(j == 0){
        i % N
        boards[i][j][i % N] = 1;
      }else {
        boards[i][j][(i / (j*N)) % N] = 1:
      }   

     
    }
  }

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

  int *gather;
  gather = calloc(N, sizeof(int));

  int *apointer;
  apointer = calloc(N, sizeof(int));


	if(comm_sz == N){
		if(my_rank == N-1){
      solveNQ(my_rank, 0, 0, N);
    }else {		
      solveNQ(my_rank, 0, my_rank + 1, N);
    }
	}

	printf("MY THREAD NO %d I HAVE %d SOLUTIONS\n", my_rank, numSol);
	

	apointer[0] = numSol;

	MPI_Reduce(apointer, gather, N, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if(my_rank == 0){

		printf("TOTAL SOLUTIONS %d\n", *gather);

	}
	free(gather);
  free(apointer);

  MPI_Finalize(); 
	return 0;
}

// 2 * 2 - (N-2*3)