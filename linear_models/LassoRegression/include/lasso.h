#ifndef LASSO_H
#define LASSO_H



typedef struct {

	Matrix *weights
	double bias;
	double alpha;


}; LassoRegression;

LassoRegression* CreateLassoRegression(int features, double alpha);
LassoRegression* TrainLassoRegression(LassoRegression *model, Matrix *X, Matrix *Y, double learning_rate, int epochs);
LassoRegression* PredictLassoRegression(LassoRegression *model, Matrix *X);


//acts as a destructor
void FreeLassoRegression(LassoRegression *model);


#endif //LASSO_H
