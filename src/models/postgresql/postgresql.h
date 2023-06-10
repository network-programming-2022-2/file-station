#ifndef POSTGRESQL_H
#define POSTGRESQL_H

#include "common.h"

// Function to establish a PostgreSQL connection
PGconn* connect_to_postgresql(const DatabaseConnection* connection);

// Function to initialize a PostgreSQL database
void initialize_postgresql(PGconn* conn);

// Function to execute a PostgreSQL query
PGresult* execute_query_postgresql(PGconn* conn, const char* query);

// Function to retrieve the number of rows returned by a PostgreSQL query
int get_row_count_postgresql(PGresult* result);

// Function to retrieve the number of columns returned by a PostgreSQL query
int get_column_count_postgresql(PGresult* result);

// Function to retrieve the name of a column in a PostgreSQL query result
const char* get_column_name_postgresql(PGresult* result, int column_number);

// Function to retrieve the value of a specific cell in a PostgreSQL query result
const char* get_cell_value_postgresql(PGresult* result, int row_number, int column_number);

// Function to release resources and close the PostgreSQL connection
void close_postgresql_connection(PGconn* conn);

#endif  // POSTGRESQL_H
