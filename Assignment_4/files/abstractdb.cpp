#include "abstractdb.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <cstring>

namespace nwen {
    bool AbstractDbTable::loadCSV(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) { // Check if the file was opened successfully
            std::cerr << "Failed to open file: " << filename << std::endl;
            return false;
        }

        std::string line;
        try { // Attempt to read and parse the file
            while (std::getline(file, line)) {
                std::istringstream ss(line);
                std::vector<std::string> fields;
                std::string field;

                while (ss.good()) { // Parse fields from the line
                    std::getline(ss, field, ',');
                    size_t start_quotes = field.find_first_of('"');
                    if (start_quotes != std::string::npos) {
                        std::string temp;
                        while (field.back() != '"') { // Handle fields with commas
                            std::getline(ss, temp, ',');
                            field += ',' + temp;
                        }
                        field = field.substr(start_quotes + 1, field.length() - start_quotes - 2); // Remove quotes
                    }
                    trim(field);  // Trim whitespace
                    fields.push_back(field);
                }

                if (fields.size() != 4) {
                    throw std::runtime_error("Invalid record format on line: " + line);
                }

                // Populate movie structure and validate
                movie mv;
                try {
                    mv.id = std::stoul(fields[0]); // 
                    strncpy(mv.title, fields[1].c_str(), sizeof(mv.title) - 1);
                    mv.title[sizeof(mv.title) - 1] = '\0';
                    mv.year = static_cast<unsigned short>(std::stoi(fields[2]));
                    strncpy(mv.director, fields[3].c_str(), sizeof(mv.director) - 1);
                    mv.director[sizeof(mv.director) - 1] = '\0';
                }
                catch (const std::exception& e) {
                    std::cerr << "Data conversion error for line: " << line << " - " << e.what() << std::endl;
                    continue; // Skip this record and continue with the next
                }

                // Add movie to table and check for success
                if (!add(mv)) {
                    std::cerr << "Failed to add movie: " << fields[1] << " (ID: " << fields[0] << ")" << std::endl;
                    continue; // Skip this record and continue with the next
                }
            }
        }
        catch (const std::exception& e) { // Catch any exceptions that occur during file processing
            std::cerr << "Exception during file processing: " << e.what() << std::endl;
            file.close();
            return false;
        }

        file.close();
        return true;
    }

    bool AbstractDbTable::saveCSV(const std::string& filename) const {
        std::ofstream outFile(filename, std::ofstream::out | std::ofstream::trunc);  // Open for writing, truncate existing

        if (!outFile.is_open()) {
            return false; // Return false if the file cannot be opened.
        }

        try {
            for (int i = 0; i < rows(); ++i) {
                const movie* mv = get(i);
                if (mv) {
                    // Write the movie's details to outFile in CSV format, with title and director in quotes to handle commas and special characters.
                    outFile << mv->id << ",\"" << mv->title << "\"," << mv->year << ",\"" << mv->director << "\"\n";
                }
            }
        }
        catch (...) {
            outFile.close(); // Ensure the file is closed if an error occurs
            return false;  // Return false if there's an error during file writing
        }

        outFile.close(); // Close the file after writing
        return true;  // Return true if everything was successful
    }

} // namespace nwen
