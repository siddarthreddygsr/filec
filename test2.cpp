#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <map>

struct FileMetadata {
    bool deletionMark = false;
    std::time_t lastUpdated = std::time(0);
};

struct FileVersion {
    std::time_t timestamp;
    std::string content;
};

struct File {
    std::string name;
    FileMetadata metadata;
    std::vector<FileVersion> versions;
    std::vector<int> allocatedBlocks;  // Store allocated data blocks

    File(std::string n) : name(n) {}

    void addVersion(const std::string& content) {
        FileVersion version;
        version.timestamp = std::time(0);
        version.content = content;
        versions.push_back(version);
        metadata.lastUpdated = version.timestamp;
    }

    std::string getLatestContent() const {
        if (!versions.empty()) {
            return versions.back().content;
        }
        return "No content available.";
    }
};

struct DataBlock {
    std::string content;

    DataBlock() : content("") {}
};

struct FileSystem {
    std::string name;
    std::vector<File> files;
    std::vector<DataBlock> dataBlocks;
    int nextBlockIndex = 0;

    FileSystem(std::string n) : name(n) {}

    File* findFileByName(const std::string& fileName) {
        for (File& file : files) {
            if (file.name == fileName) {
                return &file;
            }
        }
        return nullptr;
    }

    void addOrUpdateFile(const std::string& fileName, const std::string& content) {
        File* existingFile = findFileByName(fileName);
        if (existingFile) {
            existingFile->addVersion(content);
            std::cout << "File '" << fileName << "' updated with a new version." << std::endl;
            allocateFileBlocks(existingFile);
        } else {
            File newFile(fileName);
            newFile.addVersion(content);
            
            // Allocate data blocks for the new file
            allocateFileBlocks(&newFile);

            files.push_back(newFile);
            std::cout << "File '" << fileName << "' created with the initial version." << std::endl;
        }
    }

    void allocateFileBlocks(File* file) {
        std::string content = file->getLatestContent();
        int blockSize = 128;
        
        // Allocate data blocks for the file's content
        while (content.size() > 0) {
            DataBlock block;
            block.content = content.substr(0, blockSize); // Take a portion of the content

            file->allocatedBlocks.push_back(nextBlockIndex);
            dataBlocks.push_back(block);
            nextBlockIndex++;

            // Remove the portion that has been allocated
            content = content.substr(blockSize);
        }
    }

    void markFileForDeletion(const std::string& fileName) {
        File* fileToDelete = findFileByName(fileName);
        if (fileToDelete) {
            fileToDelete->metadata.deletionMark = true;
            std::cout << "File '" << fileName << "' marked for deletion." << std::endl;
        } else {
            std::cout << "File not found: '" << fileName << "'" << std::endl;
        }
    }

    void listFiles() {
        std::cout << "Files in the file system:" << std::endl;
        for (const File& file : files) {
            std::cout << "Name: " << file.name << " | Deletion Mark: " << (file.metadata.deletionMark ? "Yes" : "No");
            std::cout << " | Last Updated: " << std::asctime(std::localtime(&file.metadata.lastUpdated));
            std::cout << " | Latest Content: " << file.getLatestContent() << std::endl;
        }
    }
};

FileSystem fileSystem("root");

void saveFileSystemState() {
    std::ofstream backupFile("filesystem_backup.txt");
    for (const File& file : fileSystem.files) {
        backupFile << file.name << " " << file.metadata.deletionMark << " " << file.metadata.lastUpdated << std::endl;
        for (int blockIndex : file.allocatedBlocks) {
            backupFile << blockIndex << " ";
        }
        backupFile << std::endl;
    }
    std::cout << "File system state saved to 'filesystem_backup.txt'." << std::endl;
}

void loadFileSystemState() {
    std::ifstream backupFile("filesystem_backup.txt");
    if (!backupFile) {
        std::cout << "No backup file found. Starting with an empty file system." << std::endl;
        return;
    }

    fileSystem.files.clear();
    fileSystem.dataBlocks.clear();
    fileSystem.nextBlockIndex = 0;

    std::string line;
    File currentFile("");
    bool readingBlocks = false;

    while (std::getline(backupFile, line)) {
        std::istringstream iss(line);
        if (!readingBlocks) {
            if (iss >> currentFile.name >> currentFile.metadata.deletionMark >> currentFile.metadata.lastUpdated) {
                readingBlocks = true;
            }
        } else {
            int blockIndex;
            while (iss >> blockIndex) {
                currentFile.allocatedBlocks.push_back(blockIndex);
            }
            fileSystem.files.push_back(currentFile);
            currentFile = File("");
            readingBlocks = false;
        }
    }
    std::cout << "File system state loaded from 'filesystem_backup.txt'." << std::endl;
}

int main() {
    loadFileSystemState();

    int choice;
    std::string fileName;
    std::string fileContent;

    while (true) {
        std::cout << "Basic File System Menu:" << std::endl;
        std::cout << "1. List Files" << std::endl;
        std::cout << "2. Create/Update File" << std::endl;
        std::cout << "3. Mark File for Deletion" << std::endl;
        std::cout << "4. Save File System State" << std::endl;
        std::cout << "5. Exit" << std::endl;
        std::cout << "Enter your choice (1, 2, 3, 4, or 5): ";

        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a valid choice." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                fileSystem.listFiles();
                break;
            case 2:
                std::cout << "Enter the file name: ";
                std::cin >> fileName;
                std::cout << "Enter the file content: ";
                std::cin.ignore();
                std::getline(std::cin, fileContent);
                fileSystem.addOrUpdateFile(fileName, fileContent);
                break;
            case 3:
                std::cout << "Enter the file name to mark for deletion: ";
                std::cin >> fileName;
                fileSystem.markFileForDeletion(fileName);
                break;
            case 4:
                saveFileSystemState();
                break;
            case 5:
                std::cout << "Exiting the program." << std::endl;
                saveFileSystemState();
                return 0;
            default:
                std::cout << "Invalid choice. Please select 1, 2, 3, 4, or 5." << std::endl;
                break;
        }
    }

    return 0;
}
