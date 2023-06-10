#include "file.h"

void insert_file(PGconn* pgconn, File file) {
    // Prepare the SQL statement with appropriate placeholders for the values
    const char* insert_query = "INSERT INTO files (filename, user_id, downloaded_number) VALUES ($1, $2, $3)";
    
    // Create the parameter values array
    const char* values[3];
    values[0] = file.filename;
    char user_id_str[10];
    snprintf(user_id_str, sizeof(user_id_str), "%d", file.user_id);
    values[1] = user_id_str;
    char downloaded_number_str[10];
    snprintf(downloaded_number_str, sizeof(downloaded_number_str), "%d", file.downloaded_number);
    values[2] = downloaded_number_str;
    
    // Execute the prepared statement with the parameter values
    PGresult* result = PQexecParams(pgconn, insert_query, 3, NULL, values, NULL, NULL, 0);
    PQclear(result);
}

PGresult* get_all_files(PGconn* pgconn) {
    // Execute a SELECT query to retrieve all records from the table
    const char* select_query = "SELECT * FROM files";
    PGresult* result = PQexec(pgconn, select_query);
    
    // Return the result set to the caller
    return result;
}

PGresult* get_file_by_id(PGconn* pgconn, int file_id) {
    // Prepare the SQL statement with appropriate placeholders for the ID value
    const char* select_query = "SELECT * FROM files WHERE file_id = $1";
    
    // Create the parameter values array
    const char* values[1];
    char file_id_str[10];
    snprintf(file_id_str, sizeof(file_id_str), "%d", file_id);
    values[0] = file_id_str;
    
    // Execute the prepared statement with the parameter values
    PGresult* result = PQexecParams(pgconn, select_query, 1, NULL, values, NULL, NULL, 0);
    
    // Return the result set to the caller
    return result;
}

void update_file_by_id(PGconn* pgconn, int file_id, File new_file) {
    // Prepare the SQL statement with appropriate placeholders for the ID value and new values
    const char* update_query = "UPDATE files SET filename = $1, user_id = $2, downloaded_number = $3 WHERE file_id = $4";
    
    // Create the parameter values array
    const char* values[4];
    values[0] = new_file.filename;
    char user_id_str[10];
    snprintf(user_id_str, sizeof(user_id_str), "%d", new_file.user_id);
    values[1] = user_id_str;
    char downloaded_number_str[10];
    snprintf(downloaded_number_str, sizeof(downloaded_number_str), "%d", new_file.downloaded_number);
    values[2] = downloaded_number_str;
    char file_id_str[10];
    snprintf(file_id_str, sizeof(file_id_str), "%d", file_id);
    values[3] = file_id_str;
    
    // Execute the prepared statement with the parameter values
    PGresult* result = PQexecParams(pgconn, update_query, 4, NULL, values, NULL, NULL, 0);
    PQclear(result);
}

void delete_file_by_id(PGconn* pgconn, int file_id) {
    // Prepare the SQL statement with appropriate placeholders for the ID value
    const char* delete_query = "DELETE FROM files WHERE file_id = $1";
    
    // Create the parameter values array
    const char* values[1];
    char file_id_str[10];
    snprintf(file_id_str, sizeof(file_id_str), "%d", file_id);
    values[0] = file_id_str;
    
    // Execute the prepared statement with the parameter values
    PGresult* result = PQexecParams(pgconn, delete_query, 1, NULL, values, NULL, NULL, 0);
    PQclear(result);
}

void delete_all_files(PGconn* pgconn) {
    // Execute a DELETE query to remove all records from the table
    const char* delete_query = "DELETE FROM files";
    PGresult* result = PQexec(pgconn, delete_query);
    PQclear(result);
}

