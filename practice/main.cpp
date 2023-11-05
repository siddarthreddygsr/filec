#include <csignal>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include "directory.h"
using namespace std;

bool running = true;
void backupFileSystem();

void sigintHandler(int signal) {
    if (signal == SIGINT) {
        std::cout << "Received SIGINT (Ctrl+C). Exiting the program." << std::endl;
        backupFileSystem(); 
        running = false; 
        exit(0);
    }
}

Directory root("root");

void listFiles() {
    std::cout << "Files in the root directory:" << std::endl;
    for (const File& file : root.files) {
        std::cout << "Name: " << file.name << " | Deletion Mark: " << (file.metadata.deletionMark ? "Yes" : "No");
        std::cout << " | Last Updated: " << std::asctime(std::localtime(&file.metadata.lastUpdated));
        std::cout << " | Latest Content: " << file.getLatestContent() << std::endl;
    }
}

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

int list_functions()
{
    string fileName;
    string fileContent;
    while(running)
    {
        cout << "1. List all files " << endl;
        cout << "2. Create a new file" << endl;
        cout << "3. Update a file" << endl;
        cout << "4. Show File contents" << endl;
        cout << "5. Delete a File" << endl;
        cout << "6. Backup File System" << endl;
        cout << "7. Exit" << endl << "> ";
        int select;
        if(!(cin >> select))
        {
            cout << "invalid input" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
            // return 0;
        }
        
        switch (select)
        {
        case 1:
            // cout << "1" << endl;
            listFiles();
            break;
        case 2:
            // cout << "2" << endl;
            cout << "Enter the file name: ";
            cin >> fileName;
            cout << "Enter the file content: ";
            cin.ignore(); // Clear the newline from the buffer
            getline(std::cin, fileContent);
            root.addFile(fileName, fileContent);
            break;
        case 3:
            // cout << "3" << endl;
            cout << "Enter the file name: ";
            cin >> fileName;
            cout << "Enter the file content: ";
            cin.ignore(); // Clear the newline from the buffer
            getline(std::cin, fileContent);
            root.UpdateFile(fileName, fileContent);
            break;
        case 4:
            cout << "4. FILE CONTENT " << endl;
            break;
        case 5:
            cout << "Enter the file name to mark for deletion: ";
            cin >> fileName;
            root.markFileForDeletion(fileName);
            break;
        case 6:
            backupFileSystem();
            break;
        case 7:
            return 0;
        default:
            cout << "invalid selction" << endl;
            break;
        }
    }
    return 1;
}

int main(){
    signal(SIGINT, sigintHandler);
    loadFileSystem(); 
    list_functions();
}