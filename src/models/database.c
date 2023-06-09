#include "database.h"
#include "postgresql/postgresql.h" 

void* connect_to_database(const DatabaseConnection* connection) 
{
  // Connect to the database based on the chosen database system
  if (connection->type == POSTGRESQL) 
  {
    return (void*)connect_to_postgresql(connection);
  } 
  return NULL;
}

void* execute_query(const DatabaseConnection* connection, void* conn, const char* query) 
{
  // Execute the query based on the chosen database system
  if (connection->type == POSTGRESQL) 
  {
    return (void*)execute_query_postgresql((PGconn*)conn, query);
  } 
  return NULL;
}

int get_row_count(const DatabaseConnection* connection, void* conn) 
{
  // Get the row count based on the chosen database system
  if (connection->type == POSTGRESQL) 
  {
    return get_row_count_postgresql((PGresult*)conn);
  } 
  return 0;
}

int get_column_count(const DatabaseConnection* connection, void* conn) 
{
  // Get the column count based on the chosen database system
  if (connection->type == POSTGRESQL) 
  {
    return get_column_count_postgresql((PGresult*)conn);
  } 
  return 0;
}

const char* get_column_name(const DatabaseConnection* connection, void* conn, int column_number) 
{
  // Get the column name based on the chosen database system
  if (connection->type == POSTGRESQL) 
  {
    return get_column_name_postgresql((PGresult*)conn, column_number);
  } 
  return NULL;
}

const char* get_cell_value(const DatabaseConnection* connection, void* conn, int row_number, int column_number) 
{
  // Get the cell value based on the chosen database system
  if (connection->type == POSTGRESQL) 
  {
    return get_cell_value_postgresql((PGresult*)conn, row_number, column_number);
  } 
  return NULL;
}

void close_connection(const DatabaseConnection* connection, void* conn) 
{
  // Close the connection and release resources based on the chosen database system
  if (connection->type == POSTGRESQL) 
  {
    close_postgresql_connection((PGconn*)conn);
  } 
}
