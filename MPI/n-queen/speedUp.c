#include "Timing_Statistics_Utils/timath.h"
#include "n-queens.h"
#include <wait.h>
#include <unistd.h>
#include <string.h>

void runWithBoardSize(int bz, int th)
{

	double time[5];
	double start;
	double stop;


	char size[2];
	char threads[2];

    sprintf(size, "%d", bz);
    sprintf(threads, "%d", th);

    char* fileName = malloc(50);

    strcpy(fileName, "mpirun -n ");
    strcat(fileName, threads);
    strcat(fileName, " ./relay ");
    strcat(fileName,  size);

    //printf("%s", fileName);
    //printf("\n");

	for(int i = 0; i < 5; i++){
		start = getWallTime();

		system(fileName);

		stop = getWallTime();
		time[i] = stop - start;
	}


	printf("\n --- EXPERIMENT WITH %d x %d BOARD --- \n\n",  bz, bz);
	printf("MEDIAN: %lf\n", median(5, time));
	printf("MEAN: %lf\n", mean(5, time));
	printf("VARIANCE: %lf\n", variance(5, time));
	printf("DEVIATION: %lf\n", deviation(5, time));
	printf("\n\n --- EXPERIMENT OVER --- \n\n");

}




int main(int argc, char const *argv[])
{

	//int th = strtol(argv[1], NULL, 10);

	int th = 8;

	// PROBLEM SIZE CHANGE

	runWithBoardSize(6 ,th);
	runWithBoardSize(8, th);
	runWithBoardSize(10, th);
	runWithBoardSize(12, th);


	// THREAD SIZE CHANGE

	runWithBoardSize(12 ,2);
	runWithBoardSize(12, 4);
	runWithBoardSize(12, 6);
	runWithBoardSize(12, 8);


	/* code */
	return 0;
}