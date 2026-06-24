/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2026 Segfault by Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#include "core/segfault.h"
#include "core/filearchive.h"
#include "core/genericfilemanager.h"
#include <cppcore/Common/TStringBase.h>

#include <nlohmann/json.hpp>
#include <iostream>
#include <string>

static const cppcore::TStringBase<char> ManifestName("Manifest", 8);

using json = nlohmann::json;
using namespace segfault::core;

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
    GenericFileManager fm;
    auto file = fm.createFileReader(input.c_str());
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
