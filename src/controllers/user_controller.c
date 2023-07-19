#include "user_controller.h"

// Function to register a new user
bool register_user(const char* username, const char* password, const char* server_port, const char* ip)
{
  bool is_available = is_username_available(username);
  if (!is_available)
  {
    return false;
  }

  User user;
  strcpy(user.username, username);
  strcpy(user.password, password);
  user.wrong_attempts = 0;
  user.status = 0;
  int i_server_port = atoi(server_port);
  user.server_port = i_server_port;
  strcpy(user.ip, ip);
  insert_user(user);
  return true;
}

// Function to check if a username is available
bool is_username_available(const char* username)
{
  User user;
  user = get_user_by_username(username);
  if (user.user_id != 0)
  {
    return false;
  }
  return true;
}

bool check_credentials(const char* username, const char* password)
{
  User user;
  user = get_user_by_username(username);
  if (user.user_id == 0)
  {
    return false;
  }
  if (strcmp(user.password, password) == 0)
  {
    return true;
  }
  return false;
}

// Function to change a user's password
bool change_password(int user_id, const char* old_password, const char* new_password)
{
  User user;
  user = get_user_by_id(user_id);
  if (user.user_id == 0)
  {
    return false;
  }
  if (strcmp(user.password, old_password) == 0)
  {
    strcpy(user.password, new_password);
    update_user_by_id(user.user_id, user);
    return true;
  }
  return false;
}

