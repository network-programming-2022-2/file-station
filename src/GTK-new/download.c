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

char path_to_be_watched[SIZE];
char file_name_inserted[SIZE];
char server_port[SIZE];


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