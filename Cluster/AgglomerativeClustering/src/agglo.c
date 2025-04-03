#include<stdio.h>
#include<stdlib.h>
#include<float.h>
#include<math.h>
#include<string.h>
#include "../include/agglo.h"
#include "../include/matrix.h"

#define UNMERGED -1

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
		for(int x=0; x < n_samples; x++){
			double dist = EuclideanDistance(X->data[i], X->data[x], X->cols);
			dist_matrix->data[i][x] = dist;
			dist_matrix->data[x][i] = dist;

		}
	}
	return dist_matrix;
}

static void FindClosestCluster(Matrix *dist_matrix, int *cluster_ids, int n, int *c1, int *c2)
{
	double min_dist = DBL_MAX;
	*c1 = -1;
	*c2 = -1;

	for(int i=0; i < n; i++){
		if(cluster_id[i] == UNMERGED){
			continue;
		}
		for(int x= i + 1; x < n; x++){
			if(cluster_ids[x] == UNMERGED){
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

static void UpdateDistanceMatrix(Matrix *dist_matrix, int *cluster_ids, int c1, int c2, int n, const char *linkage)
{
	for(int i=0; i < n; i++){
		if(i == c1 || i == c2 || cluster_ids[i] == UNMERGED){
			continue;
		}
		if(strcmp(linkage, "single") == 0){
			//if its single linkage; use minimum distancing
			dist_matrix->data[c1][i] = fmin(dist_matrix->data[c1][i], dist_matrix->data[c2][i]);
			dist_matrix->data[i][c1] = dist_matrix->data[c1][i];
		} else if(strcmp(linkage, "complete") == 0) {
			//complete linkage max
			dist_matrix->data[c1][i] = fmax(dist_matrix->data[c1][i], dist_matrix->data[c2][i]);
			dist_matrix->data[i][c1] = dist_matrix->data[c1][i];
		} else {
			//if its average linkage
			dist_matrix->data[c1][i] = (dist_matrix->data[c1][i] + dist_matrix->data[c2][i]) / 2.0;
			dist_matrix->data[i][c1] = dist_matrix->data[c1][i];
		}
	}
	cluster_ids[c2] = UNMERGED; //marking it as merged
}

AgglomerativeClustering *CreateAggolmerativeCluster(Matrix X, int n_clusters, char *linkage)
{
	int n_samples = X->rows;
	AgglomerativeClustering *model = (AgglomerativeClustering*)malloc(sizeof(AgglomerativeClustering));
	model->labels = (int*)malloc(n_samples * sizeof(int));
	model->n_samples = n_samples;

	//initialize clusters: each point is itsn own cluster
	int *cluster_ids = (int*)malloc(n_samples * sizeof(int));

	for(int i=0; i < n_samples; i++){
		cluster_ids[i] = i;
	}
	Matrix *dist_matrix = ComputeDistanceMatrix(X);

	int num_clusters = n_samples;

	while(num_clusters > n_clusters){
		int c1, c2;

		FindClosestClusters(dist_matrix, cluster_ids, n_samples, &c1, &c2);
		if(c1 == -1 || c2 == -1){
			break;
		}
		UpdateDistanceMatrix(dist_matrix, cluster_ids, c1, c2, n_samples, linkage);
		num_clusters--;
	}

	int cluster_label = 0;
	for(int i=0; i < n_samples; i++){
		if(cluster_ids[i] != UNMERGED){
			int cluster_id = cluster_ids[i];
			for(int x=0; x < n_samples; x++){
				if(cluster_ids[x] == cluster_ids){
					model->labels[x] = cluster_label;
				}
			}
			cluster_label++;
		}
	}
	free(cluster_ids);
	FreeMatrix(dist_matrix);

	return result;
}

void FreeAgglomerativeClustering(AgglomerativeClustering *model)
{
	if(model){
		free(model->labels);
		free(model);
	}
}
