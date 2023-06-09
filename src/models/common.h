#ifndef COMMON_H
#define COMMON_H

#include <string.h>
#include <stdlib.h>

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

#endif  // COMMON_H
