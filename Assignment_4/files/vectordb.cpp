#include "vectordb.hpp"
#include "abstractdb.hpp"
#include <algorithm>

namespace nwen {
    int VectorDbTable::rows() const {
        return table.size();
    }

    movie* VectorDbTable::get(int index) const {
        // Handle index out of bounds by returning nullptr
        if (index < 0 || index >= table.size()) {
            return nullptr;
        }
        return const_cast<movie*>(&table.at(index));
    }

    bool VectorDbTable::add(const movie& m) {
        // Prevent duplicates based on movie ID
        if (findMovieIndexById(m.id) != -1) {
            return false;
        }
        table.push_back(m);
        return true;
    }

    bool VectorDbTable::update(unsigned long id, const movie& m) {
        // Directly update the movie if it exists
        int index = findMovieIndexById(id);
        if (index != -1) {
            table[index] = m;
            return true;
        }
        return false;
    }

    bool VectorDbTable::remove(unsigned long id) {
        // Remove movie by ID, if it exists
        int index = findMovieIndexById(id);
        if (index != -1) {
            table.erase(table.begin() + index);
            return true;
        }
        return false;
    }

    int VectorDbTable::findMovieIndexById(unsigned long id) const {
        // Search for movie by ID and return index
        for (int i = 0; i < table.size(); ++i) {
            if (table[i].id == id) {
                return i;
            }
        }
        return -1;
    }

    bool VectorDbTable::loadCSV(const std::string& filename) {
        return AbstractDbTable::loadCSV(filename);
    }

    bool VectorDbTable::saveCSV(const std::string& filename) const {
        return AbstractDbTable::saveCSV(filename);
    }

} // namespace nwen
