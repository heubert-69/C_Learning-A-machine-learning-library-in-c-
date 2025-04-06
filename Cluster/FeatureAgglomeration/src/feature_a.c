#include<stdolib.h>
#include<stdio.h>
#include<float.h>
#include<math.h>
#include<string.h>
#include "../include/matrix.h"
#include "../include/feature_a.h"


//calculating Euclidean Distance
static double EuclideanDistance(double *a, double *b, int d)
{
	double sum = 0.0;
	for(int i=0; i < d; i++){
		double diff = a[i] - b[i];
		sum += diff * diff;
	}
	return sqrt(sum);
}


static Matrix *ComputeDistanceMatrix(Matrix *X)
{
	int n_samples = X->rows;
	Matrix *dist_matrix = CreateMatrix(n_samples, n_samples);

	for(int i=0; i < n_samples; i++){
		for(int x=i + 1; x < n_samples; x++){
			double dist = EuclideanDistance(X->data[i], X->data[x], X->cols);
			dist_matrix->data[i][x] = dist_matrix->data[x][i] = dist;

		}
	}
	return dist_matrix;
}

static void FindClosestCluster(Matrix *dist_matrix, int *active, int n, int *c1, int *c2)
{
	double min_dist = DBL_MAX;
	*c1 = -1;
	*c2 = -1;

	for(int i=0; i < n; i++){
		if(!active[i]){
			continue;
		}
		for(int x= i + 1; x < n; x++){
			if(!active[i]){
				continue;
			}
			if(dist_matrix->data[i][x] < min_dist){
				min_dist = dist_matrix->data[i][x];
				*c1 = i;
				*c2 = x;
			}
		}
	}
}


static void MergeFeatures(Matrix *X, int a, int b)
{
	for(int i=0; i < X->cols; i++){
		X->data[a][i] = (X->data[a][i] + X->data[b][i]) / 2.0;
	}
}

FeatureAgglomeration *CreateFeatureAgglomeration(Matrix *X, int n_clusters, char *linkage)
{
	Matrix *XT = TransposeMatrix(X); //each row is now a feature
	int n_features = XT->rows;
	int *labels = malloc(n_features * sizeof(int));
	for(int i=0; i < n_features; i++){
		labels[i] = i;
	}

	for(int x=0; x < n_features; x++){
             	active[x] = 1;
        }

	int *active = malloc(n_features * sizeof(int));

	int cluster = n_features;
       	Matrix *D = ComputeDistanceMatrix(XT);
        while(clusters > n_clusters){
		int i, x;
		FindClosestCluster(D, active, n_features, &i, &x);
		if(i == -1 || x == -1){
			break;
		}
		MergeFeatures(XT, i, x);
		active[x] = 0;


		for(int k=0; k < n_features; k++){
			if(!active[k]){
				continue;
			}
			D->data[i][k] = D->data[k][i] = EuclideanDistance(XT->data[i], XT->data[k], XT->cols);
		}
		for(int n=0; n < n_features; n++){
			if(labels[n] == labels[x]){
				labels[n] = labels[i];
			}
		}
		clusters--;
	}

	//build the reduced matrix (merged features)
	Matrix *reduced = CreateMatrix(X->rows, n_clusters);
	int current = 0;

	for(int i=0; i < n_features; i++){
		if(active[i]){
			for(int x=0; x < X->rows; x++){
				reduced->data[x][current] = XT->data[i][x];
			}
			current++;
		}
	}


	FeatureAgglomeration *model = (FeatureAgglomeration*)malloc(sizeof(FeatureAgglomeration));
	model->labels = labels;
	model->n_features = n_features;
	model->reduced = reduced;

	free(active);
	FreeMatrix(XT);
	FreeMatrix(D);
	return result;
}

void FreeFeatureAgglomeration(FeatureAgglomeration *model)
{
	if(model){
		free(model->labels);
		FreeMatrix(model->reduced);
		free(model);
	}
}







