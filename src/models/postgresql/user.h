#ifndef USER_H
#define USER_H

#include "common.h"

typedef struct {
    int user_id;
    char* username;
    char* password;
    int wrong_attempts;
    int status;
    int is_login;
} User;

// Function to insert a new user record into the table
void insert_user(PGconn* conn, User user);

// Function to retrieve all user records from the table
PGresult* get_all_users(PGconn* conn);

// Function to retrieve user rocords from the table by account status
PGresult* get_users_by_status(PGconn* conn, int status);

// Function to retrieve user rocords from the table by account status and is_login
PGresult* get_users_by_status_and_is_login(PGconn* conn, int status, int is_login);

// Function to retrieve a specific user record from the table by its ID
PGresult* get_user_by_id(PGconn* conn, int user_id);

// Function to retrieve a specific user record from the table by its username
PGresult* get_user_by_username(PGconn* conn, const char* username);

// Function to update a specific user record in the table by its ID
void update_user_by_id(PGconn* conn, int user_id, User new_user);

// Function to delete a specific user record from the table by its ID
void delete_user_by_id(PGconn* conn, int user_id);

// Function to delete all user records from the table
void delete_all_users(PGconn* conn);

#endif // USER_H
