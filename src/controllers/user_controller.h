#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H 

#include "../models/postgresql/user.h"
#include "../models/postgresql/postgresql.h"

// Function to register a new user
bool register_user(const char* username, const char* password, const char* server_port, const char* ip);

// Function to check if a username is available
bool is_username_available(const char* username);

// Function to login a user
// bool login_user(const char* username, const char* password);

// Function to check if the username and password are correct
bool check_credentials(const char* username, const char* password);

// Function to get user information by username
// User get_user_by_username(const char* username);

// Function to get user information by user ID
// User get_user_by_id(int user_id);

// Function to update user information
// bool update_user(User user);

// Function to delete a user
// bool delete_user(int user_id);

// Function to change a user's password
bool change_password(int user_id, const char* old_password, const char* new_password);

#endif  // USER_CONTROLLER_H

