#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>

#include "ReduceDLL.h"
#include "MapDLL.h"
#include "File Management.h"

#include <Windows.h>

using std::stringstream;
using std::vector;
using std::string;
using std::to_string;
using std::getline;
using std::cout;
using std::cin;

typedef void (*MapFunction)(const char* input, char* output);
typedef void (*ReduceFunction)(const char* input, char* output);

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

    HMODULE mapDLL = LoadLibrary("Map.dll");
    HMODULE reduceDLL = LoadLibrary("Reduce.dll");
    HMODULE mapreduceparentDLL = LoadLibrary("MapReduceParent.dll");

    if (mapDLL == NULL || reduceDLL == NULL) {
        cout << "Failed to load DLLs." << std::endl;
        return 1;
    }

    MapFunction mapFunction = (MapFunction)GetProcAddress(mapDLL, "map_function");
    ReduceFunction reduceFunction = (ReduceFunction)GetProcAddress(reduceDLL, "reduce_function");

    if (mapFunction == NULL || reduceFunction == NULL) {
        cout << "Failed to get function pointers from DLLs." << std::endl;
        FreeLibrary(mapDLL);
        FreeLibrary(reduceDLL);
        return 1;
    }

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

    //Read all files into single string and pass to Map class
    //cout << "FileManagement Class initialized.\n";
    fileString = FileManage.ReadAllFiles();
    //cout << "All files read.\n";
    Mapping.map(fileString);
    //cout << "String from files passed to map function.\n";

    //Write mapped output string to intermediate file 
    mapped_string = Mapping.mapped_export();
    //cout << "Mapping complete; exporting resulting string.\n";
    FileManage.WriteToTempFile(tempFilename, mapped_string);
    //cout << "String from mapping written to temp file.\n";

    //Read from intermediate file and pass data to Reduce class
    tempFileContent = FileManage.ReadFromTempFile(tempFilename);
    //cout << "New string read from temp file.\n";
    Reduction.import(tempFileContent);
    cout << "String imported by reduce class function and placed in vector.\n";
    Reduction.sort();
    //cout << "Vector sorted.\n";
    Reduction.aggregate();
    //cout << "Vector aggregated.\n";
    Reduction.reduce();
    //cout << "Vector reduced.\n";
    reduced_string = Reduction.reduce_export();
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
