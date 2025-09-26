#include "core/segfault.h"
#include <cppcore/Common/TStringBase.h>

#include <iostream>
#include <string>

static const cppcore::TStringBase<char> ManifestName("Manifest", 8);

struct MemoryStatistics {
    size_t inputSize{ 0l };
    size_t outputSize{ 0l };
};

void getVersion(std::string& version) {
    version.append("V0.1");
}

static void showHelp() {
    std::string v;
    getVersion(v);
    std::cout << "SegFault AssetBacker "<< v << std::endl << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "assetbaker -i <manifest_file> -o <output_file" << std::endl;
}

bool readManifest(const std::string& input, MemoryStatistics& stats) {
    std::cout << "Try to read input manifest " << input << std::endl;
    return true;
}

bool writeAssetArchive(const std::string& output, MemoryStatistics& stats) {
    std::cout << "Try to write output asset archive " << output << std::endl;
    return true;
}

void showStatistics(const MemoryStatistics& stats) {
    std::cout << "Memory statistics:" << std::endl;
    std::cout << "==================" << std::endl;
    std::cout << "Input filesize: " << stats.inputSize << std::endl;
    std::cout << "Output filesize: " << stats.inputSize << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        showHelp();
        return 0;
    }

    std::string input, output;
    if (strncmp(argv[1], "-i", 2) == 0) {
        input = std::string(argv[2]);
    }

    if (strncmp(argv[3], "-o", 2) == 0) {
        output = std::string(argv[4]);
    }
    std::string v;
    getVersion(v);
    std::cout << std::endl << "AssetBaker " << v << std::endl;
    std::cout << std::endl << "Start asset baking process ... " << std::endl;
    MemoryStatistics stats;
    if (!readManifest(input, stats)) {
        return -1;
    }

    if (!writeAssetArchive(output, stats)) {
        return -1;
    }
    showStatistics(stats);

    return 0;
}
