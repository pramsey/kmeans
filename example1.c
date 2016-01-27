#include <math.h>
#include <stdio.h>

#include "kmeans.h"


static double d_distance(const Pointer a, const Pointer b)
{
	double da = *((double*)a);
	double db = *((double*)b);
	return fabs(da - db);
}

static void d_centroid(const Pointer * objs, const int * clusters, size_t num_objs, int cluster, Pointer centroid)
{
	int i;
	int num_cluster = 0;
	double sum = 0;
	double **doubles = (double**)objs;
	double *dcentroid = (double*)centroid;

	if (num_objs <= 0) return;

	for (i = 0; i < num_objs; i++)
	{
		/* Only process objects of interest */
		if (clusters[i] != cluster)
			continue;

		sum += *(doubles[i]);
		num_cluster++;
	}
	if (num_cluster)
	{
		sum /= num_cluster;
		*dcentroid = sum;
	}
	return;
}

int
main(int nargs, char **args)
{
	double v[10] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
	double c[2] = {2.0, 5.0};
	kmeans_config config;
	kmeans_result result;
	int i;

	config.num_objs = 11;
	config.k = 2;
	config.max_iterations = 100;
	config.distance_method = d_distance;
	config.centroid_method = d_centroid;

	config.objs = calloc(config.num_objs, sizeof(Pointer));
	config.centers = calloc(config.k, sizeof(Pointer));
	config.clusters = calloc(config.num_objs, sizeof(int));

	/* populate objs */
	for (i = 0; i < config.num_objs - 1; i++)
	{
		config.objs[i] = &(v[i]);
	}
	config.objs[10] = NULL;
	// config.objs = objs;

	/* populate centroids */
	for (i = 0; i < config.k; i++)
	{
		config.centers[i] = &(c[i]);
	}

	/* run k-means */
	result = kmeans(&config);

	/* print result */
	for (i = 0; i < config.num_objs; i++)
	{
		if (config.objs[i])
			printf("%g [%d]\n", *((double*)config.objs[i]), config.clusters[i]);
		else
			printf("NN [%d]\n", config.clusters[i]);
	}

	free(config.objs);
	free(config.clusters);
	free(config.centers);

	return 0;
}


// typedef struct kmeans_config
// {
// 	kmeans_distance_method object_distance;
// 	kmeans_centroid_method object_centroid;
// 	size_t num_objs;
// 	const object *objs;
// 	unsigned int k;
// 	unsigned int max_iterations;
// } kmeans_config;
