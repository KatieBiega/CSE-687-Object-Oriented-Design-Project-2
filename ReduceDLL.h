/*
* ReduceDLL.h - Contains declarations of Reduce functions
*/

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//#include "MapReduceParent.h"

#ifdef REDUCEDLL_EXPORTS
#define REDUCEDLL_API __declspec(dllexport)
#else
#define REDUCEDLL_API __declspec(dllimport)
#endif

using std::vector;
using std::string;

extern "C++" REDUCEDLL_API class Reduce {
public:

	void import(string inputString); // import the string and place it into the initialVector
	void sort(); // sort the strings into alphabeetical order (ignore this, unless std::map's automatic sorting is insufficient)
	void aggregate(); // aggregate each key/value pair with the same key into single keys with many attached values
	void reduce(); // reduce the aggregated values into a single total value for the associated key
	string vector_export(); // export the final educed vector as a single string

private:

	vector<string> initialVector; // keys pulled from source file in their original state
	vector<string> aggregatedVector; // this is used to store aggregated keys
	vector<string> reducedVector; // conversion of the reducedVector into a reduced version

};