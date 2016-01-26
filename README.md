# kmeans

A simple C routine for generic K-means implementation.

All the K-means code I found was either too complex, or bound to assumptions about 2-dimensionality, or n-dimensionality, and I really just wanted something like `qsort()` that I could pass a list of pointers and some support functions and get a clustering. So I took the code from the [PostgreSQL kmeans](https://github.com/umitanuki/kmeans-postgresql) implementation and made it a little more generic.

This code will probably only get used in PostGIS, but in case someone needs a simple drop-in K-means implementation, now there's one here.

## Usage

Best to read the [kmeans.h](kmeans.h) file and the two example programs for usage.