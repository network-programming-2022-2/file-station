#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/inotify.h>
#include <fcntl.h>

#define BUFF_SIZE 8192
#define SIZE 1024

#define MAX_EVENTS 1024
#define LEN_NAME 16
#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (MAX_EVENTS * (EVENT_SIZE + LEN_NAME))

typedef struct InotifyThreadArgs {
  char path_to_watch[SIZE];
  int client_sock;
  char username[SIZE];
} InotifyThreadArgs;

void menu() {
    printf("--------------------------------------------\n");
    printf("1. Register\n");
    printf("2. Login\n");
    printf("--------------------------------------------\n");
    printf("Enter your choice (1 or 2, other to quit): ");
}

void submenu() {
    printf("--------------------------------------------\n");
    printf("1. Search\n");
    printf("2. Download\n");
    printf("3. Logout\n");
    printf("--------------------------------------------\n");
    printf("Enter your choice (1 or 2, other to quit): ");
}


char* construct_string(const char** info_array, int size, const char* delimiter);

void* inotify_thread(void* arg) {
    InotifyThreadArgs* args = (InotifyThreadArgs*)arg;
    char path_to_be_watched[SIZE];
    strcpy(path_to_be_watched, args->path_to_watch);
    int client_sock = args->client_sock;
    char username[SIZE];
    strcpy(username, args->username);
    int fd, wd;
    int length, i;
    char buffer[BUF_LEN];
    const char* delimiter = ":";

    fd = inotify_init();
    if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
        printf("Error setting non-blocking flag\n");
        exit(2);
    }

    wd = inotify_add_watch(fd, path_to_be_watched, IN_MODIFY | IN_CREATE | IN_DELETE);
    if (wd == -1) {
        printf("Could not watch : %s\n", path_to_be_watched);
        pthread_exit(NULL);
    }
    else {
        printf("Watching : %s\n", path_to_be_watched);
    }

    while (1) {
        length = read(fd, buffer, BUF_LEN);
        if (length < 0) {
            continue;
        }

        i = 0;
        while (i < length) {
            struct inotify_event* event = (struct inotify_event*)&buffer[i];

            if (event->len) {
                if (event->mask & IN_CREATE) {
                    if (event->mask & IN_ISDIR) {
                        printf("\nThe directory %s was created.\n", event->name);
                    }
                    else {
                        printf("\nThe file %s was created.\n", event->name);

                        // Send the file name to the server
                        const char* info_array[] = { "99", "upload", username, event->name };

                        int size = sizeof(info_array) / sizeof(info_array[0]);
                        char* message = construct_string(info_array, size, delimiter);

                        int bytes_sent = send(client_sock, message, strlen(message), 0);
                        if (bytes_sent < 0) {
                          printf("\nError! Can not send data to server! Client exit immediately!\n");
                        }
                        printf("%s\n", message);

                        int bytes_received = recv(client_sock, buffer, BUFF_SIZE, 0);
                        if (bytes_received < 0) {
                          printf("\nError! Can not receive data from server! Client exit immediately!\n"); 
                        }
                        printf("%s\n", buffer);
                    }
                }
                else if (event->mask & IN_DELETE) {
                    if (event->mask & IN_ISDIR) {
                        printf("\nThe directory %s was deleted.\n", event->name);
                    }
                    else {
                        printf("\nThe file %s was deleted.\n", event->name);
                    }
                }
                else if (event->mask & IN_MODIFY) {
                    if (event->mask & IN_ISDIR) {
                        printf("\nThe directory %s was modified.\n", event->name);
                    }
                    else {
                        printf("\nThe file %s was modified.\n", event->name);
                    }
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    int client_sock, choice;
    char buff[BUFF_SIZE];
    char username[SIZE];
    char password[SIZE];
    struct sockaddr_in server_addr;
    int msg_len, bytes_sent, bytes_received, bytes_read, total_bytes_sent, total_bytes_received;
    int port;
    char ip[INET_ADDRSTRLEN];
    const char* delimiter = ":";

    if (argc != 3) {
        printf("[ERROR]: The client needs to be bound to an IP address and a port.\n");
        exit(EXIT_FAILURE);
    }
    else {
        port = atoi(argv[2]);
        strcpy(ip, argv[1]);
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
    char path_to_be_watched[SIZE];
    strcpy(path_to_be_watched, "/home/nvt/2022-2/file-station");
    InotifyThreadArgs inotify_args;
    strcpy(inotify_args.path_to_watch, path_to_be_watched);
    inotify_args.client_sock = client_sock;

    while (1) {
        menu();
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear input buffer
        printf("Username: ");
        scanf("%s", username);

        printf("Password: ");
        scanf("%s", password);

        switch (choice) {
            case 1:
                const char* info_array[] = { "1", "register", username, password };
                int size = sizeof(info_array) / sizeof(info_array[0]);
                char* constructed_string = construct_string(info_array, size, delimiter);

                bytes_sent = send(client_sock, constructed_string, strlen(constructed_string), 0);
                if (bytes_sent < 0) {
                    printf("\nError! Cannot send data to server! Client exits immediately!\n");
                    return 0;
                }

                bytes_received = recv(client_sock, buff, BUFF_SIZE, 0);
                if (bytes_received < 0) {
                    printf("\nError! Cannot receive data from server! Client exits immediately!\n");
                    return 0;
                }
                buff[bytes_received] = '\0';
                printf("%s\n", buff);
                break;

            case 2:
                const char* login_array[] = { "2", "login", username, password };
                size = sizeof(info_array) / sizeof(login_array[0]);
                constructed_string = construct_string(login_array, size, delimiter);

                bytes_sent = send(client_sock, constructed_string, strlen(constructed_string), 0);
                if (bytes_sent < 0) {
                    printf("\nError! Cannot send data to server! Client exits immediately!\n");
                    return 0;
                }

                bytes_received = recv(client_sock, buff, BUFF_SIZE, 0);
                if (bytes_received < 0) {
                    printf("\nError! Cannot receive data from server! Client exits immediately!\n");
                    return 0;
                }
                buff[bytes_received] = '\0';
                printf("%s\n", buff);

                if (strcmp(buff, "[server]: Login successfully!\n") == 0) 
                {
                  strcpy(inotify_args.username, username);
                  pthread_create(&inotify_tid, NULL, inotify_thread, &inotify_args);
                  sleep(2); // Wait for 2 seconds

                  while (1) {
                    submenu();
                    int sub_choice;
                    scanf("%d", &sub_choice);
                    while (getchar() != '\n'); // Clear input buffer

                    if (sub_choice == 1) {
                      break;
                    }
                  }
                  
                }

                break;
        }
    }

    close(client_sock);
    return 0;
}

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
