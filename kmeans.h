#include <stdlib.h>

#define KMEANS_NULL_CLUSTER -1
#define KMEANS_MAX_ITERATIONS 1000
/*
* Simple k-means implementation for arbitrary data structures
*
* Since k-means partitions based on inter-object "distance" the same
* machinery can be used to support any object type that can calculate a 
* "distance" between pairs.
*
* To use the k-means infrastructure, just fill out the kmeans_config
* structure and invoke the kmeans() function.
*/

typedef void * Pointer;

typedef enum {
	KMEANS_OK,
	KMEANS_EXCEEDED_MAX_ITERATIONS,
	KMEANS_ERROR
} kmeans_result;

/* Prototype for the distance calculating function */
typedef double (*kmeans_distance_method) (const Pointer a, const Pointer b);

/* Prototype for the centroid calculating function */
typedef void (*kmeans_centroid_method) (const Pointer * objs, size_t num_objs, Pointer centroid);

typedef struct kmeans_config
{
	/* Function returns the "distance" between any pair of objects */
	kmeans_distance_method distance_method;
	
	/* Function returns the "centroid" of a collection of objects */
	kmeans_centroid_method centroid_method;
	
	/* An array of objects to be analyzed. User allocates this array */
	/* and is responsible for freeing it. */
	/* For objects that are not capable of participating in the distance */
	/* calculations, but for which you still want included in the process */
	/* (for examples, database nulls, or geometry empties) use a NULL */
	/* value in this list. All NULL values will be returned in the */
	/* KMEANS_NULL_CLUSTER. */
	Pointer * objs;
	
	/* Number of objects in the preceding array */
	size_t num_objs;
	
	/* An array of inital centers for the algorithm */
	/* Can be randomly assigned, or using proportions, */
	/* unfortunately the algorithm is sensitive to starting */
	/* points, so using a "better" set of starting points */
	/* might be wise. User allocates and is responsible for freeing. */
	Pointer * centers;
	
	/* Number of means we are calculating, length of preceding array */
	unsigned int k;
	
	/* Maximum number of times to iterate the algorithm, or 0 for */
	/* library default */
	unsigned int max_iterations;
	
	/* Array to fill in with cluster numbers. User allocates and frees. */
	int * clusters;
	
} kmeans_config;

/* This is where the magic happens. */
kmeans_result kmeans(kmeans_config *config);

