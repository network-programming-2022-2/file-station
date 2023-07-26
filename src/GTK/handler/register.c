#include "../main.h"

bool handle_registration(const char* delimiter, const char* password, InotifyThreadArgs inotify_args)
{
  char buff[BUFF_SIZE];
  const char *register_array[6] = { "1", "register", inotify_args.username, password, inotify_args.server_port, inotify_args.ip };
  int size = sizeof(register_array) / sizeof(register_array[0]);

  char* constructed_str = construct_string(register_array, size, delimiter);

  printf("%s %d\n", constructed_str, inotify_args.client_sock);
  int bytes_sent = send(inotify_args.client_sock, constructed_str, strlen(constructed_str), 0);
  if (bytes_sent < 0) {
    printf("\nError! Cannot send data to server! Client exits immediately!\n");
    exit(1);
  }

  int bytes_received = recv(inotify_args.client_sock, buff, BUFF_SIZE, 0);
  if (bytes_received < 0) {
    printf("\nError! Cannot receive data from server! Client exits immediately!\n");
    exit(1);
  }
  buff[bytes_received] = '\0';
  if (strcmp(buff, "[server]: Registered successfully!\n") == 0)
  {
    return true;
  }
  return false;
}

