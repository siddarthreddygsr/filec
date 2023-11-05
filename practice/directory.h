#include <iostream>
#include "files.h"
using namespace std;

struct Directory {
    std::string name;
    std::vector<File> files;

    Directory(std::string n) : name(n) {}

    int checkExistence(const std::string& fileName) {
        for (File& file : files) {
            if (file.name == fileName) {
                return 1;
            }
        }
        return 0;
    }

    void addFile(const std::string& fileName, const std::string& content) {
        if (checkExistence(fileName)) {
            cout << "File already exists. Try updating it." << std::endl;
            return;
        }
        // If the file doesn't exist, add it with the initial version
        File newFile(fileName);
        newFile.addVersion(content);
        files.push_back(newFile);
        std::cout << "File '" << fileName << "' created with the initial version." << std::endl;
    }

    void UpdateFile(const std::string& fileName, const std::string& content) {
        for (File& file : files) {
            if (file.name == fileName) {
                file.addVersion(content);
                std::cout << "File '" << fileName << "' updated with a new version." << std::endl;
                return;
            }
        }
        // If the file doesn't exist, add it with the initial version
        File newFile(fileName);
        newFile.addVersion(content);
        files.push_back(newFile);
        std::cout << "File '" << fileName << "' not found. Created with the initial version." << std::endl;
    }

    // Function to mark a file for deletion by name
    void markFileForDeletion(const std::string& fileName) {
        for (File& file : files) {
            if (file.name == fileName) {
                file.metadata.deletionMark = true;
                std::cout << "File '" << fileName << "' marked for deletion." << std::endl;
                return;
            }
        }
        std::cout << "File not found: '" << fileName << "'" << std::endl;
    }
};