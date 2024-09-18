#ifndef LINEAR_SVM_H
#define LINEAR_SVM_H


#include<stdlib.h>
#include<stdio.h>
#include "../include/matrix.h"



typedef struct {
	Matrix *weights;
	double learning_rate;
	double bias;
	double lambda;
	int epochs;

} LinearSVM;



LinearSVM* CreateSVM(int features, double learning_rate, double lambda, int epochs);
void FreeSVM(LinearSVM *model);
void TrainSVM(LinearSVM *model, Matrix *X, Matrix *Y);
double PredictSVM(LinearSVM *model, Matrix *X)



#endif //LINEAR_SVM_H
