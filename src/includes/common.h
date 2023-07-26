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
  int file_id;
  char filename[100];
  int downloaded_numbers;
  char username[100];
  char ip[100];
  int port;
} SearchResult;

typedef struct {
    int user_id;
    char* username;
    char* password;
    int wrong_attempts;
    int status;
    int server_port;
    char* ip;
} User;

typedef struct {
    User* users;
    int num_users;
} UserResult;

extern PGconn* pgconn;

#endif  // COMMON_H
