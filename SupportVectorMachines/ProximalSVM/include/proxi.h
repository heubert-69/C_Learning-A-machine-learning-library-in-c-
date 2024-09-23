#ifndef PROXI_H
#define PROXI_H

#include<stdlib.h>
#include<stdio.h>
#include "../include/matrix.h"


typedef struct {
	Matrix *weights;
	double learning_rate;
	double regularization;
	int epochs;
} ProximalSVM;


ProximalSVM* CreateProximalSVM(int features, double learning_rate, double regularization, int epochs);
void TrainProximalSVM(ProximalSVM *model, Matrix *X, Matrix *Y);
int PredictProximalSVM(ProximalSVM *model, Matrix *X);
void FreeProximalSVM(ProximalSVM *model);


#endif //PROXI_H
