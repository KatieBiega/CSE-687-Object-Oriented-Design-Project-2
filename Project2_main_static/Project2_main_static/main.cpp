#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>

//#include "ReduceDLL.h"
#include "BaseInterface.h"
#include "File Management.h"

#include <Windows.h>

using std::stringstream;
using std::vector;
using std::string;
using std::to_string;
using std::getline;
using std::cout;
using std::cin;
using std::endl;
/*
typedef void (*MapFunction)(const char* input, char* output);
typedef void (*ReduceFunction)(const char* input, char* output);
*/
typedef MapReduceObject* (*CREATE_MAPPER) ();

int main() {
    string fileName = "";
    string fileString = "";
    string inputDirectory = "";
    string outputDirectory = "";
    string tempDirectory = "";

    string mapped_string;
    string tempFilename = "TempFile.txt";
    string tempFileContent;
    string reduced_string;
    string outputFilename = "Final_OutputFile.txt";
    string successString = "";
    string successFilename = "SUCCESS.txt";

    HMODULE mapDLL = LoadLibrary("MapDLL.dll");
    HMODULE reduceDLL = LoadLibrary("ReduceDLL.dll");

    if (mapDLL == NULL) {
        cout << "Failed to load mapDLL." << endl;
        return 1;
    }

    if (reduceDLL == NULL) {
        cout << "Failed to load reduceDLL." << endl;
        return 1;
    }


    CREATE_MAPPER mapperPtr (CREATE_MAPPER)GetProcAddress(DLL, "CreateMap");
    MapReduceParent* pMapper = mapperPtr(3, 4);
    pMapper->map();
    cout << "Area is:" << pShape->GetArea();

    

    /*
    MapFunction map = (MapFunction)GetProcAddress(mapDLL, "map"); // find the map function
    if (map == NULL) {
        cout << "Failed to get function pointer for function 'map'." << endl;
        FreeLibrary(mapDLL);
        FreeLibrary(reduceDLL);
        return 1;
    }

    MapFunction mapExport = (MapFunction)GetProcAddress(mapDLL, "vector_export"); // find the map function
    if (mapExport == NULL) {
        cout << "Failed to get function pointer for function 'mapExport'." << endl;
        FreeLibrary(mapDLL);
        FreeLibrary(reduceDLL);
        return 1;
    }

    ReduceFunction sort = (ReduceFunction)GetProcAddress(reduceDLL, "sort");
    if (sort == NULL) {
        cout << "Failed to get function pointer for function 'sort'." << endl;
        FreeLibrary(mapDLL);
        FreeLibrary(reduceDLL);
        return 1;
    }

    ReduceFunction aggregate = (ReduceFunction)GetProcAddress(reduceDLL, "aggregate");
    if (aggregate == NULL) {
        cout << "Failed to get function pointer for function 'aggregate'." << endl;
        FreeLibrary(mapDLL);
        FreeLibrary(reduceDLL);
        return 1;
    }


    ReduceFunction reduce = (ReduceFunction)GetProcAddress(reduceDLL, "reduce");
    if (reduce == NULL) {
        cout << "Failed to get function pointer for function 'reduce'." << endl;
        FreeLibrary(mapDLL);
        FreeLibrary(reduceDLL);
        return 1;
    }

    ReduceFunction reduceExport = (ReduceFunction)GetProcAddress(reduceDLL, "vector_export"); // find the function for vector_export(reduce)
    if (reduceExport == NULL) {
        cout << "Failed to get function pointer for function 'reduceExport'." << endl;
        FreeLibrary(mapDLL);
        FreeLibrary(reduceDLL);
        return 1;
    }
    */


    cout << "==== MAP & REDUCE ====\n\n";

    cout << "Enter the input directory: ";
    cin >> inputDirectory;
    cout << "Enter the output directory: ";
    cin >> outputDirectory;
    cout << "Enter the temp directory: ";
    cin >> tempDirectory;

    //WORKFLOW//
    //Create file management class based on the user inputs
    FileManagement FileManage(inputDirectory, outputDirectory, tempDirectory);
    extern Map mapping;
    extern Reduce reduction;

    //Read all files into single string and pass to Map class
    //cout << "FileManagement Class initialized.\n";
    fileString = FileManage.ReadAllFiles();
    //cout << "All files read.\n";
    mapping.map(fileString);
    //cout << "String from files passed to map function.\n";

    //Write mapped output string to intermediate file 
    mapped_string = mapping.mapExport();
    //cout << "Mapping complete; exporting resulting string.\n";
    FileManage.WriteToTempFile(tempFilename, mapped_string);
    //cout << "String from mapping written to temp file.\n";

    //Read from intermediate file and pass data to Reduce class
    tempFileContent = FileManage.ReadFromTempFile(tempFilename);
    //cout << "New string read from temp file.\n";
    reduction.import(tempFileContent);
    cout << "String imported by reduce class function and placed in vector.\n";
    sort();
    //cout << "Vector sorted.\n";
    aggregate();
    //cout << "Vector aggregated.\n";
    reduce();
    //cout << "Vector reduced.\n";
    reduced_string = reduce_export();
    //cout << "Vector exported to string.\n";

    //Sorted, aggregated, and reduced output string is written into final output file
    FileManage.WriteToOutputFile(outputFilename, reduced_string);
    //cout << "string written to output file.\n";
    FileManage.WriteToOutputFile(successFilename, successString);
    cout << "Success.\n";


    FreeLibrary(mapDLL);
    FreeLibrary(reduceDLL);

    return 0;
}
