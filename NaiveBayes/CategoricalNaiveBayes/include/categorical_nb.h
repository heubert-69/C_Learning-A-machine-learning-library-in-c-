#ifndef CATEGORICAL_NB_H
#define CATEGORICAL_NB_H

typedef struct {
	int n_features;
	int n_classes;
	int *n_categories;
	Matrix *class_priori;
	double ***feature_probs;
} CategoricalNaiveBayes;

CategoricalNaiveBayes* CreateCategoricalNB(int n_features, int n_classes);
void TrainCategoricalNB(CategoricalNaiveBayes *model, Matrix *X, Matrix *Y);
int PredictSingleCategoricalNB(CategoricalNaiveBayes *model, double *samples);
Matrix *PredictCategoricalNB(CategoricalNaiveBayes *model, Matrix *X);
void FreeCategoricalNB(CategoricalNaiveBayes *model);

#endif //CATEGORICAL_NB_H
