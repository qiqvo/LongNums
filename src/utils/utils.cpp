#include "core/long.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<Long> load_small_primes(const std::string& filename) {
    std::vector<Long> primes;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open prime file: " << filename << std::endl;
        // Return some default small primes if file can't be opened
        return {Long(2), Long(3), Long(5), Long(7), Long(11), Long(13), Long(17), Long(19), Long(23), Long(29)};
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines
        if (line.empty()) continue;
        
        // Convert string to number and add to vector
        try {
            ull prime_value = std::stoull(line);
            primes.emplace_back(prime_value);
        } catch (const std::exception& e) {
            std::cerr << "Warning: Could not parse line as prime: " << line << std::endl;
        }
    }
    
    file.close();
    return primes;
} 