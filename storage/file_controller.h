#ifndef FILE_CONTROLLER_H
#define FILE_CONTROLLER_H 

#include "../models/postgresql/file.h"
#include "../models/postgresql/postgresql.h"
#include "../models/postgresql/user.h"
#include "user_controller.h"

// Function to upload a file
bool upload_file(const char* filename, const char* username);

// Function to delete a file
bool delete_file(const char* filename, const char* username);

// // Function to check if a file exists
// bool does_file_exist(const char* filename);

// // Function to get file information by file ID
// File* get_file_by_id(int file_id);

// // Function to get file information by filename
// File* get_file_by_filename(const char* filename);

// // Function to update file information
// bool update_file(File* file);

// // Function to delete a file
// bool delete_file(int file_id);

// // Function to increment the downloaded number of a file
// bool increment_downloads(int file_id);

// // Function to get the most downloaded files
// File** get_most_downloaded_files(int limit);

// // F