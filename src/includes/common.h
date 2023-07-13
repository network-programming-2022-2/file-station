#ifndef COMMON_H
#define COMMON_H

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <libpq-fe.h>

typedef enum {
    POSTGRESQL,
    // MYSQL,
    // Add more database types as needed
} DatabaseType;

typedef struct {
    DatabaseType type;
    const char* host;
    const char* port;
    const char* dbname;
    const char* user;
    const char* password;
} DatabaseConnection;

typedef struct File {
  int file_id;
  char* filename;
  int user_id;
  int downloaded_numbers;
} File;

typedef struct {
    File* files;
    int num_files;
} FileResult;

typedef struct {
    int user_id;
    char* username;
    char* password;
    int wrong_attempts;
    int status;
    int is_login;
} User;

typedef struct {
    User* users;
    int num_users;
} UserResult;

extern PGconn* pgconn;

#endif  // COMMON_H
