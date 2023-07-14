#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "../../includes/common.h"

// Function to insert a new file record into the table
void insert_file(File file);

// Function to retrieve all file records from the table
FileResult get_all_files();

// Function to retrieve a specific file record from the table by its name
FileResult get_files_by_name(const char* filename);

// Function to retrieve files by user_id 
FileResult get_files_by_user_id(int user_id);

// Function to retrieve a specific file record from the table
File get_file_by_id(int file_id);

// Function to update a specific file record in the table
void update_file_by_id(int file_id, File new_file);

// Function to delete a specific file record from the table
void delete_file_by_id(int file_id);

// Function to delete a specific file record from the table by filename and user_id
File get_file_by_filename_and_user_id(const char* filename, int user_id);

// Function to delete all file records from the table
void delete_all_files();

#endif  // FILE_MANAGER_H

