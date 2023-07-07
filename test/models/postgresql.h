#include <stdio.h>
#include <stdlib.h>
#include "../../src/models/postgresql/postgresql.h"
#include "../../src/models/postgresql/file.h"
#include "../../src/models/postgresql/user.h"

void initialize_database();
void cleanup_and_close_database();

// File Manager Tests
void insert_file_test();
void get_all_files_test();
void get_file_by_name_test();
void get_file_by_id_test();
void update_file_by_id_test();
void delete_file_by_id_test();
void test_file_manager();

// User Manager Tests
void insert_user_test();
void get_all_users_test();
void get_user_by_id_test();
void get_user_by_username_test();
void update_user_by_id_test();
void delete_user_by_id_test();
void test_user_manager();
