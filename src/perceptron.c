#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#include <perceptron.h>
#include <activations.h>
#include <mathutils.h>

/* Makes a perceptron given its input dimension and its activation function */
/* and initialize its weights to random values between -1 and 1				*/
perceptron_t *perceptron_create(size_t dim, activation_t a){
	perceptron_t *p = (perceptron_t*) malloc(sizeof(perceptron_t));
	p->dim = dim;
	p->w = (float*) malloc((p->dim + 1) * sizeof(float)); /* +1 for the bias */
	
	srand(time(NULL));
	for(size_t i = 0; i < p->dim + 1; i++)
		p->w[i] = 2.0f*((rand() / (float)RAND_MAX) - 1.0f);
	
	p->activation = a;
	return p;
}

void perceptron_destroy(perceptron_t *p){
	free(p->w);
	free(p);	
}

/* Computes the percentage of the incorrect predictions */
static float compute_LS(perceptron_t *p, labeled_dataset_t *data){
	float LS = 0;
	for(size_t i = 0; i < data->len; i++){
		if(perceptron_predict(p, data->x + i*p->dim) != data->y[i])
			LS += 1;
	}
	return LS / (float)(data->len);
}

/* Note : All learning algorithms implemented here, */
/*	only work with sign as activation 	*/
/* TODO : extend them to work on other activations */


/* Note : Please use THE sign function defined in activations.h */
/*	Do NOT try to implement your own, it won't work, */
/*	since the assertion uses the one in activations.h */


/* One iteration of the Perceptron Learning algorithm */
static void PLA_weight_update(perceptron_t *p, labeled_dataset_t *data, size_t *t){
	for(size_t i = 0; i < data->len; i++){
		if(((perceptron_predict(p, data->x + i*p->dim)) != (data->y[i]))){		
			for(size_t j = 0; j < p->dim; j++)
				p->w[j] += data->y[i] * data->x[i*p->dim + j];
			p->w[p->dim] += data->y[i];
			++(*t);
		}
	}
}


/* Trains a perceptron on a dataset using Perceptron Learning algorithm */
size_t perceptron_learn(perceptron_t *p, labeled_dataset_t *data){
	assert(p->activation == sign);
	size_t t = 0;
	float LS;
	LS = compute_LS(p, data);
	while(LS != 0){
		PLA_weight_update(p, data, &t);
		LS = compute_LS(p, data);
	}
	return t;
}

/* One iteration of Pocket algorithm */
static void pocket_weight_update(perceptron_t *p, labeled_dataset_t *data, size_t *t){
	float LS, LS_before, *w_before;
	w_before = (float*) malloc((p->dim+1) * sizeof(float));
	
	for(size_t i = 0; i < data->len; i++){
		if(((perceptron_predict(p, data->x + i*p->dim)) != (data->y[i]))){
			/* Keep a backup of the weights and the LS */
			LS_before = compute_LS(p, data);		
			memcpy(w_before, p->w, (p->dim+1)*sizeof(float));
			
			/* Change the weights as in the PLA */
			for(size_t j = 0; j < p->dim; j++)
				p->w[j] += data->y[i] * data->x[i*p->dim + j];
			p->w[p->dim] += data->y[i];
			
			/* Compare the new LS with the previous one */
			/* If it is worse than go back to the previous weights */
			LS = compute_LS(p, data);
			if(LS > LS_before)
				memcpy(p->w, w_before, (p->dim+1)*sizeof(float));
							
			
		}
		++(*t);
	}
	free(w_before);
}

size_t perceptron_pocket_learn(perceptron_t *p, labeled_dataset_t *data, size_t max_iterations){
	assert(p->activation == sign);	
	size_t t = 0;
	while(compute_LS(p, data) && t < max_iterations)
		pocket_weight_update(p, data, &t);	
	return t;
}

/* One iteration of Adaline (Delta rule) algorithm */
static void adaline_weight_update(perceptron_t *p, labeled_dataset_t *data, size_t *t){
	for(size_t i = 0; i < data->len; i++){
		if(((perceptron_predict(p, data->x + i*p->dim)) != (data->y[i]))){
			float e = data->y[i];
			e -= perceptron_predict(p, data->x + i*p->dim);
			for(size_t j = 0; j < p->dim; j++)
				p->w[j] += 2 * e * data->x[i*p->dim + j];
			p->w[p->dim] += 2*e;
		}
	}
	++(*t);
}
/* I noticed this is pretty much the same code in pocket function */
/* TODO: DRY! Do not repeat yourself */
size_t perceptron_adaline_learn(perceptron_t *p, labeled_dataset_t *data, size_t max_iterations){
	assert(p->activation == sign);
	size_t t = 0;	
	while(t < max_iterations)
		adaline_weight_update(p, data, &t);

	return t;
}

/* Predicts an input's class */
float perceptron_predict(perceptron_t *p, float *x){
	float z = vec_dot(p->dim, p->w, x) + p->w[p->dim];
	return p->activation(z);
}

