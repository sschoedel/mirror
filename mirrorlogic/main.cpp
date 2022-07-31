#include <mirrorgrid.h>

#include <iostream>

int main(int argc, char* argv[]) {
    
    char* configFile;
    bool configArgExists = false;
    for (int i=0; i<argc; i++) {
        if (std::string(argv[i]) == "-c") {
            configArgExists = true;
            configFile = argv[i+1];
        }
    }

    if (!configArgExists) {
        std::cerr << "Error: must pass config file argument with -c <config_file_location>" << std::endl;
        return 1;
    }

    std::cout << configFile << std::endl;

    // Create grid of tiles
    MirrorGrid grid(configFile);

    return 0;
}