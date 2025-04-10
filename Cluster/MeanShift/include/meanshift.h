#ifndef MEANSHIFT_H
#define MEANSHIFT_H

#include "matrix.h"

typedef struct {
	Matrix *centroids;
	double bandwidth;
	int max_iter;
	double tol;
} MeanShift;


MeanShift* CreateMeanShift(double bandwidth, int max_iter, double tol);
void TrainMeanShift(MeanShift *model, Matrix *X);
int PredictMeanShift(MeanShift *model, Matrix *X);
void FreeMeanShift(MeanShift *model);


#endif //MEANSHIFT_H
