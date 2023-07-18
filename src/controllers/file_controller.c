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
