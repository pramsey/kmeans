# kmeans

A simple C routine for generic K-means calculations.

All the K-means code I found was either too complex, or bound to assumptions about 2-dimensionality, or n-dimensionality, and I really just wanted something like `qsort()` that I could pass a list of pointers and some support functions and get a clustering. So I took the code from the [PostgreSQL kmeans](https://github.com/umitanuki/kmeans-postgresql) implementation and made it a little more generic.

This code will probably only get used in [PostGIS](http://postgis.net), but in case someone needs a simple drop-in K-means implementation, now there's one here.

## Usage

Best to read the [kmeans.h](kmeans.h) file and the two example programs for usage.

Everything is very easy except figuring out the initial set of centers to feed the algorithm. Some rules of thumb:

* No matter what, use actual values from your set, not averages or other medial values. If you use an actual value, you're guaranteed that in the first assignment of clusters, at least one data point will match your cluster. If you don't, you can end up with orphaned clusters.
* Just picking K random entries from your input data is probably not the worst thing you could do.
* Other approaches include partitioning the space and calculating seeds evenly in the space, then choosing the closest real data points to each of the calculated seeds. This results in the nicest outputs I have seen so far.

## Multi-threading

The calculations are embarrasingly parallel, so there is an optional multi-threading switch you can turn on. You have to select your maximum amount of parallelism at compile time, I haven't put in any automatic detection, in aid of keeping things simple. Because the threading uses `pthread.h` it's not portable to Windows without some kind of a POSIX environment.
