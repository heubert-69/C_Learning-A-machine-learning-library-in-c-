#ifndef KMEANS_H
#define KMEANS_H

#include "matrix.h"

typedef struct {
	int k;
	int max_iters;
	double tol; //convergence tolerance
	Matrix *centroids;
} KMeans;

KMeans *CreateKMeans(int k, int max_iters, double tol);
int *TrainKMeans(KMeans *model, Matrix *X);
int *PredictKMeans(KMeans *model, Matrix *X);
void FreeKMeans(KMeans *model);

#endif //KMEANS_H
