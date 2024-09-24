#ifndef FSVM_H
#define FSVM_H

#include<stdlib.h>
#include<stdio.h>
#include "../include/matrix.h"


typedef struct {
	Matrix *weights;
	double bias;
	double regularization;
	Matrix *fuzzy_memberships;

} FuzzySVM;


FuzzySVM* CreateFSVM(int features, double regularization, Matrix *fuzzy_memberships);
void TrainFSVM(FuzzySVM *model, Matrix *X, Matrix *Y);
double PredictFSVM(FuzzySVM *model, Matrix *X);
void FreeFSVM(FuzzySVM *model);


#endif //FSVM_H
