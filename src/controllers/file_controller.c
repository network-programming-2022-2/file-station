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
