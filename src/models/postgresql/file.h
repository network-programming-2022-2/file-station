#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "common.h"

typedef struct File {
  int file_id;
  char* filename;
  int user_id;
  int downloaded_number;
} File;

// Function declarations

// Function to insert a new file record into the table
void insert_file(PGconn* conn, File file);

// Function to retrieve all file records from the table
PGresult* get_all_files(PGconn* conn);

// Function to retrieve a specific file record from the table by its name
PGresult* get_file_by_name(PGconn* conn, const char* filename);

// Function to retrieve a specific file record from the table
PGresult* get_file_by_id(PGconn* conn, int file_id);

// Function to update a specific file record in the table
void update_file_by_id(PGconn* conn, int file_id, File new_file);

// Function to delete a specific file record from the table
void delete_file_by_id(PGconn* conn, int file_id);

// Function to delete all file records from the table
void delete_all_files(PGconn* conn);

#endif  // FILE_MANAGER_H

