#include <stdio.h>
#include <stdlib.h>
#include "../../src/models/postgresql/postgresql.h"
#include "../../src/models/postgresql/file.h"
#include "../../src/models/postgresql/user.h"

void test_user_management(PGconn* pgconn) {
    // Insert sample users
    User user1 = {1, "user1", "password1", 0, 1, 1};
    User user2 = {2, "user2", "password2", 0, 1, 1};
    User user3 = {3, "user3", "password3", 0, 1, 1};
    User user4 = {4, "user4", "password4", 0, 1, 1};

    insert_user(pgconn, user1);
    insert_user(pgconn, user2);
    insert_user(pgconn, user3);
    insert_user(pgconn, user4);

    // Get all users
    PGresult* all_users_result = get_all_users(pgconn);
    int row_count = get_row_count_postgresql(all_users_result);
    int column_count = get_column_count_postgresql(all_users_result);
    printf("All Users:\n");
    for (int i = 0; i < row_count; i++) {
        for (int j = 0; j < column_count; j++) {
            const char* column_name = get_column_name_postgresql(all_users_result, j);
            const char* cell_value = get_cell_value_postgresql(all_users_result, i, j);
            printf("%s: %s\t", column_name, cell_value);
        }
        printf("\n");
    }
    PQclear(all_users_result);

    // Get user by ID
    int target_user_id = 3;
    PGresult* user_by_id_result = get_user_by_id(pgconn, target_user_id);
    row_count = get_row_count_postgresql(user_by_id_result);
    column_count = get_column_count_postgresql(user_by_id_result);
    printf("\nUser with ID %d:\n", target_user_id);
    for (int i = 0; i < row_count; i++) {
        for (int j = 0; j < column_count; j++) {
            const char* column_name = get_column_name_postgresql(user_by_id_result, j);
            const char* cell_value = get_cell_value_postgresql(user_by_id_result, i, j);
            printf("%s: %s\t", column_name, cell_value);
        }
        printf("\n");
    }
    PQclear(user_by_id_result);

    // Update user by ID
    int update_user_id = 2;
    User updated_user = {update_user_id, "new_username", "new_password", 2, 0, 0};
    update_user_by_id(pgconn, update_user_id, updated_user);

    // Get updated user by ID
    PGresult* updated_user_result = get_user_by_id(pgconn, update_user_id);
    row_count = get_row_count_postgresql(updated_user_result);
    column_count = get_column_count_postgresql(updated_user_result);
    printf("\nUpdated User with ID %d:\n", update_user_id);
    for (int i = 0; i < row_count; i++) {
        for (int j = 0; j < column_count; j++) {
            const char* column_name = get_column_name_postgresql(updated_user_result, j);
            const char* cell_value = get_cell_value_postgresql(updated_user_result, i, j);
            printf("%s: %s\t", column_name, cell_value);
        }
        printf("\n");
    }
    PQclear(updated_user_result);

    // Delete user by ID
    int delete_user_id = 1;
    delete_user_by_id(pgconn, delete_user_id);

    // Get all users after deletion
    all_users_result = get_all_users(pgconn);
    row_count = get_row_count_postgresql(all_users_result);
    column_count = get_column_count_postgresql(all_users_result);
    printf("\nAll Users after deletion:\n");
    for (int i = 0; i < row_count; i++) {
        for (int j = 0; j < column_count; j++) {
            const char* column_name = get_column_name_postgresql(all_users_result, j);
            const char* cell_value = get_cell_value_postgresql(all_users_result, i, j);
            printf("%s: %s\t", column_name, cell_value);
        }
        printf("\n");
    }
    PQclear(all_users_result);
}

void test_file_management(PGconn* pgconn) {
    // Insert sample files
    File file1 = {1, "file1.txt", 1, 1};
    File file2 = {2, "file2.txt", 2, 2};
    File file3 = {3, "file3.txt", 3, 3};

    insert_file(pgconn, file1);
    insert_file(pgconn, file2);
    insert_file(pgconn, file3);

    // Get all files
    PGresult* all_files_result = get_all_files(pgconn);
    int row_count = get_row_count_postgresql(all_files_result);
    int column_count = get_column_count_postgresql(all_files_result);
    printf("\nAll Files:\n");
    for (int i = 0; i < row_count; i++) {
        for (int j = 0; j < column_count; j++) {
            const char* column_name = get_column_name_postgresql(all_files_result, j);
            const char* cell_value = get_cell_value_postgresql(all_files_result, i, j);
            printf("%s: %s\t", column_name, cell_value);
        }
        printf("\n");
    }
    PQclear(all_files_result);

    // Delete file by ID
    int delete_file_id = 2;
    delete_file_by_id(pgconn, delete_file_id);

    // Get all files after deletion
    all_files_result = get_all_files(pgconn);
    row_count = get_row_count_postgresql(all_files_result);
    column_count = get_column_count_postgresql(all_files_result);
    printf("\nAll Files after deletion:\n");
    for (int i = 0; i < row_count; i++) {
        for (int j = 0; j < column_count; j++) {
            const char* column_name = get_column_name_postgresql(all_files_result, j);
            const char* cell_value = get_cell_value_postgresql(all_files_result, i, j);
            printf("%s: %s\t", column_name, cell_value);
        }
        printf("\n");
    }
    PQclear(all_files_result);
}

int main() {
    // Connect to the PostgreSQL database
    DatabaseConnection connection;
    connection.host = "localhost";
    connection.port = "5432";
    connection.user = "postgres";
    connection.password = "";
    connection.dbname = "postgres";
    PGconn* pgconn = connect_to_postgresql(&connection);

    // Initialize the database tables
    initialize_postgresql(pgconn);

    // Test user management
    test_user_management(pgconn);

    // Test file management
    test_file_management(pgconn);

    // Drop the database tables
    drop_all_postgresql_tables(pgconn);
  
    // Cleanup and close the database connection
    close_postgresql_connection(pgconn);

    return 0;
}

