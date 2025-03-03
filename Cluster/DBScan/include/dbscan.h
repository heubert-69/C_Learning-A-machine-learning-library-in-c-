#ifndef DBSCAN_H
#define DBSCAN_H

#include"matrix.h"
#include<stdlib.h>
#include<stdio.h>

#define UNCLASSIFIED 0
#define NOISE -1


typedef struct {
	int *labels;
	int n_clusters;
	int n_samples;
} DBSCAN;

//creates the dbscan object withe epsilon and the minimum number of points needed
DBSCAN *CreateDBSCAN(Matrix *X, double eps, int min_pts);

void FreeDBSCAN(DBSCAN *model);


#endif //DBSCAN_H
