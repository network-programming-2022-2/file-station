#include "postgresql.h"

// Function to establish a PostgreSQL connection
PGconn* connect_to_postgresql(const DatabaseConnection* connection)
{
  char connection_string[1000];
  snprintf(connection_string, sizeof(connection_string), "host=%s port=%s user=%s password=%s dbname=%s",
        connection->host, connection->port, connection->user, connection->password, connection->dbname);
  
  PGconn* conn = PQconnectdb(connection_string);

  if (PQstatus(conn) != CONNECTION_OK) {
		fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
    PQfinish(conn);
    exit(1);
	}

  return conn;
}

// Function to initialize a PostgreSQL database
void initialize_postgresql()
{
    const char* createUsersTableQuery = 
      "CREATE TABLE IF NOT EXISTS users (user_id SERIAL PRIMARY KEY, username VARCHAR(255), password VARCHAR(255), wrong_attempts INT, status INT, server_port INT, ip VARCHAR(50))";
    PGresult* result = PQexec(pgconn, createUsersTableQuery);
    PQclear(result);
    
    const char* createFilesTableQuery = "CREATE TABLE IF NOT EXISTS files (file_id SERIAL PRIMARY KEY, filename VARCHAR(255), user_id INT, downloaded_numbers INT, FOREIGN KEY (user_id) REFERENCES users (user_id) ON DELETE CASCADE)";
    result = PQexec(pgconn, createFilesTableQuery);
    PQclear(result);
}

// Function to execute a PostgreSQL query
PGresult* execute_query_postgresql(const char* query) 
{
  PGresult* result = PQexec(pgconn, query);
  
  if (PQresultStatus(result) != PGRES_COMMAND_OK && PQresultStatus(result) != PGRES_TUPLES_OK) 
  {
    fprintf(stderr, "Query execution failed: %s\n", PQerrorMessage(pgconn));
    PQclear(result);
    return NULL;
  }
    
  return result;
}

// Function to retrieve the number of rows returned by a PostgreSQL query
int get_row_count_postgresql(PGresult* result)
{
  return PQntuples(result);
}

// Function to retrieve the number of columns returned by a PostgreSQL query
int get_column_count_postgresql(PGresult* result)
{
  return PQnfields(result);
}

// Function to retrieve the name of a column in a PostgreSQL query result
const char* get_column_name_postgresql(PGresult* result, int column_number)
{
  return PQfname(result, column_number);
}

// Function to retrieve the value of a specific cell in a PostgreSQL query result
const char* get_cell_value_postgresql(PGresult* result, int row_number, int column_number)
{
  return PQgetvalue(result, row_number, column_number);
}

// Function to drop all PostgreSQL tables
void drop_all_postgresql_tables()
{
  const char* dropUsersTableQuery = "DROP TABLE IF EXISTS users";
  const char* dropFilesTableQuery = "DROP TABLE IF EXISTS files";
  execute_query_postgresql(dropFilesTableQuery);
  execute_query_postgresql(dropUsersTableQuery);
}

// Function to release resources and close the PostgreSQL connection
void close_postgresql_connection()
{
  PQfinish(pgconn);
}
