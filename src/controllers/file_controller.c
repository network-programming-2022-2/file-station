#include "file_controller.h"

bool upload_file(const char* filename, const char* username)
{
  User user = get_user_by_username(username);
  if (user.user_id == 0)
    return false;

  FileResult files = get_files_by_user_id(user.user_id);
  for (int i = 0; i < files.num_files; i++)
    if (strcmp(files.files[i].filename, filename) == 0)
      return true;

  File file = {0,(char*) filename, user.user_id, 0};
  
  insert_file(file);
  return true;
}

bool delete_file(const char* filename, const char* username)
{
  User user = get_user_by_username(username);
  if (user.user_id == 0)
    return false;

  File file = get_file_by_filename_and_user_id(filename, user.user_id);
  if (file.file_id == 0)
    return false;
  delete_file_by_id(file.file_id);

  return true;
}

bool update_file_name(const char* old_filename, const char* filename, const char* username)
{
  User user = get_user_by_username(username);
  if (user.user_id == 0)
    return false;

  File file = get_file_by_filename_and_user_id(old_filename, user.user_id);
  if (file.file_id == 0)
    return false;

  strcpy(file.filename, filename);
  update_file_by_id(file.file_id, file);
  return true;
}

bool search_files(const char* filename, SearchResult result[], int *num_files)
{
  FileResult files = get_files_by_name(filename); 

  printf("%d files found\n", files.num_files);
  if (files.num_files == 0)
    return false;

  for (int i = 0; i < files.num_files; i++)
  {
    printf("File: %s\n", files.files[i].filename);
  }

  for (int i = 0; i < files.num_files; i++)
  {
    strcpy(result[i].filename, filename);
    result[i].file_id = files.files[i].file_id;
    result[i].downloaded_numbers = files.files[i].downloaded_numbers;

    User user = get_user_by_id(files.files[i].user_id);
    if (user.user_id == 0)
      continue;

    strcpy(result[i].username, user.username);
    strcpy(result[i].ip, user.ip);
    result[i].port = user.server_port;

    printf("User: %s\n", result[i].username);
    printf("IP: %s\n", result[i].ip);
    printf("Port: %d\n", result[i].port);
    printf("File ID: %d\n", result[i].file_id);
    printf("File name: %s\n", result[i].filename);
    printf("Downloaded numbers: %d\n", result[i].downloaded_numbers);
    printf("New file added\n");
  }

  *num_files = files.num_files;
  return true;
}

