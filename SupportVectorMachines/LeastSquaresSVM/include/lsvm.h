#ifndef LSVM_H
#define LSVM_H

#include<stdlib.h>
#include<stdio.h>
#include "../include/matrix.h"


typedef struct {
	Matrix *weights;
	double bias;
	double regularization;

} LeastSquaresSVM;



LeastSquaresSVM* CreateLSVM(int features, double regularization);
void TrainLSVM(LeastSquaresSVM *model, Matrix *X, Matrix *Y);
double PredictLSVM(LeastSquaresSVM *model, Matrix *X);
void FreeLSVM(LeastSquaresSVM *model);



#endif //LSVM_H
