#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>

#include "../../mapDLL/MapDLL/MapInterface.h"
#include "../../ReduceDLL/ReduceDLL/ReduceInterface.h"
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

typedef MapInterface* (*CREATE_MAPPER) ();
typedef ReduceInterface* (*CREATE_REDUCER) ();

int main() {
    cout << "Program started. Press any key to continue...\n";
    cin;

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

    HMODULE mapDLL = LoadLibrary("MapDLL.dll"); // load dll for map functions
    HMODULE reduceDLL = LoadLibrary("ReduceDLL.dll"); // load dll for library functions

    if (mapDLL == NULL) // exit main function if mapDLL is not found
    {
        cout << "Failed to load mapDLL." << endl;
        return 1;
    }

    if (reduceDLL == NULL) // exit main function if reduceDLL is not found
    {
        cout << "Failed to load reduceDLL." << endl;
        return 1;
    }


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

    cout << "==== MAP & REDUCE ====\n\n"; // add title

    cout << "Enter the input directory: "; // prompt user to input i/o directories
    cin >> inputDirectory;
    cout << "Enter the output directory: ";
    cin >> outputDirectory;
    cout << "Enter the temp directory: ";
    cin >> tempDirectory;

    //WORKFLOW//
    

    CREATE_MAPPER mapperPtr = (CREATE_MAPPER)GetProcAddress(mapDLL, "CreateMap"); // create pointer to function to create new Map object
    MapInterface* pMapper = mapperPtr();

    CREATE_REDUCER reducerPtr = (CREATE_REDUCER)GetProcAddress(reduceDLL, "CreateReduce");  // create pointer to function to create new Reduce object
    ReduceInterface* pReducer = reducerPtr();


    FileManagement FileManage(inputDirectory, outputDirectory, tempDirectory); //Create file management class based on the user inputs
    cout << "FileManagement Class initialized.\n";



    fileString = FileManage.ReadAllFiles();     //Read all files into single string and pass to Map class
    cout << "All files read.\n";
    cout << "String from files passed to map function.\n";
    // 
    cout << "Beginning map function\n";
    pMapper->map(fileString);


    
    cout << "Mapping complete; exporting resulting string.\n";
    mapped_string = pMapper->vector_export();     //Write mapped output string to intermediate file 

    FileManage.WriteToTempFile(tempFilename, mapped_string);
    cout << "String from mapping written to temp file.\n";

    //Read from intermediate file and pass data to Reduce class
    tempFileContent = FileManage.ReadFromTempFile(tempFilename);
    cout << "New string read from temp file.\n";
    
    pReducer->import(tempFileContent);
    cout << "String imported by reduce class function and placed in vector.\n";

    pReducer->sort();
    cout << "Vector sorted.\n";

    pReducer->aggregate();
    cout << "Vector aggregated.\n";

    pReducer->reduce();
    cout << "Vector reduced.\n";

    reduced_string = pReducer->vector_export();
    cout << "Vector exported to string.\n";

    //Sorted, aggregated, and reduced output string is written into final output file
    FileManage.WriteToOutputFile(outputFilename, reduced_string);
    cout << "string written to output file.\n";

    FileManage.WriteToOutputFile(successFilename, successString);
    cout << "Success.\n";


    FreeLibrary(mapDLL);
    FreeLibrary(reduceDLL);

    return 0;
}
