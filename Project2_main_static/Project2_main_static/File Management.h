#ifndef FILE_MANAGEMENT
#define FILE_MANAGEMENT

#include <string>

class FileManagement {
public:
    FileManagement(const std::string& inputDir, const std::string& outputDir, const std::string& tempDir);

    std::string ReadFromTempFile(const std::string& fileName);
    std::string ReadAllFiles();
    void WriteToTempFile(const std::string& fileName, const std::string& data);
    void WriteToOutputFile(const std::string& fileName, const std::string& data);

    template<typename T>
    void WriteToTempOrOutputFile(const std::string& fileName, const std::string& data);

private:
    std::string inputDirectory;
    std::string outputDirectory;
    std::string tempDirectory;
};

template<typename T>
void FileManagement::WriteToTempOrOutputFile(const std::string& fileName, const std::string& data) {
    T fileStream((tempDirectory + fileName).c_str());
    if (fileStream.is_open()) {
        fileStream << data;
        fileStream.close();
    }
}

#endif

