#include "postgresql.h"

PGconn* pgconn = NULL;

int main() {
  DatabaseConnection connection;
  connection.host = "localhost";
  connection.port = "5432";
  connection.user = "postgres";
  connection.password = "";
  connection.dbname = "postgres";
  pgconn = connect_to_postgresql(&connection);

  initialize_database();

  test_user_manager();
  test_file_manager();

  // cleanup_and_close_database();

  return 0;
}
