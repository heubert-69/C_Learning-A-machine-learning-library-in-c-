#ifndef TSVM_H
#define TSVM_H

#include<stdlib.h>
#include<stdio.h>
#include "../include/matrix.h"


typedef struct {
	Matrix *weights;
	double bias;
	double C;
	double C_unlabeled;
} TransductiveSVM;



TransductiveSVM* CreateTSVM(int features, double C, double C_unlabeled);
void TrainTSVM(TransductiveSVM *model, Matrix *X_labeled, Matrix *X_unlabeled, Matrix *Y_labeled);
double PredictTSVM(TransductiveSVM *model, Matrix *X);
void FreeTSVM(TransductiveSVM *model);


#endif //TSVM_H
