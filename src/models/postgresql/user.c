#include "user.h"

void insert_user(PGconn* pgconn, User user) {
    // Prepare the SQL statement with appropriate placeholders for the values
    const char* insert_query = "INSERT INTO users (username, password, wrong_attempts, status, is_login) VALUES ($1, $2, $3, $4, $5)";
    
    // Create the parameter values array
    const char* values[5];
    values[0] = user.username;
    values[1] = user.password;
    char wrong_attempts_str[10];
    snprintf(wrong_attempts_str, sizeof(wrong_attempts_str), "%d", user.wrong_attempts);
    values[2] = wrong_attempts_str;
    char status_str[10];
    snprintf(status_str, sizeof(status_str), "%d", user.status);
    values[3] = status_str;
    char is_login_str[10];
    snprintf(is_login_str, sizeof(is_login_str), "%d", user.is_login);
    values[4] = is_login_str;
    
    // Execute the prepared statement with the parameter values
    PGresult* result = PQexecParams(pgconn, insert_query, 5, NULL, values, NULL, NULL, 0);
    PQclear(result);
}

PGresult* get_all_users(PGconn* pgconn) {
    // Execute a SELECT query to retrieve all records from the table
    const char* select_query = "SELECT * FROM users";
    PGresult* result = PQexec(pgconn, select_query);
    
    // Return the result set to the caller
    return result;
}

PGresult* get_user_by_id(PGconn* pgconn, int user_id) {
    // Prepare the SQL statement with appropriate placeholders for the ID value
    const char* select_query = "SELECT * FROM users WHERE user_id = $1";
    
    // Create the parameter values array
    const char* values[1];
    char user_id_str[10];
    snprintf(user_id_str, sizeof(user_id_str), "%d", user_id);
    values[0] = user_id_str;
    
    // Execute the prepared statement with the parameter values
    PGresult* result = PQexecParams(pgconn, select_query, 1, NULL, values, NULL, NULL, 0);
    
    // Return the result set to the caller
    return result;
}

void update_user_by_id(PGconn* pgconn, int user_id, User new_user) {
    // Prepare the SQL statement with appropriate placeholders for the ID value and new values
    const char* update_query = "UPDATE users SET username = $1, password = $2, wrong_attempts = $3, status = $4, is_login = $5 WHERE user_id = $6";
    
    // Create the parameter values array
    const char* values[6];
    values[0] = new_user.username;
    values[1] = new_user.password;
    char wrong_attempts_str[10];
    snprintf(wrong_attempts_str, sizeof(wrong_attempts_str), "%d", new_user.wrong_attempts);
    values[2] = wrong_attempts_str;
    char status_str[10];
    snprintf(status_str, sizeof(status_str), "%d", new_user.status);
    values[3] = status_str;
    char is_login_str[10];
    snprintf(is_login_str, sizeof(is_login_str), "%d", new_user.is_login);
    values[4] = is_login_str;
    char user_id_str[10];
    snprintf(user_id_str, sizeof(user_id_str), "%d", user_id);
    values[5] = user_id_str;
    
    // Execute the prepared statement with the parameter values
    PGresult* result = PQexecParams(pgconn, update_query, 6, NULL, values, NULL, NULL, 0);
    PQclear(result);
}

void delete_user_by_id(PGconn* pgconn, int user_id) {
    // Prepare the SQL statement with appropriate placeholders for the ID value
    const char* delete_query = "DELETE FROM users WHERE user_id = $1";
    
    // Create the parameter values array
    const char* values[1];
    char user_id_str[10];
    snprintf(user_id_str, sizeof(user_id_str), "%d", user_id);
    values[0] = user_id_str;
    
    // Execute the prepared statement with the parameter values
    PGresult* result = PQexecParams(pgconn, delete_query, 1, NULL, values, NULL, NULL, 0);
    PQclear(result);
}

void delete_all_users(PGconn* pgconn) {
    // Execute a DELETE query to remove all records from the table
    const char* delete_query = "DELETE FROM users";
    PGresult* result = PQexec(pgconn, delete_query);
    PQclear(result);
}
