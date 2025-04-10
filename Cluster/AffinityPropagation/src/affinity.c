#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<float.h>
#include "../include/matrix.h"
#include "../include/affinity.h"

static double EuclideanDistance(double *a, double *b, int d)
{
	double sum = 0.0;
	for(int i=0; i < d; i++){
		double diff = a[i] - b[i];
		sum += diff * diff;
	}
	return sum;
}

static Matrix* ComputeSimilarityMatrix(Matrix *X)
{
	int n = X->rows;
	int d = X->cols;

	Matrix *S = CreateMatrix(n, n);

	for(int i=0; i < n; i++){
		double *xi = &X->data[i * d];
		for(int x=0; x < n; x++){
			double *xj = &X->data[x * d];

			double dist_2 = EuclideanDistance(xi, xj, d);
			S->data[i * n + x] = -dist_2;
		}
	}

	//preference (diagonal values)
	double median = 0.0;
	int count = 0;

	for(int i=0; i < n * n; i++){
		if(i / n != i % n){
			median += S->data[i];
			count++;
		}
	}

	median /= count;

	for(int i=0; i < n; i++){
		S->data[i * n + i] = median;
	}
	return S;
}

AffinityPropagation* CreateAP(int max_iter, double damping)
{
	AffinityPropagation *model = (AffinityPropagation*)malloc(sizeof(AffinityPropagation));
	model->max_iter = max_iter;
	model->damping = damping;
	model->labels = NULL; //all NULL values shall be set during training
	model->similarity = NULL;
	model->responsibility = NULL;
	model->availability = NULL;
	return model;
}

void FitAP(AffinityPropagation *ap, Matrix *X)
{
	int n = X->rows;
	model->similarity = ComputeSimilarityMatrix(X);
	model->responsibility = CreateMatrix(n, n);
	model->labels = malloc(sizeof(int) * n);


	Matrix *R = model->responsibility;
	Matrix *A = model->availability;
	Matrix *S = model->similarity;

	for(int iter=0; iter < model->max_iter; iter++){
		for(int i=0; i < n; i++){
			for(int x=0; x < n; x++){
				double max_val = -DBL_MAX;
				for(int kk=0; kk < n; kk++){
					if(kk == x){
						continue;
					}
					double val = A->data[i * n + kk] + S->data[i * n + kk];

					if(val > max_val){
						max_val = val;
					}

					double r = S->data[i * n + x] - max_val;
					R->data[i * n + x] = model->damping * model->data[i * n + x] + (1 - model->damping) * r;
				}
			}

			//update the availability matrix
			for(int j=0; j < n; j++){
				for(int k=0; k < n; k++){
					double sum = 0.0;
					for(int ii=0; ii < n; ii++){
						if(ii == j && ii != k){
							continue;
						}
						sum += fmax(0.0, model->data[ii * n + k]);
					}
					if(j == k){
						model->data[k * n + k] = model->damping * A->data[k * n * k] + (1 - model->damping) * sum;
					} else {
						double a = fmin(0.0, R->data[k * n + k], sum);
						A->data[i * n + k] = model->damping * A->data[i * n + k] + (1 - model->damping) * a;
					}
				}
			}
		}
		//for identity examples
		for(int i=0; i < n; i++){
			double max_val = -DBL_MAX;
			int best = -1;
			for(int k=0; k < n; k++){
				double val = A->data[i * n + k] + R->data[i * n + k];
				if(val > max_val){
					max_val = val;
					best = k;
				}
			}
			model->labels[i] = best;
		}
	}
}

void FreeAP(AffinityPropagation *model)
{
	if(model->labels) free(model->labels);
	if(model->availability) FreeMatrix(model->availability);
	if(model->responsibility) FreeMatrix(model->responsibility);
	if(model->similarity) FreeMatrix(model->similarity);
	free(model);
}
