#ifndef SSVM_H
#define SSVM_H

#include<stdlib.h>
#include<stdio.h>
#include "../include/matrix.h"



typedef struct {
	Matrix *weights;
	double C;
} StructuredSVM;


StructuredSVM* CreateSSVM(int features, double C);
void TrainSSVM(StructuredSVM *model, Matrix *X, Matrix *Y, int epochs);
Matrix* PredictSSVM(StructuredSVM *model, Matrix *X);
void FreeSSVM(StructuredSVM *model);


#endif //SSVM_H
