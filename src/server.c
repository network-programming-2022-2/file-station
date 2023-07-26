#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <openssl/sha.h>
#include "models/postgresql/postgresql.h"
#include "controllers/user_controller.h"
#include "controllers/file_controller.h"
#include "models/postgresql/file.h"
#include "models/postgresql/user.h"

#define BACKLOG 20 /* Number of allowed connections */
#define BUFF_SIZE 1024
#define MAX_CLIENTS 100

typedef struct
{
  int connfd;
  char username[50];
  char filename[BUFF_SIZE];
  char old_filename[BUFF_SIZE];
  char method[BUFF_SIZE];
} Client;
void extract_username_passwd(char buffer[], char username[], char password[], char *delimiter);
void *login_handler(void *);
char **extract_information(const char *str, int *count);
void cleanup_and_close_database();
void *file_upload_handler(void *arg);

PGconn *pgconn = NULL;
Client clients[MAX_CLIENTS];
int client_index;

int main(int argc, char *argv[])
{
  int listenfd, *connfd;
  struct sockaddr_in server;  /* server's address information */
  struct sockaddr_in *client; /* client's address information */
  socklen_t sin_size;
  pthread_t tid;
  int port;

  if (argc != 2)
  {
    printf("[server]: The server needs to be binded with a port.\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    port = atoi(argv[1]);
  }

  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  { /* calls socket() */
    perror("\n[server]: ");
    return 0;
  }
  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = htonl(INADDR_ANY); /* INADDR_ANY puts your IP address automatically */

  if (bind(listenfd, (struct sockaddr *)&server, sizeof(server)) == -1)
  {
    perror("\n[server]: ");
    return 0;
  }

  if (listen(listenfd, BACKLOG) == -1)
  {
    perror("\n[server]: ");
    return 0;
  }

  sin_size = sizeof(struct sockaddr_in);
  client = malloc(sin_size);

  DatabaseConnection connection;
  connection.host = "127.0.0.1";
  connection.port = "5432";
  connection.user = "postgres";
  connection.password = "kytran";
  connection.dbname = "postgres";
  pgconn = connect_to_postgresql(&connection);

  initialize_postgresql();

  while (1)
  {
    connfd = malloc(sizeof(int));
    if ((*connfd = accept(listenfd, (struct sockaddr *)client, &sin_size)) == -1)
      perror("\n[server]: ");

    printf("[server]: You got a connection from %s - port: %d\n\n", inet_ntoa(client->sin_addr), (int)ntohs(client->sin_port)); /* prints client's IP */

    /* For each client, spawns a thread, and the thread handles the new client */
    pthread_create(&tid, NULL, &login_handler, connfd);
  }

  close(listenfd);

  cleanup_and_close_database();
  return 0;
}

void delete_client(Client arr[], int size, int index)
{
  if (index < 0 || index >= size)
  {
    printf("Invalid index for deletion.\n");
    return;
  }

  for (int i = index; i < size - 1; i++)
  {
    arr[i] = arr[i + 1];
  }
}

void *login_handler(void *arg)
{
  int connfd;
  int bytes_sent, bytes_received;
  char buff[BUFF_SIZE];
  char username[50], password[50], message[100];
  int count;
  bool ok, is_login;

  connfd = *((int *)arg);
  free(arg);
  pthread_detach(pthread_self());

  while (1)
  {
    // 1:register:username:password
    // 2:login:username:password
    // 5:search:filename
    bytes_received = recv(connfd, buff, BUFF_SIZE, 0);
    if (bytes_received < 0)
      perror("\n[server]: ");
    else if (bytes_received == 0)
      printf("[server]: Connection closed.");
    buff[bytes_received] = '\0';

    printf("[server]: Received client choice: %s\n\n", buff);
    char **info_array = extract_information(buff, &count);

    int choice = atoi(info_array[0]);
    //
    int is_logged_in;
    int check;
    char reply_search[100];

    //
    switch (choice)
    {
    case 1:
      ok = register_user(info_array[2], info_array[3], info_array[4], info_array[5]);
      if (!ok)
      {
        strcpy(message, "[server]: Username already exists!\n");
      }
      else
      {
        strcpy(message, "[server]: Registered successfully!\n");
      }
      bytes_sent = send(connfd, message, BUFF_SIZE, 0);
      if (bytes_sent < 0)
        perror("\n[server]: ");

      printf("%s\n\n", message);
      break;

    case 2:
      is_logged_in = 0;
      for (int i = 0; i < client_index; i++)
      {
        if (strcmp(clients[i].username, info_array[2]) == 0 && clients[i].connfd != connfd || strcmp(clients[i].username, info_array[2]) != 0 && clients[i].connfd == connfd)
        {
          strcpy(message, "[server]: Account already logged in on another device!\n");
          is_logged_in = 1;
          break;
        }
        else if (strcmp(clients[i].username, info_array[2]) == 0 && clients[i].connfd == connfd)
        {
          strcpy(message, "[server]: You are already logged in!\n");
          is_logged_in = 1;
          break;
        }
      }

      if (!is_logged_in)
      {
        ok = check_credentials(info_array[2], info_array[3]);
        if (!ok)
        {
          strcpy(message, "[server]: Incorrect username or password!\n");
        }
        else
        {
          strcpy(message, "[server]: Login successfully!\n");
          clients[client_index].connfd = connfd;
          strcpy(clients[client_index++].username, info_array[2]);
          bytes_sent = send(connfd, message, BUFF_SIZE, 0);
          if (bytes_sent < 0)
            perror("\n[server]: ");
          printf("%s\n\n", message);

          // upload existing files
          bytes_received = recv(connfd, buff, BUFF_SIZE, 0);
          if (bytes_received < 0)
            perror("\n[server]: ");

          buff[bytes_received] = '\0';

          int file_count;
          char **files = extract_information(buff, &file_count);
          for (int i = 0; i < file_count; i++)
          {
            bool ok = upload_file(files[i], info_array[2]);
            if (!ok)
            {
              printf("[server]: File upload failed!\n");
            }
          }
          printf("[server]: Files upload successfully!\n");
          break;
        }
      }

      bytes_sent = send(connfd, message, BUFF_SIZE, 0);
      if (bytes_sent < 0)
        perror("\n[server]: ");
      printf("%s\n\n", message);
      break;

    case 3:
      // int check;

      for (int i = 0; i < client_index; i++)
      {
        if (strcmp(clients[i].username, info_array[2]) == 0 && clients[i].connfd == connfd)
        {
          check = 1;

          // Create a file update struct to pass to the file_update_handler thread
          strcpy(clients[i].filename, info_array[3]);
          strcpy(clients[i].method, info_array[1]);
          if (count == 5)
            strcpy(clients[i].old_filename, info_array[4]);

          // Create a new thread to handle the file update
          pthread_t file_update_tid;
          pthread_create(&file_update_tid, NULL, file_upload_handler, &clients[i]);
          break;
        }
      }
      if (check == 0)
      {
        strcpy(message, "[server]: Have to login first!\n");
        bytes_sent = send(connfd, message, BUFF_SIZE, 0);
        if (bytes_sent < 0)
          perror("\n[server]: ");
        printf("%s\n\n", message);
      }
      break;

    case 4:

      for (int i = 0; i < client_index; i++)
      {
        if (strcmp(clients[i].username, info_array[2]) == 0 && clients[i].connfd == connfd)
        {
          check = 1;

          delete_client(clients, client_index, i);
          client_index--;
          strcpy(message, "[server]: Logout successfully!\n");
          break;
        }
      }
      if (check == 0)
      {
        strcpy(message, "[server]: Have to login first!\n");
      }

      bytes_sent = send(connfd, message, BUFF_SIZE, 0);
      if (bytes_sent < 0)
        perror("\n[server]: ");
      printf("%s\n\n", message);
      break;

    case 5:
      strcpy(reply_search, "reply search from server\n");
      bytes_sent = send(connfd, reply_search, sizeof(reply_search), 0);
      if (bytes_sent < 0)
        perror("\n[server]: ");
      printf("%s\n\n", reply_search);
      break;
    }
  }
  close(connfd);
  // return NULL;
  return NULL;
}

// Function to handle file updates
void *file_upload_handler(void *arg)
{
  Client *client = (Client *)arg;
  int connfd = client->connfd;
  char username[50];
  char filename[BUFF_SIZE];
  char method[BUFF_SIZE];
  strcpy(filename, client->filename);
  strcpy(username, client->username);
  strcpy(method, client->method);
  char message[BUFF_SIZE];
  int bytes_sent;

  // Process the file update (send to the database, perform any necessary operations, etc.)
  printf("[server]: Received file from client %s: %s\n", username, filename);

  if (strcmp(method, "upload") == 0)
  {
    bool ok = upload_file(filename, username);
    if (!ok)
      strcpy(message, "[server]: File upload failed!\n");
    else
      strcpy(message, "[server]: File upload successfully!\n");
  }
  else if (strcmp(method, "delete") == 0)
  {
    bool ok = delete_file(filename, username);
    if (!ok)
      strcpy(message, "[server]: File deletion failed!\n");
    else
      strcpy(message, "[server]: File deletion successfully!\n");
  }
  else if (strcmp(method, "modify") == 0)
  {
    bool ok = update_file_name(client->old_filename, filename, username);
    if (!ok)
      strcpy(message, "[server]: File name modification failed!\n");
    else
      strcpy(message, "[server]: File name modification successfully!\n");
  }

  bytes_sent = send(connfd, message, BUFF_SIZE, 0);
  if (bytes_sent < 0)
    perror("\n[server]: ");
  printf("%s\n\n", message);

  return NULL;
}

void cleanup_and_close_database()
{
  // Drop the database tables
  drop_all_postgresql_tables();

  // Cleanup and close the database connection
  close_postgresql_connection();
}

char **extract_information(const char *str, int *count)
{
  char *copy = (char *)malloc(strlen(str) + 1);
  if (copy != NULL)
    strcpy(copy, str);

  char *token = strtok(copy, ":"); // Get the first token
  *count = 0;

  // Count the number of tokens
  while (token != NULL)
  {
    (*count)++;
    token = strtok(NULL, ":"); // Get the next token
  }

  // Allocate memory for the array of tokens
  char **info_array = (char **)malloc((*count) * sizeof(char *));

  // Reset the copy string and tokenize again to populate the array
  strcpy(copy, str);
  token = strtok(copy, ":"); // Get the first token
  int i = 0;

  while (token != NULL)
  {
    // Allocate memory for the current token
    info_array[i] = (char *)malloc(strlen(token) + 1);
    if (info_array[i] != NULL)
    {
      strcpy(info_array[i], token);
      token = strtok(NULL, ":"); // Get the next token
      i++;
    }
  }

  free(copy); // Free the copied string

  return info_array;
}
