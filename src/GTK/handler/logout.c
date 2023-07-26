#include "../main.h"

bool handle_logout(int client_sock, const char* delimiter, char username[])
{
  char buff[BUFF_SIZE];
  const char *logout_array[3] = { "4", "logout", username };
  int size = sizeof(logout_array) / sizeof(logout_array[0]);
  char* constructed_string = construct_string(logout_array, size, delimiter);

  int bytes_sent = send(client_sock, constructed_string, strlen(constructed_string), 0);
  if (bytes_sent < 0) {
    printf("\nError! Cannot send data to server! Client exits immediately!\n");
    return false;
  }

  int bytes_received = recv(client_sock, buff, BUFF_SIZE, 0);
  if (bytes_received < 0) {
    printf("\nError! Cannot receive data from server! Client exits immediately!\n");
    return false;
  }
  buff[bytes_received] = '\0';

  if (strcmp(buff, "[server]: Logout successfully!\n") == 0) {
    return true;
  }
  return false;
}

