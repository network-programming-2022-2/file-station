#ifndef HOMEPAGE_HANDLER_H
#define HOMEPAGE_HANDLER_H

#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/inotify.h>
#include <fcntl.h>
#include <sys/select.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>

#define BUFF_SIZE 8192
#define SIZE 1024

#define MAX_EVENTS 1024
#define LEN_NAME 16
#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (MAX_EVENTS * (EVENT_SIZE + LEN_NAME))

typedef struct {
  int file_id;
  char filename[100];
  int downloaded_numbers;
  char username[100];
  char ip[100];
  int port;
} SearchResult;

typedef struct InotifyThreadArgs {
  char path_to_watch[SIZE];
  int client_sock;
  char username[SIZE];
  pthread_t inotify_tid;
  int port;
  char server_port[SIZE];
  char ip[SIZE];
  int peer_server_fd;
} InotifyThreadArgs;

char* construct_string(const char** info_array, int size, const char* delimiter);
char** extract_information(const char* str, int* count, const char* delimiter);

#endif /* HOMEPAGE_HANDLER_H */
