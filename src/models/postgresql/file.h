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
void insert_file(PGconn* conn, File file);
PGresult* get_all_files(PGconn* conn);
PGresult* get_file_by_id(PGconn* conn, int file_id);
void update_file_by_id(PGconn* conn, int file_id, File new_file);
void delete_file_by_id(PGconn* conn, int file_id);
void delete_all_files(PGconn* conn);

#endif  // FILE_MANAGER_H

