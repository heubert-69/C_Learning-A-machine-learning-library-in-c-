#include<stdlib.h>
#include<math.h>
#include<float.h>
#include "../include/bayes_g.h"


//Helper Functions

static double GaussianPDF(double *X, double *mean, double *var, int d)
{
	double log_det_sum = 0.0, sum = 0.0;
	for(int i=0; i < d; i++){
		double diff = X[i] - mean[i];
		//later
	}

}

BayesianGaussianMixture* CreateBGM(int n_components, int n_features, double alpha, double beta)
{
	BayesianGaussianMixture *model = (BayesianGuassianMixture*)malloc(sizeof(BayesianGaussianMixture));

	model->n_components = n_components;
	model->n_features = n_features;
	model->alpha = alpha;
	model->beta = beta;
	model->weights = (double)calloc(n_components, sizeof(double));
	model->means = (Matrix)calloc(n_components, sizeof(Matrix*));
	model->variances = (Matrix)calloc(n_components, sizeof(Matrix*));
	//creating Matrices for means and variances
	for(int i=0; i < n_components; i++){
		model->means[i] = CreateMatrix(1, n_features);
		model->variances[i] = CreateMatrix(1, n_features);
	}

	return model;
}

void TrainBGM(BayesianGaussianMixture *model, Matrix *X, int max_iter)
{
	int n = X->rows, d = model->n_features, k = model->n_components;
	double **responsibilities = (double)calloc(n, sizeof(double*));
	//initializing 2d array
	for(int i=0; i < n; i++){
		responsibilities[i] = calloc(k, sizeof(double));
	}

	for (int iter = 0; iter < max_iter; iter++) {
        // E-step
        for (int i = 0; i < n; i++) {
            double norm = 0;
            for (int j = 0; j < k; j++) {
                double *mu = model->means[j]->data[0];
                double *var = model->variances[j]->data[0];
                responsibilities[i][j] = model->weights[j] * gaussian_pdf(X->data[i], mu, var, d);
                norm += responsibilities[i][j];
            }
            for (int j = 0; j < k; j++) responsibilities[i][j] /= norm + 1e-10;
        }

        // M-step
        for (int j = 0; j < k; j++) {
            double Nk = model->alpha - 1.0;
            for (int i = 0; i < n; i++) Nk += responsibilities[i][j];

            model->weights[j] = (Nk + 1e-10) / (n + k * (model->alpha - 1));

            for (int f = 0; f < d; f++) {
                double mean = 0, var = 0;
                for (int i = 0; i < n; i++) {
                    mean += responsibilities[i][j] * X->data[i][f];
                }
                mean /= (Nk + 1e-10);

                for (int i = 0; i < n; i++) {
                    double diff = X->data[i][f] - mean;
                    var += responsibilities[i][j] * diff * diff;
                }
                var = var / (Nk + 1e-10) + 1e-6;

                model->means[j]->data[0][f] = mean;
                model->variances[j]->data[0][f] = var;
            }
        }
    }

    for (int i = 0; i < n; i++) free(responsibilities[i]);
    free(responsibilities);
}

int predict_bayesian_gmm(BayesianGMM *model, double *x) {
    int best = 0;
    double best_prob = -DBL_MAX;
    for (int j = 0; j < model->n_components; j++) {
        double p = model->weights[j] * gaussian_pdf(x, model->means[j]->data[0], model->variances[j]->data[0], model->n_features);
        if (p > best_prob) {
            best_prob = p;
            best = j;
        }
    }
    return best;
}

void free_bayesian_gmm(BayesianGMM *model) {
    for (int i = 0; i < model->n_components; i++) {
        free_matrix(model->means[i]);
        free_matrix(model->variances[i]);
    }
    free(model->means);
    free(model->variances);
    free(model->weights);
    free(model);
}











