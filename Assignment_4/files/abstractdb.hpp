#ifndef __ABSTRACT_DB_HPP__
#define __ABSTRACT_DB_HPP__

#include <string>
#include <algorithm>

namespace nwen {
    struct movie {
        unsigned long id;
        char title[50];
        unsigned short year;
        char director[50];
    };

    class AbstractDbTable {
    public:
        virtual ~AbstractDbTable() = default;

        /**
         * Get the number of rows in the database.
         * @return The number of rows in the database.
        */
        virtual int rows() const = 0;

        /**
         * Get a movie from the database.
         * @param index The index of the movie to retrieve.
         * @return A pointer to the movie structure or nullptr if the index is out of bounds.
        */
        virtual movie* get(int index) const = 0;

        /**
         * Add a movie to the database.
         * @param m The movie structure to add.
         * @return true if the movie was added, false if the movie already exists.
        */
        virtual bool add(const movie& m) = 0;

        /**
         * Update a movie in the database.
         * @param id The ID of the movie to update.
         * @param m The movie structure to update.
         * @return true if the movie was updated, false if the movie does not exist.
        */
        virtual bool update(unsigned long id, const movie& m) = 0;

        /**
         * Remove a movie from the database.
         * @param id The ID of the movie to remove.
         * @return true if the movie was removed, false if the movie does not exist.
        */
        virtual bool remove(unsigned long id) = 0;

        /**
         * Load a CSV file into the database.
         * @param filename The name of the CSV file to load.
         * @return true if the file was loaded successfully, false if an error occurred.
        */
        virtual bool loadCSV(const std::string& filename);

        /**
         * Save the database to a CSV file.
         * @param filename The name of the CSV file to save.
         * @return true if the file was saved successfully, false if an error occurred.
        */
        virtual bool saveCSV(const std::string& filename) const;

        /**
         * Trim whitespace from the beginning and end of a string.
         * @param s The string to trim.
         * @return The trimmed string.
         * @see https://stackoverflow.com/a/217605
        */
        static inline void trim(std::string& s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
                }));
            s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
                return !std::isspace(ch);
                }).base(), s.end());
        }
    };
}

#endif /* __ABSTRACT_DB_HPP__ */
