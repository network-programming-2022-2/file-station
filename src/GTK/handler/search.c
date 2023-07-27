#include "../main.h"

bool handle_search(int client_sock, const char* delimiter, char username[], char filename[], SearchResult* result, int* num_files)
{
  char buff[BUFF_SIZE];
  const char *search_array[4] = { "5", "search", username, filename };
  int size = sizeof(search_array) / sizeof(search_array[0]);

  char* constructed_str = construct_string(search_array, size, delimiter);

  printf("Send: %s\n", constructed_str);
  int bytes_sent = send(client_sock, constructed_str, strlen(constructed_str), 0);
  if (bytes_sent < 0) {
    printf("\nError! Cannot send data to server! Client exits immediately!\n");
    exit(1);
  }

  int bytes_received = recv(client_sock, buff, BUFF_SIZE, 0);
  if (bytes_received < 0) {
    printf("\nError! Cannot receive data from server! Client exits immediately!\n");
    exit(1);
  }
  buff[bytes_received] = '\0';

  printf("Receive: %s\n", buff);
  if (strcmp(buff, "[server]: File not found!\n") == 0)
  {
    return false;
  }

  int received_count, count;
  char** result_list = extract_information(buff, &received_count, "\\");
  *num_files = received_count;

  for (int i = 0; i < received_count; i++)
  {
    char** fields = extract_information(result_list[i], &count, ":");

    result[i].file_id = atoi(fields[0]);
    result[i].downloaded_numbers = atoi(fields[2]);
    result[i].port = atoi(fields[5]);
    strcpy(result[i].filename, fields[1]);
    strcpy(result[i].username, fields[3]);
    strcpy(result[i].ip, fields[4]);  

    printf("User: %s\n", result[i].username);
    printf("IP: %s\n", result[i].ip);
    printf("Port: %d\n", result[i].port);
    printf("File ID: %d\n", result[i].file_id);
    printf("File name: %s\n", result[i].filename);
    printf("Downloaded numbers: %d\n", result[i].downloaded_numbers);
  }

  return true;
}

