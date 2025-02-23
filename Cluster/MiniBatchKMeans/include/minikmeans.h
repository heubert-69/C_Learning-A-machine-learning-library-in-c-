#ifndef MINIKMEANS_H
#define MINIKMEANS_H

#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <float.h>


typedef struct {
	int minibatch_size;
	int max_iters;
	int k;
	Matrix *centroids;
	int *counts;
} MiniBatchKMeans;

MiniBatchKMeans *CreateMiniKMeans(int k, int max_iters, int minibatch_size, int d);
int *TrainMiniBatchKMeans(MiniBatchKMeans *model, Matrix *X);
int *PredictMiniBatchKMeans(MiniBatchKMeans *model, double *sample);
void FreeMiniBatchKMeans(MiniBatchKMeans *model);

#endif //MINIKMEANS_H
