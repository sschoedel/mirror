#include <mirrorgrid.h>

// Take config file with grid layout for simulation
// For non-simulation, take config file with no grid layout and 
// search through tree to find which edges are connected
MirrorGrid::MirrorGrid(char* cfgFileName) {
    // Parse file and create tile grid
    uint8_t result = ParseConfigFile(cfgFileName);
    if (result != 0) {
        std::cerr << "Error during parsing" << std::endl;
    }
    else {
        std::cout << "Successfully parsed config file" << std::endl;
    }
}

uint8_t MirrorGrid::ParseConfigFile(char* cfgFileName) {
    // Load file with grid configuration
    std::ifstream cfgIn(cfgFileName);
    if ((cfgIn.rdstate() & std::ifstream::failbit) != 0) {
        std::cerr << "Error opening " << cfgFileName << std::endl;
        return 1;
    }
    else {
        std::cout << "successfully opened " << cfgFileName << std::endl;
    }

    // Dump config file contents
    std::cout << "Config file content dump: \n\n";
    for (std::string line; std::getline(cfgIn, line); ) {
        std::cout << line << std::endl;
    }
    std::cout << std::endl;

    std::ifstream cfgIn2(cfgFileName);

    bool found_if_simulation = false;
    bool found_num_tiles = false;
    bool found_base_tile_location = false;
    bool found_layout = false;
    bool good_input = true;

    // Parse file
    std::cout << "Searching config file...\n";
    std::string word;
    while (std::getline(cfgIn2, word, ':')) {
        word.erase(std::remove_if(word.begin(), word.end(), ::isspace), word.end());
        if (word == "simulation") {
            found_if_simulation = true;
            std::getline(cfgIn2, word);
            word.erase(std::remove_if(word.begin(), word.end(), ::isspace), word.end());
            if (word == "true" || word == "True") {
                simulating = true;
                std::cout << "Simulating" << std::endl;
            }
            else if (word == "false" || word == "False") {
                simulating = false;
                std::cout << "Deploying to real hex grid" << std::endl;
            }
            else {
                std::cerr << "Could not determine if simulating or deploying. Please enter true or false.\n";
                good_input = false;
            }
        }
        else if (word == "num_tiles") {
            found_num_tiles = true;
            std::getline(cfgIn2, word);
            try {
                num_tiles = std::stoi(word);
                std::cout << "Found num_tiles: " << num_tiles << std::endl;
            }
            catch (...) {
                std::cerr << "Invalid value for num_tiles. Please enter an integer.\n";
                good_input = false;
            }
        }
        else if (word == "base_tile_location") {
            found_base_tile_location = true;
            std::getline(cfgIn2, word, ',');
            try {
                base_tile_location_x =  std::stof(word);
                std::cout << "Found base_tile_location_x: " << base_tile_location_x << std::endl;
            }
            catch (...) {
                std::cerr << "Invalid value for base_tile_location_x. Please enter a float.\n";
                good_input = false;
            }

            std::getline(cfgIn2, word);
            try {
                base_tile_location_y =  std::stof(word);
                std::cout << "Found base_tile_location_y: " << base_tile_location_y << std::endl;
            }
            catch (...) {
                good_input = false;
                std::cerr << "Invalid value for base_tile_location_y. Please enter a float.\n";
            }
        }
        else if (word == "layout") {
            found_layout = true;
            std::cout << "Parse layout here!" << std::endl;
        }
    }

    if (!found_if_simulation) {
        std::cerr << "Couldn't find simulation in configuration file.\n";
    }
    if (!found_num_tiles) {
        std::cerr << "Couldn't find num_tiles identifier in configuration file.\n";
    }
    if (!found_base_tile_location) {
        std::cerr << "Couldn't find base_tile_location identifier in configuration file.\n";
    }
    if (!found_layout) {
        std::cerr << "Couldn't find layout identifier in configuration file.\n";
    }

    if (!good_input) {
        std::cerr << "Something went wrong when parsing config file. Exiting." << std::endl;
        return 2;
    }



    return 0;
}