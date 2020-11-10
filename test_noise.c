#include <stdio.h>
#include <perceptron.h>

int main(int argc, char **argv){
	(void) argc;
	(void) argv;
	
	/* Points' coordinates */
	float x[] = {
		0.0f, 2.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 2.0f,
		1.0f, 3.0f,
		1.0f, 3.5f,
		2.0f, 1.0f,
		2.0f, 2.0f,
		2.0f, 3.0f,
		2.0f, 3.5f,//
		3.0f, 0.0f,
		3.0f, 2.0f,
		2.7f, 3.3f,//
		4.0f, 1.0f,
		
		1.0f, 4.0f,
		2.0f, 4.0f,
		2.0f, 5.0f,
		2.5f, 3.0f,
		2.5f, 4.0f,
		3.0f, 3.0f,
		3.0f, 4.5f,
		3.0f, 6.0f,
		4.0f, 2.5f,
		4.0f, 3.5f,
		4.0f, 5.0f,
		5.0f, 2.0f,
		5.0f, 3.0f,
		5.0f, 4.0f,
	};
	/* Points' classes */
	float y[] = {
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		
		-1.0f,
		-1.0f,
		-1.0f,
		-1.0f,
		-1.0f,
		-1.0f,
		-1.0f,
		-1.0f,
		-1.0f,
		-1.0f,
		-1.0f,
		-1.0f,
		-1.0f,
		-1.0f
	};
	
	/* Make a new dataset with the previous arrays */
	labeled_dataset_t *dataset = labeled_dataset_create(2, 28);
	dataset->x = x;
	dataset->y = y;
	
	/* Make a new perceptron and train it with the dataset */
	perceptron_t *p = perceptron_create(2, &sign);
	
	/* You can switch between pocket and adaline here */
#if 1
	perceptron_adaline_learn(p, dataset, 15000);
#else
	perceptron_pocket_learn(p, dataset, 15000);
#endif

	/* Print the final weights */
	printf("\n Final weights :");
	for(size_t j = 0; j < p->dim+1; j++)
		printf("\t%.5f",  (float)(p->w[j]));
	printf("\n");
	
	/* Test the perceptron */
	float in[2] = {4, 4};
	printf("\n%f", (float)perceptron_predict(p, in));

	/* Generate the output files for plotting */
	FILE *out;
	out = fopen("data.in", "w");
	for(size_t i = 0; i < 28; i++){
		fprintf(out, "%.5lf %.5lf %.5lf\n", x[2*i], x[2*i+1], y[i]);
	}
	fclose(out);	
	out = fopen("weights.in", "w");
	for(size_t i = 0; i < 3; i++){
		fprintf(out, "w%ld = %.5lf\n", i, p->w[i]);
	}
	fclose(out);
}

