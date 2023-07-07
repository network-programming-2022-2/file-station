#include "postgresql.h"

void initialize_database() {
    // Initialize the database tables
    initialize_postgresql();
}

void cleanup_and_close_database() {
    // Drop the database tables
    drop_all_postgresql_tables();

    // Cleanup and close the database connection
    close_postgresql_connection();
}

void insert_file_test() {
    File test_files[] = {
        {1, "file1.txt", 1, 10},
        {2, "file2.txt", 2, 5},
        {3, "file3.txt", 1, 3},
        {4, "file4.txt", 3, 8}
    };
    int num_files = sizeof(test_files) / sizeof(File);

    printf("Testing insert_file() function...\n");
    for (int i = 0; i < num_files; i++) {
        insert_file(test_files[i]);
    }
    FileResult all_files = get_all_files();
    for (int i = 0; i < all_files.num_files; i++) {
        printf("File: %d, %s, %d, %d\n", all_files.files[i].file_id, all_files.files[i].filename,
               all_files.files[i].user_id, all_files.files[i].downloaded_numbers);
    }

}

void get_all_files_test() {
    printf("Testing get_all_files() function...\n");
    FileResult all_files = get_all_files();
    for (int i = 0; i < all_files.num_files; i++) {
        printf("File: %d, %s, %d, %d\n", all_files.files[i].file_id, all_files.files[i].filename,
               all_files.files[i].user_id, all_files.files[i].downloaded_numbers);
    }
    free(all_files.files);
}

void get_file_by_name_test() {
    printf("Testing get_file_by_name() function...\n");
    const char* filename = "file2.txt";
    FileResult file_by_name = get_file_by_name(filename);
    int num_files = file_by_name.num_files;
    for (int i = 0; i < num_files; i++) {
        printf("File: %d, %s, %d, %d\n", file_by_name.files[i].file_id, file_by_name.files[i].filename,
               file_by_name.files[i].user_id, file_by_name.files[i].downloaded_numbers);
    }
    free(file_by_name.files);
}

void get_file_by_id_test() {
    printf("Testing get_file_by_id() function...\n");
    int file_id = 3;
    File file_by_id = get_file_by_id(file_id);
    printf("File by ID: %d, %s, %d, %d\n", file_by_id.file_id, file_by_id.filename,
           file_by_id.user_id, file_by_id.downloaded_numbers);
}

void update_file_by_id_test() {
    printf("Testing update_file_by_id() function...\n");
    int file_id = 3;
    File updated_file = {3, "updated_file3.txt", 2, 15};
    update_file_by_id(file_id, updated_file);
    File file_by_id = get_file_by_id(file_id);
    printf("Updated File by ID: %d, %s, %d, %d\n", file_by_id.file_id, file_by_id.filename,
           file_by_id.user_id, file_by_id.downloaded_numbers);
}

void delete_file_by_id_test() {
    printf("Testing delete_file_by_id() function...\n");
    int file_id = 3;
    delete_file_by_id(file_id);
    File file_by_id = get_file_by_id(file_id);
    if (file_by_id.file_id == 0) {
        printf("File with ID %d deleted successfully.\n", file_id);
    } else {
        printf("ERROR: File deletion failed.\n");
    }
}

void test_file_manager() {
    insert_file_test();
    // get_all_files_test();
    // get_file_by_name_test();
    // get_file_by_id_test();
    // update_file_by_id_test();
    // delete_file_by_id_test();
  
    printf("File manager tests completed.\n");
}

void insert_user_test() {
    User test_users[] = {
        {1, "user1", "password1", 0, 1, 1},
        {2, "user2", "password2", 0, 1, 1},
        {3, "user3", "password3", 0, 1, 0},
        {4, "user4", "password4", 0, 0, 0}
    };
    int num_users = sizeof(test_users) / sizeof(User);

    printf("Testing insert_user() function...\n");
    for (int i = 0; i < num_users; i++) {
        insert_user(test_users[i]);
    }
}

void get_all_users_test() {
    printf("Testing get_all_users() function...\n");
    UserResult all_users = get_all_users();
    int num_users = all_users.num_users;
    for (int i = 0; i < num_users; i++) {
        printf("User: %d, %s, %s, %d, %d, %d\n", all_users.users[i].user_id, all_users.users[i].username,
               all_users.users[i].password, all_users.users[i].wrong_attempts, all_users.users[i].status,
               all_users.users[i].is_login);
    }
    free(all_users.users);
}

void get_user_by_id_test() {
    printf("Testing get_user_by_id() function...\n");
    int user_id = 2;
    User user_by_id = get_user_by_id(user_id);
    printf("User by ID: %d, %s, %s, %d, %d, %d\n", user_by_id.user_id, user_by_id.username,
           user_by_id.password, user_by_id.wrong_attempts, user_by_id.status,
           user_by_id.is_login);
}

void get_user_by_username_test() {
    printf("Testing get_user_by_username() function...\n");
    const char* username = "user3";
    User user_by_username = get_user_by_username(username);
    printf("User by username: %d, %s, %s, %d, %d, %d\n", user_by_username.user_id,
           user_by_username.username, user_by_username.password, user_by_username.wrong_attempts,
           user_by_username.status, user_by_username.is_login);
}

void update_user_by_id_test() {
    printf("Testing update_user_by_id() function...\n");
    int user_id = 2;
    User updated_user = {3, "user3_updated", "updated_password3", 2, 0, 0};
    update_user_by_id(user_id, updated_user);
    User user_by_id = get_user_by_id(user_id);
    printf("Updated User by ID: %d, %s, %s, %d, %d, %d\n", user_by_id.user_id, user_by_id.username,
           user_by_id.password, user_by_id.wrong_attempts, user_by_id.status,
           user_by_id.is_login);
}

void delete_user_by_id_test() {
    printf("Testing delete_user_by_id() function...\n");
    int user_id = 2;
    delete_user_by_id(user_id);
    User user_by_id = get_user_by_id(user_id);
    if (user_by_id.user_id == 0) {
        printf("User with ID %d deleted successfully.\n", user_id);
    } else {
        printf("ERROR: User deletion failed.\n");
    }
}

void test_user_manager() {
    insert_user_test();
    get_all_users_test();
    get_user_by_id_test();
    get_user_by_username_test();
    update_user_by_id_test();
    // delete_user_by_id_test();
  
    printf("User manager tests completed.\n");
}
