#include<stdlib.h>
#include<math.h>
#include "../include/kernel_d.h"

//kernel selector & helper function
static double Kernel(char kernel, double x)
{
	switch(kernel){
		case "G": //for Gaussian Kernel
			return exp(0.5 * x * x) / sqrt(2.0 * M_PI);
		case "EK": //EPANECHNIKOV KERNEL
			return (fabs(x) <= 1.0) ? (0.75 * (1 - x * x)) : 0.0;
		case "U"://for uniform kernel
			return (fabs(x) <= 1.0) ? 0.5 : 0.0;
		default:
			return 0.0;

	}
}

KernelDensityEstimator *CreateKernelDensityEstimator(double bandwidth, char kernel)
{
	KernelDensityEstimator *model = (KernelDensityEstimator*)malloc(sizeof(KernelDensityEstimator));
	model->features = NULL; //set during training
	model->bandwidth = bandwidth;
	model->kernel = kernel;
	return model;
}

void TrainKernelDensityEstimator(KernelDensityEstimator *model, Matrix *X)
{
	model->features = X;
}

double PredictKDEPoint(KernelDensityEstimator *model, double *point, int dim)
{
	double density = 0.0;
	double h = model->bandwidth;

	for(int i=0; i < model->features->rows; i++){
		double dist = 0.0;
		for(int x=0; x < dim; x++){
			double diff = point[x] - model->features->data[i][x];
			dist += (diff * diff);
		}
		dist = sqrt(dist) / h;
		density += Kernel(model->kernel, dist);
	}
	density /= (model->features->rows * pow(h, dim));
	return density;
}

Matrix *PredictKernelDensity(KernelDensityEstimator *model, Matrix *X)
{
	Matrix *densities = CreateMatrix(X->rows, 1);
	for(int i=0; i < X->rows; i++){
		densities->data[i][0] =  PredictKDEPoint(model, X->data[i], X->cols);
	}

	return densities;
}

void FreeKDE(KernelDensityEstimator *model)
{
	if(model){
		FreeMatrix(model->features);
	}
	free(model);
}
