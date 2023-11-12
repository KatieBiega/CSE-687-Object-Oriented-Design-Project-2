#pragma once
#include<iostream>
#include<string>
#include<vector>

using namespace std;
using std::string;
using std::vector;

class MapReduceParent
{
public:
    virtual ~MapReduceParent() { ; }
    virtual void map();
    virtual string vector_export();
	virtual void import(string inputString); // import the string and place it into the initialVector
	virtual void sort(); // sort the strings into alphabeetical order (ignore this, unless std::map's automatic sorting is insufficient)
	virtual void aggregate(); // aggregate each key/value pair with the same key into single keys with many attached values
	virtual void reduce(); // reduce the aggregated values into a single total value for the associated key
};
