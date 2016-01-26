/*-------------------------------------------------------------------------
*
* kmeans.c
*    Generic k-means implementation
*
* Copyright (c) 2016, Paul Ramsey <pramsey@cleverelephant.ca>
*
*------------------------------------------------------------------------*/

#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "kmeans.h"

static void
update_r(kmeans_config *config)
{
	int i;
	
	for (i = 0; i < config->num_objs; i++)
	{
		double distance, curr_distance;
		int cluster, curr_cluster;
		Pointer obj;
		
		assert(config->objs != NULL);
		assert(config->num_objs > 0);
		assert(config->centers);
		assert(config->clusters);
		
		obj = config->objs[i];
		
		/* 
		* Don't try to cluster NULL objects, just add them
		* to the "unclusterable cluster"
		*/
		if (!obj)
		{
			config->clusters[i] = KMEANS_NULL_CLUSTER;
			continue;
		}
		
		/* Initialize with distance to first cluster */
		curr_distance = (config->distance_method)(obj, config->centers[0]);
		curr_cluster = 0;
		
		/* Check all other cluster centers and find the nearest */
		for (cluster = 1; cluster < config->k; cluster++)
		{
			distance = (config->distance_method)(obj, config->centers[cluster]);
			if (distance < curr_distance)
			{
				curr_distance = distance;
				curr_cluster = cluster;
			}
		}
		
		/* Store the nearest cluster this object is in */
		config->clusters[i] = curr_cluster;
	}
}

static double
J(kmeans_config *config)
{
	int i;
	double sum = 0.0;
	for (i = 0; i < config->num_objs; i++)
	{
		/* Don't bother adding distances for objects in the NULL category */
		if (config->objs[i] && config->clusters[i] != KMEANS_NULL_CLUSTER)
			sum += (config->distance_method)(config->objs[i], config->centers[config->clusters[i]]);
	}
	return sum;
}

static void
update_means(kmeans_config *config)
{
	/* Allocate enough space for all objects, just in case */
	Pointer * objs = calloc(config->num_objs, sizeof(Pointer));
	int i;
	int cluster;
	
	for (cluster = 0; cluster < config->k; cluster++) 
	{
		/* Set up a clean list to hold the cluster object references */
		int num_objs = 0;
		memset(objs, 0, sizeof(Pointer) * config->num_objs);
		
		/* Find all the objects currently in this cluster */
		for (i = 0; i < config->num_objs; i++)
		{
			if (config->clusters[i] == cluster)
				objs[num_objs++] = config->objs[i];
		}
		
		/* Update the centroid for this cluster */
		(config->centroid_method)(objs, num_objs, config->centers[cluster]);
	}
	
	free(objs);
}

kmeans_result
kmeans(kmeans_config *config)
{
	double	target, new_target;
	int iterations = 0;

	/* Zero out cluster numbers, just in case user forgets */
	memset(config->clusters, 0, sizeof(int)*config->num_objs);
	
	/* Set default max iterations if necessary */
	if (!config->max_iterations)
		config->max_iterations = KMEANS_MAX_ITERATIONS;
	
	/*
	 * initialize purpose value. At this time, r doesn't mean anything
	 * but it's ok; just fill target by some value.
	 */
	target = J(config);
	while (1)
	{
		update_r(config);
		update_means(config);
		new_target = J(config);
		// kmeans_debug(mean, dim, k);
		/*
		 * if all the classification stay, diff must be 0.0,
		 * which means we can go out!
		 */
		if (fabs(target - new_target) < DBL_EPSILON)
			return KMEANS_OK;
		
		if (iterations > config->max_iterations)
			return KMEANS_EXCEEDED_MAX_ITERATIONS;
		
		iterations++;
		target = new_target;
	}

	return KMEANS_ERROR;
}


