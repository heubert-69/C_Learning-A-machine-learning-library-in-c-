#include<stdio.h>
#include<stdlib.h>
#include<float.h>
#include<math.h>
#include "../include/spectre.h"


//Gaussian Similarity
static double GaussianSimilarity(double *a, double *b, int d, double sigma)
{
	double sum = 0.0;
	for(int i=0; i < d; i++){
		double diff = a[i] - b[i];
		sum += diff * diff;
	}
	return exp(-sum / (2 * sigma * sigma));
}

static Matrix *ComputeAffinity(Matrix *X, double sigma)
{
	int n = X->rows;
	Matrix *W = CreateMatrix(n, n);
	for(int i=0; i < n; i++){
		for(int x=0; x < n; x++){
			if(i == x){
				W->data[i][x] = 0.0;
			}
			else {
				W->data[i][x] = GaussianSimilarity(X->data[i], X->data[x], X->cols, sigma);
			}
		}
	}
	return W;
}


static Matrix *ComputeDegree(Matrix *W)
{
	int n = W->rows;
	Matrix *D = CreateMatrix(n, n);
	for(int i=0; i < n; i++){
		double sum = 0.0;
		for(int x=0; x < n; x++){
			sum += W->data[i][x];
		}
		D->data[i][i] = sum;
	}
	return D;
}

static Matrix *ComputeLaplace(Matrix *D, Matrix *W)
{
	int n = D->rows;
	Matrix *L = CreateMatrix(n, n);
	for(int i=0; i < n; i++){
		for(int x=0; x < n; x++){
			L->data[i][x] = D->data[i][x] - W->data[i][x];
		}
	}
	return L;
}

static Matrix *ComputeEigen(Matrix, *D, int n_clusters)
{
	int n = D->rows;
	Matrix *E = CreateMatrix(D, n_clusters);
	for(int i=0; i < n; i++){
		for(int x=0; x < n; x++){
			E->data[i][x] = D->data[i][x];
		}
	}
	return E;
}

static int KMeans(Matrix *X, int n_clusters)
{
	int *labels = (int*)malloc(X->rows * sizeof(int));
	for(int i=0; i < X->rows; i++){
		labels[i] = i % n_clusters; //Fake Clustering.
	}
	return labels;
}


SpectralClustering *CreateSpectre(Matrix *X, int n_clusters, double sigma)
{
	int n = X->rows;

	//computing the affinity matrix
	Matrix *A = ComputeAffinity(X, sigma)

	//computing The Degree
	Matrix *D = ComputeDegree(X);

	//computing Laplace computation
	Matrix *L = ComputeLaplace(Matrix *D, Matrix *A);

	//computing eigen vectors
	Matrix *E = ComputeEigen(L, n_clusters);

	//computing the Kmeans
	int *labels = KMeans(E, n_clusters);


	//store the data
	SpectralClustering *model = (SpectralClustering*)malloc(sizeof(SpectralClustering));
	model->labels = labels;
	model->n_samples = n;

	//free the matrices
	FreeMatrix(A);
	FreeMatrix(D);
	FreeMatrix(L);
	FreeMatrix(E);


	return model;
}

void FreeSpectralClustering(SpectralClustering *model)
{
	if(model){
		if(model->labels){
			free(labels);
		}
		free(model);
	}
}

