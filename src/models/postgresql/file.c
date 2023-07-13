#include "file.h"

void insert_file(File file) {
    // Prepare the SQL statement with appropriate placeholders for the values
    const char* insert_query = "INSERT INTO files (filename, user_id, downloaded_numbers) VALUES ($1, $2, $3)";
    
    // Create the parameter values array
    const char* values[3];
    values[0] = file.filename;
    char user_id_str[10];
    snprintf(user_id_str, sizeof(user_id_str), "%d", file.user_id);
    values[1] = user_id_str;
    char downloaded_numbers_str[10];
    snprintf(downloaded_numbers_str, sizeof(downloaded_numbers_str), "%d", file.downloaded_numbers);
    values[2] = downloaded_numbers_str;
    
    // Execute the prepared statement with the parameter values
    PGresult* result = PQexecParams(pgconn, insert_query, 3, NULL, values, NULL, NULL, 0);
    if (PQresultStatus(result) != PGRES_COMMAND_OK) {
      printf("Error: %s\n", PQresultErrorMessage(result));
    }
    PQclear(result);
}

FileResult get_all_files() {
    // Execute a SELECT query to retrieve all records from the table
    const char* select_query = "SELECT * FROM files";
    PGresult* result = PQexec(pgconn, select_query);

    // Get the number of rows in the result set
    int num_rows = PQntuples(result);
    if (num_rows == 0) {
        FileResult file_result;
        file_result.files = NULL;
        file_result.num_files = 0;
        return file_result;
    }

    // Create an array of File structs
    File* files = malloc(num_rows * sizeof(File));

    // Iterate over the result set and populate the array
    for (int i = 0; i < num_rows; i++) {
        File file;

        // Retrieve the values from the result set
        file.file_id = atoi(PQgetvalue(result, i, 0));
        file.filename = PQgetvalue(result, i, 1);
        file.user_id = atoi(PQgetvalue(result, i, 2));
        file.downloaded_numbers = atoi(PQgetvalue(result, i, 3));

        // Assign the file struct to the array
        files[i] = file;
    }

    // Clear the result set
    PQclear(result);

    FileResult file_result;
    file_result.files = files;
    file_result.num_files = num_rows;
    return file_result;
}

FileResult get_files_by_user_id(int user_id)
{
    // Prepare the SQL statement with appropriate placeholders for the values
    const char* select_query = "SELECT * FROM files WHERE user_id = $1";

    // Create the parameter values array
    const char* values[1];
    char user_id_str[10];
    snprintf(user_id_str, sizeof(user_id_str), "%d", user_id);
    values[0] = user_id_str;


    // Execute the prepared statement with the parameter values
    PGresult* result = PQexecParams(pgconn, select_query, 1, NULL, values, NULL, NULL, 0);

    // Get the number of rows in the result set
    int num_rows = PQntuples(result);
    if (num_rows == 0) {
        FileResult file_result;
        file_result.files = NULL;
        file_result.num_files = 0;
        return file_result;
    }

    // Create an array of File structs
    File* files = malloc(num_rows * sizeof(File));

    // Iterate over the result set and populate the array
    for (int i = 0; i < num_rows; i++) {
        File file;
        
        file.file_id = atoi(PQgetvalue(result, i, 0));
        file.filename = PQgetvalue(result, i, 1);
        file.user_id = atoi(PQgetvalue(result, i, 2));
        file.downloaded_numbers = atoi(PQgetvalue(result, i, 3));

        // Assign the file struct to the array
        files[i] = file;
    }

    // Clear the result set
    PQclear(result);

    FileResult file_result;
    file_result.files = files;
    file_result.num_files = num_rows;
    return file_result;
}


FileResult get_files_by_name(const char* filename) {
    // Prepare the SQL statement with appropriate placeholders for the values
    const char* select_query = "SELECT * FROM files WHERE filename = $1";

    // Create the parameter values array
    const char* values[1];
    values[0] = filename;

    // Execute the prepared statement with the parameter values
    PGresult* result = PQexecParams(pgconn, select_query, 1, NULL, values, NULL, NULL, 0);

    // Get the number of rows in the result set
    int num_rows = PQntuples(result);
    if (num_rows == 0) {
        FileResult file_result;
        file_result.files = NULL;
        file_result.num_files = 0;
        return file_result;
    }

    // Create an array of File structs
    File* files = malloc(num_rows * sizeof(File));

    // Iterate over the result set and populate the array
    for (int i = 0; i < num_rows; i++) {
        File file;

        // Retrieve the values from the result set
        file.file_id = atoi(PQgetvalue(result, i, 0));
        file.filename = PQgetvalue(result, i, 1);
        file.user_id = atoi(PQgetvalue(result, i, 2));
        file.downloaded_numbers = atoi(PQgetvalue(result, i, 3));

        // Assign the file struct to the array
        files[i] = file;
    }

    // Clear the result set
    PQclear(result);

    FileResult file_result;
    file_result.files = files;
    file_result.num_files = num_rows;
    return file_result;
}

File get_file_by_id(int file_id) {
    // Prepare the SQL statement with appropriate placeholders for the ID value
    const char* select_query = "SELECT * FROM files WHERE file_id = $1";

    // Create the parameter values array
    const char* values[1];
    char file_id_str[10];
    snprintf(file_id_str, sizeof(file_id_str), "%d", file_id);
    values[0] = file_id_str;

    // Execute the prepared statement with the parameter values
    PGresult* result = PQexecParams(pgconn, select_query, 1, NULL, values, NULL, NULL, 0);

    // Get the number of rows in the result set
    int num_rows = PQntuples(result);
    if (num_rows == 0) {
        File file = {0, "", 0, 0};
        return file;
    }

    // Retrieve the values from the result set
    File file;
    file.file_id = atoi(PQgetvalue(result, 0, 0));
    file.filename = PQgetvalue(result, 0, 1);
    file.user_id = atoi(PQgetvalue(result, 0, 2));
    file.downloaded_numbers = atoi(PQgetvalue(result, 0, 3));

    // Clear the result set
    PQclear(result);

    // Return the File struct
    return file;
}


void update_file_by_id(int file_id, File new_file) {
    // Prepare the SQL statement with appropriate placeholders for the ID value and new values
    const char* update_query = "UPDATE files SET filename = $1, user_id = $2, downloaded_numbers = $3 WHERE file_id = $4";
    
    // Create the parameter values array
    const char* values[4];
    values[0] = new_file.filename;
    char user_id_str[10];
    snprintf(user_id_str, sizeof(user_id_str), "%d", new_file.user_id);
    values[1] = user_id_str;
    char downloaded_numbers_str[10];
    snprintf(downloaded_numbers_str, sizeof(downloaded_numbers_str), "%d", new_file.downloaded_numbers);
    values[2] = downloaded_numbers_str;
    char file_id_str[10];
    snprintf(file_id_str, sizeof(file_id_str), "%d", file_id);
    values[3] = file_id_str;
    
    // Execute the prepared statement with the parameter values
    PGresult* result = PQexecParams(pgconn, update_query, 4, NULL, values, NULL, NULL, 0);
    PQclear(result);
}

void delete_file_by_id(int file_id) {
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

void delete_all_files() {
    // Execute a DELETE query to remove all records from the table
    const char* delete_query = "DELETE FROM files";
    PGresult* result = PQexec(pgconn, delete_query);
    PQclear(result);
}

