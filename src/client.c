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
    printf("Enter your choice (1 or 2 or 3, other to quit): ");
}


char* construct_string(const char** info_array, int size, const char* delimiter);

void *inotify_thread(void *arg);

int handle_download(char ip[INET_ADDRSTRLEN]);
void *receive_thread(void *server_fd);
char* receiving(int server_fd);
void sending(char ip[INET_ADDRSTRLEN], int sender_port);
// int file_list(char path[SIZE]);
int search_for_file(char path[SIZE], char file_name[SIZE]);
void handle_req_string(char bufff[SIZE], int* des_port, char filename[SIZE]);
void send_file(char ip[SIZE], int des_port, char filename[SIZE]);
void receive_file(int src_sockfd, char filename[]);
long get_file_size(FILE* file);
void handle_receive_file_size(char buffer[SIZE], int* file_size);
int file_list(char path[SIZE], char * list_of_file[]);

void* inotify_thread(void* arg); 
char* get_input();
bool handle_registration(int client_sock, const char* delimiter, char username[], char password[], char server_port[], char ip[]);
bool handle_login(int client_sock, const char* delimiter, char* username, char* password, pthread_t inotify_tid, InotifyThreadArgs inotify_args);

char path_to_be_watched[SIZE];
char file_name_inserted[SIZE];
char server_port[SIZE];

bool handle_login(int client_sock, const char* delimiter, char* username, char* password, pthread_t inotify_tid, InotifyThreadArgs inotify_args) 
{
  const char *login_array[4] = { "2", "login", username, password };
  int size = sizeof(login_array) / sizeof(login_array[0]);
  char* constructed_string = construct_string(login_array, size, delimiter);
  char buff[BUFF_SIZE];

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
  printf("%s\n", buff);

  if (strcmp(buff, "[server]: Login successfully!\n") == 0) 
  {
    // upload existing files
    char* list_of_files[SIZE];

    // Allocate memory for each file name
    for (int i = 0; i < SIZE; i++) {
      list_of_files[i] = (char*)malloc(SIZE);
    }

    int file_count = file_list(path_to_be_watched, list_of_files);
    char* message = construct_string((const char**)list_of_files, file_count, delimiter);
    int bytes_sent = send(client_sock, message, strlen(message), 0);
    if (bytes_sent < 0) {
      printf("\nError! Can not send data to server! Client exit immediately!\n");
    }
                  
    strcpy(inotify_args.username, username);
    pthread_create(&inotify_tid, NULL, inotify_thread, &inotify_args);
    sleep(2); // Wait for 2 seconds
    return true;
  }
  return false;
}

bool handle_registration(int client_sock, const char* delimiter, char username[], char password[], char server_port[], char ip[])
{
  char buff[BUFF_SIZE];
  const char *register_array[6] = { "1", "register", username, password, server_port, ip };
  int size = sizeof(register_array) / sizeof(register_array[0]);

  char* constructed_str = construct_string(register_array, size, delimiter);

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
  printf("%s\n", buff);
  return true;
}

