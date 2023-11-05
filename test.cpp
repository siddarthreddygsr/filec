#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <map>

// Define a structure to represent file metadata
struct FileMetadata {
    bool deletionMark = false;
    std::time_t lastUpdated = std::time(0);
};

// Define a structure to represent a file version
struct FileVersion {
    std::time_t timestamp;
    std::string content;
};

// Define a structure to represent a file with metadata and versions
struct File {
    std::string name;
    FileMetadata metadata;
    std::vector<FileVersion> versions;

    File(std::string n) : name(n) {}

    // Function to add a new version of the file
    void addVersion(const std::string& content) {
        FileVersion version;
        version.timestamp = std::time(0);
        version.content = content;
        versions.push_back(version);
        metadata.lastUpdated = version.timestamp;
    }

    // Function to get the latest content of the file (const version for use in const functions)
    std::string getLatestContent() const {
        if (!versions.empty()) {
            return versions.back().content;
        }
        return "No content available.";
    }
};

// Define a structure to represent a directory
struct Directory {
    std::string name;
    std::vector<File> files;

    Directory(std::string n) : name(n) {}

    // Function to add or update a file in the directory
    void addOrUpdateFile(const std::string& fileName, const std::string& content) {
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
        std::cout << "File '" << fileName << "' created with the initial version." << std::endl;
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

// Create a root directory to store files
Directory root("root");

// Function to list files in the root directory
void listFiles() {
    std::cout << "Files in the root directory:" << std::endl;
    for (const File& file : root.files) {
        std::cout << "Name: " << file.name << " | Deletion Mark: " << (file.metadata.deletionMark ? "Yes" : "No");
        std::cout << " | Last Updated: " << std::asctime(std::localtime(&file.metadata.lastUpdated));
        std::cout << " | Latest Content: " << file.getLatestContent() << std::endl;
    }
}

// Function to save the file system state to a backup file
void backupFileSystem() {
    std::ofstream backupFile("filesystem_backup.txt");
    for (const File& file : root.files) {
        backupFile << file.name << " " << file.metadata.deletionMark << " " << file.metadata.lastUpdated << std::endl;
        for (const FileVersion& version : file.versions) {
            backupFile << version.timestamp << " " << version.content << std::endl;
        }
    }
    std::cout << "File system backup created." << std::endl;
}

// Function to load the file system state from a backup file
void loadFileSystem() {
    std::ifstream backupFile("filesystem_backup.txt");
    if (!backupFile) {
        std::cout << "No backup file found. Starting with an empty file system." << std::endl;
        return;
    }

    root.files.clear(); // Clear existing files in the root directory

    std::string line;
    File currentFile("");
    bool readingVersions = false;

    while (std::getline(backupFile, line)) {
        std::istringstream iss(line);
        if (!readingVersions) {
            if (iss >> currentFile.name >> currentFile.metadata.deletionMark >> currentFile.metadata.lastUpdated) {
                root.files.push_back(currentFile);
                readingVersions = true;
            }
        } else {
            FileVersion version;
            if (iss >> version.timestamp) {
                std::getline(iss, version.content); // Read the content including spaces
                currentFile.versions.push_back(version);
            }
        }
    }
    std::cout << "File system loaded from backup." << std::endl;
}

int main() {
    loadFileSystem(); // Load the file system from backup (if available)

    int choice;
    std::string fileName;
    std::string fileContent;

    while (true) {
        // Display menu to the user
        std::cout << "Basic File System Menu:" << std::endl;
        std::cout << "1. List Files" << std::endl;
        std::cout << "2. Create/Update File" << std::endl;
        std::cout << "3. Mark File for Deletion" << std::endl;
        std::cout << "4. Backup File System" << std::endl;
        std::cout << "5. Exit" << std::endl;
        std::cout << "Enter your choice (1, 2, 3, 4, or 5): ";
        // std::cin >> choice;

        if (!(std::cin >> choice)) {
            // Handle invalid input
            std::cout << "Invalid input. Please enter a valid choice." << std::endl;
            std::cin.clear();  // Clear the error state
            // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
            continue; // Restart the loop
        }
        switch (choice) {
            case 1:
                listFiles();
                break;
            case 2:
                std::cout << "Enter the file name: ";
                std::cin >> fileName;
                std::cout << "Enter the file content: ";
                std::cin.ignore(); // Clear the newline from the buffer
                std::getline(std::cin, fileContent);
                root.addOrUpdateFile(fileName, fileContent);
                break;
            case 3:
                std::cout << "Enter the file name to mark for deletion: ";
                std::cin >> fileName;
                root.markFileForDeletion(fileName);
                break;
            case 4:
                backupFileSystem();
                break;
            case 5:
                std::cout << "Exiting the program." << std::endl;
                backupFileSystem(); // Backup the file system before exiting
                return 0;
            default:
                std::cout << "Invalid choice. Please select 1, 2, 3, 4, or 5." << std::endl;
                break;
        }
    }

    return 0;
}
