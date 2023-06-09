#include <stdio.h>
#include <stdlib.h>
#include "models/database.h"

int main() {
    // Create a DatabaseConnection struct for PostgreSQL
    DatabaseConnection connection;
    connection.type = POSTGRESQL;
    connection.host = "localhost";
    connection.port = "5432";
    connection.dbname = "postgres";
    connection.user = "postgres";
    connection.password = "";
    
    // Connect to the PostgreSQL database
    void* conn = connect_to_database(&connection);
    if (conn == NULL) {
        fprintf(stderr, "Failed to connect to the database.\n");
        return 1;
    }
    
    // Execute a query
    const char* query = "SELECT * FROM users";
    void* result = execute_query(&connection, conn, query);
    if (result == NULL) {
        fprintf(stderr, "Failed to execute the query.\n");
        close_connection(&connection, conn);
        return 1;
    }
    
    // Retrieve the number of rows and columns
    int row_count = get_row_count(&connection, result);
    int col_count = get_column_count(&connection, result);
    printf("Number of rows: %d\n", row_count);
    printf("Number of columns: %d\n", col_count);
    
    // Retrieve column names
    for (int col = 0; col < col_count; col++) {
        const char* col_name = get_column_name(&connection, result, col);
        printf("Column %d name: %s\n", col, col_name);
    }
    
    // Retrieve cell values
    for (int row = 0; row < row_count; row++) {
        for (int col = 0; col < col_count; col++) {
            const char* value = get_cell_value(&connection, result, row, col);
            printf("Row %d, Column %d: %s\n", row, col, value);
        }
    }
    
    // Close the connection and release resources
    close_connection(&connection, conn);
    
    return 0;
}
