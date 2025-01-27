#ifndef KERNEL_D_H
#define KERNEL_D_H

#include "matrix.h"

typedef struct {
	Matrix *features;
	double bandwidth; //parameter for bandwidth
	char kernel; // gaussian, epanechnikov or uniform
} KernelDensityEstimator;


KernelDensityEstimator *CreateKernelDensityEstimator(double bandwidth, char kernel);
void TrainKernelDensityEstimator(KernelDensityEstimator *model, Matrix *X);
double PredictKDEPoint(KernelDensityEstimator *model, double *point, int dim);
Matrix *PredictKernelDensity(KernelDensityEstimator *model, Matrix *X);
void FreeKDE(KernelDensityEstimator *model);

#endif //KERNEL_D_H6
