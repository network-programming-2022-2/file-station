#ifndef USER_H
#define USER_H

#include "../../includes/common.h"

// Function to insert a new user record into the table
void insert_user(User user);

// Function to retrieve all user records from the table
UserResult get_all_users();

// Function to retrieve user rocords from the table by account status
UserResult get_users_by_status(int status);

// Function to retrieve a specific user record from the table by its ID
User get_user_by_id(int user_id);

// Function to retrieve a specific user record from the table by its username
User get_user_by_username(const char* username);

// Function to update a specific user record in the table by its ID
void update_user_by_id(int user_id, User new_user);

// Function to delete a specific user record from the table by its ID
void delete_user_by_id(int user_id);

// Function to delete all user records from the table
void delete_all_users();

#endif // USER_H
