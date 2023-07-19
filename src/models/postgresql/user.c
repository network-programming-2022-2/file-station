#include "user.h"

void insert_user(User user) {
    // Prepare the SQL statement with appropriate placeholders for the values
    const char* insert_query = "INSERT INTO users (username, password, wrong_attempts, status, server_port, ip) VALUES ($1, $2, $3, $4, $5, $6)";
    
    // Create the parameter values array
    const char* values[6];
    values[0] = user.username;
    values[1] = user.password;
    char wrong_attempts_str[10];
    snprintf(wrong_attempts_str, sizeof(wrong_attempts_str), "%d", user.wrong_attempts);
    values[2] = wrong_attempts_str;
    char status_str[10];
    snprintf(status_str, sizeof(status_str), "%d", user.status);
    values[3] = status_str;
    char server_port_str[10];
    snprintf(server_port_str, sizeof(server_port_str), "%d", user.server_port);
    values[4] = server_port_str;
    values[5] = user.ip;
    
    // Execute the prepared statement with the parameter values
    PGresult* result = PQexecParams(pgconn, insert_query, 6, NULL, values, NULL, NULL, 0);
    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
      printf("Error: %s\n", PQerrorMessage(pgconn));
      PQclear(result);
      return;
    }

    PQclear(result);
}

UserResult get_all_users() {
    // Execute a SELECT query to retrieve all records from the table
    const char* select_query = "SELECT * FROM users";
    PGresult* result = PQexec(pgconn, select_query);
    
    // Get the number of rows in the result set
    int num_rows = PQntuples(result);
    if (num_rows == 0) {
        UserResult user_result;
        user_result.users = NULL;
        user_result.num_users = 0;
        return user_result;
    }

    // Create an array of User structs
    User* users = malloc(num_rows * sizeof(User));

    // Iterate over the result set and populate the array
    for (int i = 0; i < num_rows; i++) {
        User user;
        
        // Retrieve the values from the result set
        user.user_id = atoi(PQgetvalue(result, i, 0));
        user.username = PQgetvalue(result, i, 1);
        user.password = PQgetvalue(result, i, 2);
        user.wrong_attempts = atoi(PQgetvalue(result, i, 3));
        user.status = atoi(PQgetvalue(result, i, 4));
        user.server_port = atoi(PQgetvalue(result, i, 5));
        user.ip = PQgetvalue(result, i, 6);

        // Assign the user struct to the array
        users[i] = user;
    }
    
    // Clear the result set
    PQclear(result);

    UserResult user_result;
    user_result.users = users;
    user_result.num_users = num_rows;
    return user_result;
}

UserResult get_users_by_status(int status) {
    // Prepare the SQL statement with appropriate placeholders for the values
    const char* select_query = "SELECT * FROM users WHERE status = $1";
    const char* values[1];
    char status_str[10];
    snprintf(status_str, sizeof(status_str), "%d", status);
    values[0] = status_str;
    PGresult* result = PQexecParams(pgconn, select_query, 1, NULL, values, NULL, NULL, 0);
    
    // Get the number of rows in the result set
    int num_rows = PQntuples(result);
    if (num_rows == 0) {
        UserResult user_result;
        user_result.users = NULL;
        user_result.num_users = 0;
        return user_result;
    }

    // Create an array of User structs
    User* users = malloc(num_rows * sizeof(User));

    // Iterate over the result set and populate the array
    for (int i = 0; i < num_rows; i++) {
        User user;
        
        // Retrieve the values from the result set
        user.user_id = atoi(PQgetvalue(result, i, 0));
        user.username = PQgetvalue(result, i, 1);
        user.password = PQgetvalue(result, i, 2);
        user.wrong_attempts = atoi(PQgetvalue(result, i, 3));
        user.status = atoi(PQgetvalue(result, i, 4));
        user.server_port = atoi(PQgetvalue(result, i, 5));
        user.ip = PQgetvalue(result, i, 6);

        // Assign the user struct to the array
        users[i] = user;
    }

    // Clear the result set
    PQclear(result);

    UserResult user_result;
    user_result.users = users;
    user_result.num_users = num_rows;
    return user_result;
}

User get_user_by_id(int user_id) {
    // Prepare the SQL statement with appropriate placeholders for the ID value
    const char* select_query = "SELECT * FROM users WHERE user_id = $1";
    
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
        User user = {0, "", "", 0, 0, 0};
        return user;
    }

    
    // Retrieve the values from the result set
    User user;
    user.user_id = atoi(PQgetvalue(result, 0, 0));
    user.username = PQgetvalue(result, 0, 1);
    user.password = PQgetvalue(result, 0, 2);
    user.wrong_attempts = atoi(PQgetvalue(result, 0, 3));
    user.status = atoi(PQgetvalue(result, 0, 4));
    user.server_port = atoi(PQgetvalue(result, 0, 5));
    user.ip = PQgetvalue(result, 0, 6);

    // Clear the result set
    PQclear(result);

    // Return the User struct
    return user;
}

User get_user_by_username(const char* username) {
    // Prepare the SQL statement with appropriate placeholders for the ID value
    const char* select_query = "SELECT * FROM users WHERE username = $1";
    
    // Create the parameter values array
    const char* values[1];
    values[0] = username;
    
    // Execute the prepared statement with the parameter values
    PGresult* result = PQexecParams(pgconn, select_query, 1, NULL, values, NULL, NULL, 0);

    // Get the number of rows in the result set
    int num_rows = PQntuples(result);
    if (num_rows == 0) {
        User user = {0, "", "", 0, 0, 0};
        return user;
    }

    
    // Retrieve the values from the result set
    User user;
    user.user_id = atoi(PQgetvalue(result, 0, 0));
    user.username = PQgetvalue(result, 0, 1);
    user.password = PQgetvalue(result, 0, 2);
    user.wrong_attempts = atoi(PQgetvalue(result, 0, 3));
    user.status = atoi(PQgetvalue(result, 0, 4));
    user.server_port = atoi(PQgetvalue(result, 0, 5));
    user.ip = PQgetvalue(result, 0, 6);

    // Clear the result set
    PQclear(result);

    // Return the User struct
    return user;
}

void update_user_by_id(int user_id, User new_user) {
    // Prepare the SQL statement with appropriate placeholders for the ID value and new values
    const char* update_query = "UPDATE users SET username = $1, password = $2, wrong_attempts = $3, status = $4, server_port = $5, ip = $6 WHERE user_id = $7";
    
    // Create the parameter values array
    const char* values[7];
    values[0] = new_user.username;
    values[1] = new_user.password;
    char wrong_attempts_str[10];
    snprintf(wrong_attempts_str, sizeof(wrong_attempts_str), "%d", new_user.wrong_attempts);
    values[2] = wrong_attempts_str;
    char status_str[10];
    snprintf(status_str, sizeof(status_str), "%d", new_user.status);
    values[3] = status_str;
    char server_port_str[10];
    snprintf(server_port_str, sizeof(server_port_str), "%d", new_user.server_port);
    values[4] = server_port_str;
    values[5] = new_user.ip;
    char user_id_str[10];
    snprintf(user_id_str, sizeof(user_id_str), "%d", user_id);
    values[6] = user_id_str;
    
    // Execute the prepared statement with the parameter values
    PGresult* result = PQexecParams(pgconn, update_query, 7, NULL, values, NULL, NULL, 0);
    PQclear(result);
}

void delete_user_by_id(int user_id) {
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

void delete_all_users() {
    // Execute a DELETE query to remove all records from the table
    const char* delete_query = "DELETE FROM users";
    PGresult* result = PQexec(pgconn, delete_query);
    PQclear(result);
}
