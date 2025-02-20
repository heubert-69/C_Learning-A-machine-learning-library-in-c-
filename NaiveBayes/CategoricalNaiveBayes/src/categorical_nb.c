#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<float.h>
#include "../include/categorical_nb.h"

//Helper function to find the max category of a categorical array
static int MaxCategory(Matrix *X, int feature_index)
{
	int max_cat = 0;
	for(int i=0; i < X->rows; i++){
		int val = (int)X->data[i][feature_index];
		if(val > max_cat){
			max_cat = val;
		}
	}
	return max_cat;
}

CategoricalNaiveBayes* CreateCategoricalNB(int n_features, int n_classes)
{
	CategoricalNaiveBayes *model = (CategoricalNaiveBayes*)malloc(sizeof(CategoricalNaiveBayes));
	model->n_features = n_features;
	model->n_classes = n_classes;
	model->class_priori = CreateMatrix(1, n_classes);
	model->n_categories = (int*)calloc(n_features, sizeof(int));
	model->feature_probs = (double***)malloc(n_classes * sizeof(double**));
	//setting the arrays of feature probabilities
	for(int i=0; i < n_classes; i++){
		model->feature_probs[i] = (double**)malloc(n_features * sizeof(double*));
		//most of the other features will be set during training
	}
	return model;
}

void TrainCategoricalNB(CategoricalNaiveBayes *model, Matrix *X, Matrix *Y)
{
	int n_samples = X->rows;
	int n_features = model->n_features;
	int n_classes = model->n_classes;


	//counting the total of categorical features in the categories
	for(int i=0; i < n_features; i++){
		int max_cat = MaxCategory(X, i);
		model->n_categories = max_cat + 1; //we assume that max category is always 1
	}

	//Allocate and initializecountt arrays for each class
	//class counts  is counted for the sample i
	int *class_counts = (int*)calloc(n_classes, sizeof(int));

	//then allocate a 2d array for each class
	double ***feature_counts = (double***)malloc(n_classes * sizeof(double));
	for(int i=0; i < n_classes; i++){
		for(int x=0; x < n_features; x++){
			int n_cats = model->n_categories[x];
			feature_counts[i][x] =  (double*)calloc(n_cats, sizeof(double));
		}
	}

	//we count occurances for each sample
	for(int i=0; i < n_samples; i++){
		int cls = (int)Y->data[i][0];
		class_counts[cls]++;
		for(int x=0; x < n_features; x++){
                        int cat = (int)X->data[i][x];
                        feature_counts[cls][x][cat]++;
                }

	}

	//we compute class priors with laplace smoothing
	for(int x=0; x < n_classes; x++){
		model->class_priori->data[0][x] = (class_counts[x] + 1.0) / (n_samples + n_classes);
	}

	//allocate the results via feature probabilities in the model and compute its probabilities via laplace smoothing
	for(int i=0; i < n_classes; i++){
		for(int x=0; x < n_features; x++){
			int n_cats = model->n_categories[x];
			model->feature_probs[i][x] = (double*)malloc(n_cats * sizeof(double));

			double total_count = 0.0;
			for(int k=0; k < n_cats; k++){
				total_count += feature_count[i][x][k] + 1.0;
			}

			//now compute probabilities after laplace smoothing and sum counting has been done
			for(int k=0; k < n_cats; k++){
				model->feature_probs[i][x][k] = (feature_counts[i][x][k] + 1.0) / total_count;
			}
		}
	}


	//free temporary feature counts array
	for(int i=0; i < n_classes; i++){
		for(int x=0; x < n_features; x++){
			free(feature_counts[i][x]);
		}
		free(feature_counts[i]);
	}

	free(feature_counts);
	free(class_counts);

}




int PredictSingleCategoricalNB(CategoricalNaiveBayes *model, double *samples)
{
	int n_features = model->n_features;
        int n_classes = model->n_classes;
	int best_class = -1;
	int best_log_prob = -DBL_MAX;

	for(int i=0; i < n_classes; i++){
		double log_prob = log(model->class_priori->data[0][i]);
		for(int x=0; x < n_features; x++){
			int cat = (int)samples[x];
			if(cat < 0 || cat >= model->n_categories[x]){
				log_prob += log(1e-10);
			} else {
				log_prob += log(model->feature_probs[i][x][cat]);
			}
		}
		if(log_prob > best_log_prob){
			best_log_prob = log_prob;
			best_class = i;
		}
	}
	return best_class;
}


Matrix *PredictCategoricalNB(CategoricalNaiveBayes *model, Matrix *X)
{
	Matrix *predictions = CreateMatrix(X->rows, 1);
	for(int i=0; i < X->rows; i++){
		predictions->data[i][0] = (double)PredictSingleCategoricalNB(model, X->data[i]);
	}
	return predictions;
}

void FreeCategoricalNB(CategoricalNaiveBayes *model)
{
	FreeMatrix(model->class_priori);
	for(int i=0; i < model->n_classes; i++){
		for(int x=0; model->n_features; x++){
			FreeMatrix(model->feature_probs[i][x]);
		}
		free(model->feature_probs[i]);
	}
	free(model->feature_probs);
	free(model->n_categories);
	free(model);
}
