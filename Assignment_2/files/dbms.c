#include "dbms.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int db_show_row(const struct db_table* db, unsigned int row) {
    // Check if the row is within the bounds of used rows
    if (row >= db->rows_used) {
        return 0;
    }

    struct album record = db->table[row]; // Retrieve the album record from the database

    printf("%6lu:%20.20s:%20.20s:%4hu\n", record.id, record.title, record.artist, record.year);

    return 1;
}

int db_add_row(struct db_table* db, struct album* a) {
    if (db == NULL || a == NULL) {
        return 0; // Early exit if inputs are invalid
    }

    // Ensure there is enough space and expand if necessary
    if (db->rows_used >= db->rows_total) {
        unsigned int new_size = db->rows_total + 5; // Increase space by 5 more records
        struct album* new_table = realloc(db->table, new_size * sizeof(struct album));
        if (new_table == NULL) {
            return 0; // Memory allocation failed
        }
        db->table = new_table;
        db->rows_total = new_size;
    }

    // Add the new record to the table
    db->table[db->rows_used] = *a;
    db->rows_used++;

    return 1; // Indicate success
}

int db_remove_row(struct db_table* db, unsigned long id) {
    if (db == NULL || db->table == NULL) {
        return 0;
    }

    int found = 0;
    for (unsigned int i = 0; i < db->rows_used; ++i) {
        if (db->table[i].id == id) {
            found = 1;
            // Shift all records one position up to fill the gap
            for (unsigned int j = i; j < db->rows_used - 1; ++j) {
                db->table[j] = db->table[j + 1];
            }
            db->rows_used--;

            // When the last record is removed, free the table and reset parameters
            if (db->rows_used == 0) {
                free(db->table);
                db->table = NULL;
                db->rows_total = 0;
                return 1;
            }

            // Reduce the table size if there are enough unused rows
            if (db->rows_total - db->rows_used >= 5) {
                struct album* new_table = realloc(db->table, (db->rows_total - 5) * sizeof(struct album));
                if (new_table == NULL) {
                    return 0;  // Reallocation failed, abort shrinking
                }
                db->table = new_table;
                db->rows_total -= 5;
            }

            return 1;
        }
    }

    return found;  // Return 1 if found and removed, 0 otherwise
}
