#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "../../includes/common.h"

// Function to insert a new file record into the table
void insert_file(File file);

// Function to retrieve all file records from the table
PGresult* get_all_files();

// Function to retrieve a specific file record from the table by its name
PGresult* get_file_by_name(const char* filename);

// Function to retrieve a specific file record from the table
PGresult* get_file_by_id(int file_id);

// Function to update a specific file record in the table
void update_file_by_id(int file_id, File new_file);

// Function to delete a specific file record from the table
void delete_file_by_id(int file_id);

// Function to delete all file records from the table
void delete_all_files();

#endif  // FILE_MANAGER_H

