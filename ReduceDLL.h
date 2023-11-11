/*
* ReduceDLL.h - Contains declarations of Reduce functions
*/

#pragma once
#include <string>
#include "MapReduceParent.h"

#ifdef REDUCEDLL_EXPORTS
#define REDUCEDLL_API __declspec(dllexport)
#else
#define REDUCEDLL_API __declspec(dllimport)
#endif

using std::string;

// import the string and place it into the initialVector
extern "C++" REDUCEDLL_API void import(string inputString);

// sort the strings into alphabeetical order (ignore this, unless std::map's automatic sorting is insufficient)
extern "C++" REDUCEDLL_API void sort();

// aggregate each key/value pair with the same key into single keys with many attached values
extern "C++" REDUCEDLL_API void aggregate();

// reduce the aggregated values into a single total value for the associated key
extern "C++" REDUCEDLL_API void reduce();

// export the final educed vector as a single string
extern "C++" REDUCEDLL_API string vector_export();
