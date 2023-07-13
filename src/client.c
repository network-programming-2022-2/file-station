#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFF_SIZE 8192
#define SIZE 1024

void menu() {
  printf("--------------------------------------------\n");
	printf("1. Register\n");
	printf("2. Login\n");
	printf("--------------------------------------------\n");
	printf("Enter your choice (1 or 2, other to quit): ");
}

char* construct_string(const char** info_array, int size, const char* delimiter); 

int main(int argc, char *argv[])
{
	int client_sock, choice;
	char buff[BUFF_SIZE];
  char username[SIZE];
  char password[SIZE];
	struct sockaddr_in server_addr; /* server's address information */
	int msg_len, bytes_sent, bytes_received, bytes_read, total_bytes_sent, total_bytes_received;
  int port;
  char ip[INET_ADDRSTRLEN];
  const char* delimiter = ":";

  if (argc != 3)
	{
		printf("[ERROR]: The client needs to be binded with an IP address, and a port.\n");
		exit(EXIT_FAILURE);
	} else {
    port = atoi(argv[2]);
    strcpy(ip, argv[1]);
  }

	//Step 1: Construct socket
	client_sock = socket(AF_INET,SOCK_STREAM,0);
	
	//Step 2: Specify server address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(ip);
	
	//Step 3: Request to connect server
	if(connect(client_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0)
  {
		printf("\nError!Can not connect to sever! Client exit imediately!\n");
		return 0;
	}
		
	//Step 4: Communicate with server			
	while(1)
  {
    menu();
    scanf("%d", &choice);
    while (getchar() != '\n'); // Clear input buff
    printf("Username: ");
    scanf("%s", username);

    printf("Password: ");
    scanf("%s", password);

    switch(choice){
      case 1:
      const char* info_array[] = {"1", "register", username, password};
      int size = sizeof(info_array) / sizeof(info_array[0]);
      char* constructed_string = construct_string(info_array, size, delimiter);

      bytes_sent = send(client_sock, constructed_string, strlen(constructed_string), 0);
      if(bytes_sent < 0)
      {
        printf("\nError!Can not send data to sever! Client exit imediately!\n");
        return 0;
      }

      bytes_received = recv(client_sock, buff, BUFF_SIZE, 0);
      if(bytes_received < 0)
      {
        printf("\nError!Can not receive data from sever! Client exit imediately!\n");
        return 0;
      }
      buff[bytes_received] = '\0';
      printf("%s\n", buff);
      break;

      case 2:
      const char* login_array[] = {"2", "login", username, password};
      size = sizeof(info_array) / sizeof(login_array[0]);
      constructed_string = construct_string(login_array, size, delimiter);

      bytes_sent = send(client_sock, constructed_string, strlen(constructed_string), 0);
      if(bytes_sent < 0)
      {
        printf("\nError!Can not send data to sever! Client exit imediately!\n");
        return 0;
      }

      bytes_received = recv(client_sock, buff, BUFF_SIZE, 0);
      if(bytes_received < 0)
      {
        printf("\nError!Can not receive data from sever! Client exit imediately!\n");
        return 0;
      }
      buff[bytes_received] = '\0';
      printf("%s\n", buff);
      break;

    }
	}
	
	//Step 4: Close socket
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
    constructed_string[0] = '\0'; // Initialize as empty string

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