char* get_input() {
    char* input = (char*)malloc(SIZE);
    scanf("%s", input);
    while (getchar() != '\n'); // Clear input buffer
    return input;
}

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

    int is_moved = 0;
    char original_file[BUF_LEN];

    fd = inotify_init();
    if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
        printf("Error setting non-blocking flag\n");
        exit(2);
    }

    wd = inotify_add_watch(fd, path_to_be_watched, IN_MOVED_TO | IN_MOVED_FROM | IN_CREATE | IN_DELETE);
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
                        const char* info_array[] = { "3", "upload", username, event->name };

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

                        // Send the file name to the server
                        const char* del_info_array[] = { "3", "delete", username, event->name };

                        int size = sizeof(del_info_array) / sizeof(del_info_array[0]);
                        char* message = construct_string(del_info_array, size, delimiter);

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
                else if (event->mask & IN_MOVED_FROM) {
                    if (event->mask & IN_ISDIR) {
                        printf("\nThe directory %s was moved.\n", event->name);
                    }
                    else {
                        printf("\nThe file %s was moved.\n", event->name);

                        strcpy(original_file, event->name);
                    }
                }
                else if (event->mask & IN_MOVED_TO) {
                    if (event->mask & IN_ISDIR) {
                        printf("\nThe directory was modified into %s.\n\n", event->name);
                    }
                    else {
                        printf("\nThe file was modified into %s.\n\n", event->name);
                        // Send the file name to the server
                        const char* mod_info_array[] = { "3", "modify", username, event->name, original_file };

                        int size = sizeof(mod_info_array) / sizeof(mod_info_array[0]);
                        char* message = construct_string(mod_info_array, size, delimiter);

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
    char ip[INET_ADDRSTRLEN];
    int msg_len, bytes_sent, bytes_received, bytes_read, total_bytes_sent, total_bytes_received;
    int port;
    const char* delimiter = ":";
    char server_port[SIZE];
    bool success;

    if (argc != 5) {
        printf("[ERROR]: The client needs to be bound to an IP address, a client port, a peer server port, and a folder to trace changes.\n");
        exit(EXIT_FAILURE);
    }
    else {
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

    while (1) {
        menu();
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear input buffer
        printf("Username: ");
        scanf("%s", username);

        printf("Password: ");
        scanf("%s", password);

        const char *logout_array[3] = { "4", "logout", username };
        int size;
        //
        switch (choice) {
            case 1:
                success = handle_registration(client_sock, delimiter, username, password, server_port, ip);
                break;

            case 2:
                // login_array[4] = login_array1;
                success = handle_login(client_sock, delimiter, username, password, inotify_tid, inotify_args);
               
                if (success == true)
        {
                int server_fd;
                while (1) {
                    int sub_choice;
                    submenu();        

                    printf("\nafter login: your choice ...\n");
                    scanf("%d", &sub_choice);
                    getchar();

                    if (sub_choice == 1) {
                        printf("chua xong ...\n");
                        int f_num;
                        char *list_of_file[100];
                        f_num = file_list(path_to_be_watched, list_of_file);
                        for (int i = 0; i < f_num ; i ++)
                        {
                          printf("\n%d: %s", i, list_of_file[i]);
                        }
                        break;
                    }
                    else if (sub_choice == 2)
                    {
                        printf("download ...\n");
                        server_fd = handle_download(ip);
                        break;
                    }
                    else
                    {
                        //logout
                        printf("loging out...\n");
                        size = sizeof(logout_array) / sizeof(logout_array[0]);
                        char* constructed_string = construct_string(logout_array, size, delimiter);

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
                    }
                    close(server_fd);
                }
                }

                break;
        }
    }

    close(client_sock);
    return 0;
}

int file_list(char path[SIZE], char* list_of_files[])
{
  DIR* dir = opendir(path);
  struct dirent* entry;
  struct stat filestat;

  int file_count = 0;
  if (dir == NULL)
  {
    printf("Error opening directory: %s\n", path);
    exit(0);
  }

  while ((entry = readdir(dir)) != NULL)
  {
    stat(entry->d_name,&filestat);
    if( S_ISDIR(filestat.st_mode) )
    {
      printf("%4s: %s\n","Dir",entry->d_name);
      continue;
    }

    printf("%4s: %s\n","File",entry->d_name);
    list_of_files[file_count] = malloc(strlen(entry->d_name) + 1);
    strcpy(list_of_files[file_count++], entry->d_name);
  }
  closedir(dir);
  return file_count;
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

int handle_download(char ip[]){
    int port = atoi(server_port);
    // printf("Enter your port number:");
    // scanf("%d", &port);
    
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(port);

    //Printed the server socket addr and port
    printf("IP address is: %s\n", inet_ntoa(address.sin_addr));
    printf("port is: %d\n", (int)ntohs(address.sin_port));

    // bind address and listen
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    //Creating thread to keep listening request from other clients
    int ch;
    pthread_t tid;
    pthread_create(&tid, NULL, &receive_thread, &server_fd); 
    printf("\n**********\n");
    sending(ip, port);
    return server_fd;
}

//Calling receiving every 2 seconds
void *receive_thread(void *server_fd)
{
    int s_fd = *((int *)server_fd);
    while (1)
    {
        sleep(2);
        receiving(s_fd);
    }
}

//Receiving messages on our port
char* receiving(int server_fd)
{
    struct sockaddr_in address;
    int bytes_read;
    char buffer[SIZE] = {0};
    int addrlen = sizeof(address);
    fd_set current_sockets, ready_sockets;
    char ip[SIZE];

    int file_size;
    int des_port;
    char filename[SIZE];
    // char file_req[SIZE];

    //Initialize my current set
    FD_ZERO(&current_sockets);
    FD_SET(server_fd, &current_sockets);
    int k = 0;
    while (1)
    {
        k++;
        ready_sockets = current_sockets;

        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
        {
            perror("Error");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < FD_SETSIZE; i++)
        {
            int client_socket;

            if (FD_ISSET(i, &ready_sockets))
            {

                if (i == server_fd)
                {

                    if ((client_socket = accept(server_fd, (struct sockaddr *)&address,
                                                (socklen_t *)&addrlen)) < 0)
                    {
                        perror("accept");
                        exit(EXIT_FAILURE);
                    }
                    FD_SET(client_socket, &current_sockets);
                }
                else
                {
                    bytes_read = recv(i, buffer, sizeof(buffer), 0);

                    printf("%s", buffer);
                    char *sub = strstr(buffer, "[PORT]:");
                    char *sub_size = strstr(buffer, "[FILE SIZE]:");
                    if (sub != NULL)
                    {
                        // receiving file name and port number
                        handle_req_string(buffer, &des_port, filename);

                        // printf("after parsing: %d %s\n", des_port, filename);
                        int is_existed = search_for_file(path_to_be_watched, filename);
                        if(is_existed == 1){
                            printf("file existed");
                        }else{
                            printf("file not found");
                        }

                        send_file(ip, des_port, filename);
                    }
                    else if(sub_size != NULL){
                        // receiving file size
                        handle_receive_file_size(buffer, &file_size);
                        printf("\nfile size: %d\n", file_size);

                        receive_file(client_socket, filename);
                    }else{
                        printf("%s", buffer);
                    }
                    // printf("\n%s\n", buffer);
                    FD_CLR(i, &current_sockets);
                }
            }
        }

        if (k == (FD_SETSIZE * 2))
            break;
    }
}

//Sending messages to port
void sending(char ip[], int sender_port)
{

    char buffer[SIZE] = {0};
    //Fetching port number
    int PORT_server;

    printf("Enter the port to send message:"); 
    scanf("%d", &PORT_server);

    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char filename[SIZE/2] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(PORT_server);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return;
    }

    char dummy;
    printf("Enter your file you want to download:");
    scanf("%c", &dummy);
    scanf("%[^\n]s", filename);
    strcpy(file_name_inserted, filename);
    sprintf(buffer, "[PORT]:%d:%s", sender_port, filename);
    send(sock, buffer, sizeof(buffer), 0);
    printf("\nMessage sent\n");

    close(sock);
}

int search_for_file(char path[], char file_name[]){
    struct dirent *entry;
    DIR *dir = opendir(path);

    // printf("compare file: %s\n", file_name);
    if (dir == NULL)
    {
        printf("Unable to open the directory.\n");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        // if (entry->d_type == DT_REG) {  // Only process regular files, excluding directories
        // printf("%s\n", entry->d_name);
        // }
        if(strcmp(entry->d_name, file_name) == 0){
            printf("found sth\n");
            return 1;
        }
    }

    return -1;
}

void handle_req_string(char buffer[SIZE], int* des_port, char filename[SIZE]){
    char *token;
    int index = 0;
    // buffer format PORT:%d:%s 
    // %d port number is the destination to send file to
    // %s file name that destination requested
    token = strtok(buffer, ":");
    while(token != NULL){
        // printf("%s-%ld\n", token, strlen(token));
        if(index == 1){
            *des_port = atoi(token);
        }else if(index == 2){
            strcpy(filename, token);
        }

        index++;
        
        token = strtok(NULL, ":");
    }
}

void handle_receive_file_size(char buffer[SIZE], int* file_size){
    char *token;
    int index = 0;
    // buffer format [FILE SIZE]:%ld 
    // %ld file size
    token = strtok(buffer, ":");
    while(token != NULL){
        // printf("%s-%ld\n", token, strlen(token));
        if(index == 1){
            *file_size = atoi(token);
        }
        index++;
        token = strtok(NULL, ":");
    }
}

void send_file(char ip[], int des_port, char filename[SIZE]){
    char path_to_file[SIZE * 2];
    int bytes_read = 0;
    char buffer[SIZE];
    struct sockaddr_in des_addr;
    int des_sock;
    strcpy(ip, "127.0.0.1"); // for testing

    if ((des_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return;
    }
    des_addr.sin_family = AF_INET;
    des_addr.sin_addr.s_addr = inet_addr(ip);
    des_addr.sin_port = htons(des_port);
    
    if (connect(des_sock, (struct sockaddr *)&des_addr, sizeof(des_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return;
    }

    strcpy(path_to_file, path_to_be_watched);
    strcat(path_to_file, "/");
    strcat(path_to_file, filename);

    // printf("full_path:%s\n", path_to_file);
    
    FILE *src_file = fopen(path_to_file, "rb");
    if(src_file == NULL){
        printf("Error: could not open file.\n");
		return ;
    }

    long file_size = get_file_size(src_file);

    char file_size_msg[SIZE];
    sprintf(file_size_msg, "[FILE SIZE]:%ld", file_size);
    // Send file size to the other peer
    if (send(des_sock, file_size_msg, sizeof(file_size_msg), 0) < 0) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }

    while ((bytes_read = fread(buffer, sizeof(char), sizeof(buffer), src_file)) > 0)
    {
        printf("content: %s\n", buffer);
        int bytes_sent;
        if ((bytes_sent = send(des_sock, buffer, bytes_read, 0)) < 0)
        {
            if (bytes_sent < 0)
		    {
			    printf("\nConnection closed");
			    break;
		    }
        }
    }
    fclose(src_file);
    printf("done read file\n");
    return ;
}

void receive_file(int src_sockfd, char filename[]){
    char storage_path[SIZE] = "../storage/";

    file_name_inserted[strlen(file_name_inserted)] = '\0';

    strcat(storage_path, file_name_inserted);
    int bytes_write;
    char buffer[SIZE];

    printf("\npath to write file: %s\n", storage_path);
    FILE *write_to_this_path = fopen(storage_path, "w");
    if (write_to_this_path == NULL)
	{
		printf("Error: could not open file.\n");
		return ;
	}

    while((bytes_write = recv(src_sockfd, buffer, sizeof(buffer), 0)) > 0){
        if(fwrite(buffer, sizeof(char), bytes_write, write_to_this_path) < 0){
			printf("Cannot write to file\n");
            break;
        }
    }

    char finished_download[SIZE];
    strcpy(finished_download, "Download completed");

    int bytes_sent = send(src_sockfd, finished_download, sizeof(finished_download), 0);
    if(bytes_sent < 0){
        perror("connection close");
        exit(0);
    }

    fclose(write_to_this_path);
    close(src_sockfd);
}

long get_file_size(FILE* file){
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

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

    int is_moved = 0;
    char original_file[BUF_LEN];

    fd = inotify_init();
    if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
        printf("Error setting non-blocking flag\n");
        exit(2);
    }

    wd = inotify_add_watch(fd, path_to_be_watched, IN_MOVED_TO | IN_MOVED_FROM | IN_CREATE | IN_DELETE);
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
                        const char* info_array[] = { "3", "upload", username, event->name };

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

                        // Send the file name to the server
                        const char* del_info_array[] = { "3", "delete", username, event->name };

                        int size = sizeof(del_info_array) / sizeof(del_info_array[0]);
                        char* message = construct_string(del_info_array, size, delimiter);

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
                else if (event->mask & IN_MOVED_FROM) {
                    if (event->mask & IN_ISDIR) {
                        printf("\nThe directory %s was moved.\n", event->name);
                    }
                    else {
                        printf("\nThe file %s was moved.\n", event->name);

                        strcpy(original_file, event->name);
                    }
                }
                else if (event->mask & IN_MOVED_TO) {
                    if (event->mask & IN_ISDIR) {
                        printf("\nThe directory was modified into %s.\n\n", event->name);
                    }
                    else {
                        printf("\nThe file was modified into %s.\n\n", event->name);
                        // Send the file name to the server
                        const char* mod_info_array[] = { "3", "modify", username, event->name, original_file };

                        int size = sizeof(mod_info_array) / sizeof(mod_info_array[0]);
                        char* message = construct_string(mod_info_array, size, delimiter);

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
            }
            i += EVENT_SIZE + event->len;
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);

    pthread_exit(NULL);
}