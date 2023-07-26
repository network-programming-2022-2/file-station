#include "main.h"
#include "./handler/homepage.c"

char* construct_string(const char** info_array, int size, const char* delimiter) {
    int total_length = 0;

    // Calculate the total length of the constructed string
    for (int i = 0; i < size; i++) {
        total_length += strlen(info_array[i]);
    }

    // Add space for delimiters and null terminator
    total_length += (size - 1) * strlen(delimiter) + 1;

    // Allocate memory for the constructed string
    char* constructed_string = (char*)malloc(total_length * sizeof(char));
    constructed_string[0] = '\0'; // Initialize as an empty string

    // Construct the string by joining elements with the delimiter
    for (int i = 0; i < size; i++) {
        strcat(constructed_string, info_array[i]);

        // Append delimiter if it's not the last element
        if (i < size - 1) {
            strcat(constructed_string, delimiter);
        }
    }

    return constructed_string;
}

int main(int argc, char *argv[]) {
    int client_sock;
    struct sockaddr_in server_addr;
    char path_to_be_watched[SIZE];
    int port;
    char ip[INET_ADDRSTRLEN];
    char server_port[SIZE];

    if (argc != 5) {
        printf("[ERROR]: The client needs to be bound to an IP address, a client port, a peer server port, and a folder to trace changes.\n");
        exit(EXIT_FAILURE);
    } else {
        strcpy(ip, argv[1]);
        port = atoi(argv[2]);
        strcpy(server_port, argv[3]);
        strcpy(path_to_be_watched, argv[4]);
    }

    client_sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(client_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0) {
        printf("\nError! Cannot connect to server! Client exits immediately!\n");
        return 0;
    }

    pthread_t inotify_tid;
    InotifyThreadArgs inotify_args;
    strcpy(inotify_args.path_to_watch, path_to_be_watched);
    inotify_args.client_sock = client_sock;
    inotify_args.port = port;
    strcpy(inotify_args.server_port, server_port);
    inotify_args.inotify_tid = inotify_tid;
    strcpy(inotify_args.ip, ip);

    printf("path_to_watch: %s\n", inotify_args.path_to_watch);
    printf("client_sock: %d\n", inotify_args.client_sock);
    printf("inotify_tid: %ld\n", inotify_args.inotify_tid);
    printf("port: %d\n", inotify_args.port);
    printf("server_port: %s\n", inotify_args.server_port);
    printf("ip: %s\n", inotify_args.ip);    

    gtk_init(NULL, NULL);

    createHomePageView(inotify_args);
    gtk_widget_show(window);
    gtk_main();
    return 1;
}

char** extract_information(const char* str, int* count, const char* delimiter) 
{
  char* copy = (char*)malloc(strlen(str) + 1);
  if (copy != NULL) 
    strcpy(copy, str);

  char* token = strtok(copy, delimiter); // Get the first token
  *count = 0;

  // Count the number of tokens
  while (token != NULL) 
  {
    (*count)++;
    token = strtok(NULL, delimiter); // Get the next token
  }

  // Allocate memory for the array of tokens
  char** info_array = (char**)malloc((*count) * sizeof(char*));

  // Reset the copy string and tokenize again to populate the array
  strcpy(copy, str);
  token = strtok(copy, delimiter); // Get the first token
  int i = 0;

  while (token != NULL) 
  {
    // Allocate memory for the current token
    info_array[i] = (char*)malloc(strlen(token) + 1);
    if (info_array[i] != NULL) {
      strcpy(info_array[i], token);
      token = strtok(NULL, delimiter); // Get the next token
      i++;
    }
  }

  free(copy); // Free the copied string

  return info_array;
}


