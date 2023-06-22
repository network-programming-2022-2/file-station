#include <stdio.h>
#include <stdlib.h>
#include "../../src/models/postgresql/postgresql.h"
#include "../../src/models/postgresql/file.h"
#include "../../src/models/postgresql/user.h"

PGconn* pgconn = NULL;

void test_file_manager() {
    // Test data
    File test_files[] = {
        {1, "file1.txt", 1, 10},
        {2, "file2.txt", 2, 5},
        {3, "file3.txt", 1, 3},
        {4, "file4.txt", 3, 8}
    };
    int num_files = sizeof(test_files) / sizeof(File);

    // Test insert_file() function
    printf("Testing insert_file() function...\n");
    for (int i = 0; i < num_files; i++) {
        insert_file(test_files[i]);
    }

    // Test get_all_files() function
    printf("Testing get_all_files() function...\n");
    File* all_files = get_all_files();
    for (int i = 0; i < num_files; i++) {
        printf("File: %d, %s, %d, %d\n", all_files[i].file_id, all_files[i].filename,
               all_files[i].user_id, all_files[i].downloaded_number);
    }
    free(all_files);

    // Test get_file_by_name() function
    printf("Testing get_file_by_name() function...\n");
    const char* filename = "file2.txt";
    File* file_by_name = get_file_by_name(filename);
    num_files = sizeof(file_by_name) / sizeof(File);
    for (int i = 0; i < num_files; i++) {
        printf("File: %d, %s, %d, %d\n", file_by_name[i].file_id, file_by_name[i].filename,
               file_by_name[i].user_id, file_by_name[i].downloaded_number);
    }
    free(file_by_name);

    // Test get_file_by_id() function
    printf("Testing get_file_by_id() function...\n");
    int file_id = 3;
    File file_by_id = get_file_by_id(file_id);
    printf("File by ID: %d, %s, %d, %d\n", file_by_id.file_id, file_by_id.filename,
           file_by_id.user_id, file_by_id.downloaded_number);

    // Test update_file_by_id() function
    printf("Testing update_file_by_id() function...\n");
    File updated_file = {3, "updated_file3.txt", 2, 15};
    update_file_by_id(file_id, updated_file);
    file_by_id = get_file_by_id(file_id);
    printf("Updated File by ID: %d, %s, %d, %d\n", file_by_id.file_id, file_by_id.filename,
           file_by_id.user_id, file_by_id.downloaded_number);

    // Test delete_file_by_id() function
    printf("Testing delete_file_by_id() function...\n");
    delete_file_by_id(file_id);
    file_by_id = get_file_by_id(file_id);
    if (file_by_id.file_id == 0) {
        printf("File with ID %d deleted successfully.\n", file_id);
    } else {
        printf("ERROR: File deletion failed.\n");
    }

    printf("File manager tests completed.\n");
}

void test_user_manager() {
    // Test data
    User test_users[] = {
        {1, "user1", "password1", 0, 1, 1},
        {2, "user2", "password2", 0, 1, 1},
        {3, "user3", "password3", 0, 1, 0},
        {4, "user4", "password4", 0, 0, 0}
    };
    int num_users = sizeof(test_users) / sizeof(User);

    // Test insert_user() function
    printf("Testing insert_user() function...\n");
    for (int i = 0; i < num_users; i++) {
        insert_user(test_users[i]);
    }

    // Test get_all_users() function
    printf("Testing get_all_users() function...\n");
    User* all_users = get_all_users();
    for (int i = 0; i < num_users; i++) {
        printf("User: %d, %s, %s, %d, %d, %d\n", all_users[i].user_id, all_users[i].username,
               all_users[i].password, all_users[i].wrong_attempts, all_users[i].status,
               all_users[i].is_login);
    }
    free(all_users);

    // Test get_user_by_id() function
    printf("Testing get_user_by_id() function...\n");
    int user_id = 2;
    User user_by_id = get_user_by_id(user_id);
    printf("User by ID: %d, %s, %s, %d, %d, %d\n", user_by_id.user_id, user_by_id.username,
           user_by_id.password, user_by_id.wrong_attempts, user_by_id.status,
           user_by_id.is_login);

    // Test get_user_by_username() function
    printf("Testing get_user_by_username() function...\n");
    const char* username = "user3";
    User user_by_username = get_user_by_username(username);
    printf("User by username: %d, %s, %s, %d, %d, %d\n", user_by_username.user_id,
           user_by_username.username, user_by_username.password, user_by_username.wrong_attempts,
           user_by_username.status, user_by_username.is_login);

    // Test update_user_by_id() function
    printf("Testing update_user_by_id() function...\n");
    User updated_user = {3, "user3_updated", "updated_password3", 2, 0, 0};
    update_user_by_id(user_id, updated_user);
    user_by_id = get_user_by_id(user_id);
    printf("Updated User by ID: %d, %s, %s, %d, %d, %d\n", user_by_id.user_id, user_by_id.username,
           user_by_id.password, user_by_id.wrong_attempts, user_by_id.status,
           user_by_id.is_login);

    // Test delete_user_by_id() function
    // printf("Testing delete_user_by_id() function...\n");
    // delete_user_by_id(user_id);
    // user_by_id = get_user_by_id(user_id);
    // if (user_by_id.user_id == 0) {
    //     printf("User with ID %d deleted successfully.\n", user_id);
    // } else {
    //     printf("ERROR: User deletion failed.\n");
    // }

    printf("User manager tests completed.\n");
}

int main() {
    // Connect to the PostgreSQL database
    DatabaseConnection connection;
    connection.host = "localhost";
    connection.port = "5432";
    connection.user = "postgres";
    connection.password = "";
    connection.dbname = "postgres";
    pgconn = connect_to_postgresql(&connection);

    // Initialize the database tables
    initialize_postgresql();

    // Test user management
    test_user_manager();

    // Test file management
    test_file_manager();

    // Drop the database tables
    drop_all_postgresql_tables();
  
    // Cleanup and close the database connection
    close_postgresql_connection();

    return 0;
}

