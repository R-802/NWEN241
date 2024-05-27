#ifndef __VECTOR_DB_HPP__
#define __VECTOR_DB_HPP__

#include "abstractdb.hpp"
#include <vector>

namespace nwen {
    class VectorDbTable : public AbstractDbTable {
    public:

        /**
         * Get the number of rows in the database.
         * @return The number of rows in the database.
         */
        int rows() const;

        /**
         * Get a movie from the database.
         * @param index The index of the movie to retrieve.
         * @return A pointer to the movie structure or nullptr if the index is out of bounds.
         */
        movie* get(int index) const;

        /**
         * Add a movie to the database.
         * @param m The movie structure to add.
         * @return true if the movie was added, false if the movie already exists.
         */
        bool add(const movie& m);

        /**
         * Update a movie in the database.
         * @param id The ID of the movie to update.
         * @param m The movie structure to update.
         * @return true if the movie was updated, false if the movie does not exist.
         */
        bool update(unsigned long id, const movie& m);

        /**
         * Remove a movie from the database.
         * @param id The ID of the movie to remove.
         * @return true if the movie was removed, false if the movie does not exist.
         */
        bool remove(unsigned long id);

        /**
         * Load a CSV file into the database.
         * @param filename The name of the CSV file to load.
         * @return true if the file was loaded successfully, false if an error occurred.
         */
        bool loadCSV(const std::string& filename);

        /**
         * Save the database to a CSV file.
         * @param filename The name of the CSV file to save.
         * @return true if the file was saved successfully, false if an error occurred.
         */
        bool saveCSV(const std::string& filename) const;

    private:
        std::vector<movie> table;
        int findMovieIndexById(unsigned long id) const;
    };
}

#endif /* __VECTOR_DB_HPP__ */
