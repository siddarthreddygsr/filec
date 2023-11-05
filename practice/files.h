#include <ctime>
#include <string>

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