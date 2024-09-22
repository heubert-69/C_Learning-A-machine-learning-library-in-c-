#ifndef MULTI_SVM_H
#define MULTI_SVM_H


#include<stdlib.h>
#include<stdio.h>
#include "../include/matrix.h"

typedef struct {
	Matrix *weights;
	double learning_rate;
	double regularization;
	int epochs;
} MultiClassSVM;


MultiClassSVM* CreateMultiSVM(int classes, int features, double learning_rate, double regularization, int epochs);
void TrainMultiClass(MultiClassSVM *model, Matrix *X, Matrix *Y);
int PredictMultiClass(MultiClassSVM *model, Matrix *X);
void FreeMultiClass(MultiClassSVM *model);


#endif //MULTI_SVM_H
