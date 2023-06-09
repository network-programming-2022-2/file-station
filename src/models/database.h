#ifndef DATABASE_H
#define DATABASE_H

#include "common.h"

// Function to establish a database connection
void* connect_to_database(const DatabaseConnection* connection);

// Function to execute a query
void* execute_query(const DatabaseConnection* connection, void* conn, const char* query);

// Function to retrieve the number of rows returned by a query
int get_row_count(const DatabaseConnection* connection, void* conn);

// Function to retrieve the number of columns returned by a query
int get_column_count(const DatabaseConnection* connection, void* conn);

// Function to retrieve the name of a column
const char* get_column_name(const DatabaseConnection* connection, void* conn, int column_number);

// Function to retrieve the value of a specific cell
const char* get_cell_value(const DatabaseConnection* connection, void* conn, int row_number, int column_number);

// Function to release resources and close the database connection
void close_connection(const DatabaseConnection* connection, void* conn);

#endif  // DATABASE_H

