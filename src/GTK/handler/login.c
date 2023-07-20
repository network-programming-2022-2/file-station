#include<gtk/gtk.h>
#include"menu.c"
#include "../main.h"

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

bool handle_login(const char* delimiter, const char* password, InotifyThreadArgs inotify_args) 
{
  const char *login_array[4] = { "2", "login", inotify_args.username, password };
  int size = sizeof(login_array) / sizeof(login_array[0]);
  char* constructed_string = construct_string(login_array, size, delimiter);
  char buff[BUFF_SIZE];

  int bytes_sent = send(inotify_args.client_sock, constructed_string, strlen(constructed_string), 0);
  if (bytes_sent < 0) {
    printf("\nError! Cannot send data to server! Client exits immediately!\n");
    return false;
  }

  int bytes_received = recv(inotify_args.client_sock, buff, BUFF_SIZE, 0);
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

    int file_count = file_list(inotify_args.path_to_watch, list_of_files);
    char* message = construct_string((const char**)list_of_files, file_count, delimiter);
    int bytes_sent = send(inotify_args.client_sock, message, strlen(message), 0);
    if (bytes_sent < 0) {
      printf("\nError! Can not send data to server! Client exit immediately!\n");
    }
                  
    pthread_create(&inotify_args.inotify_tid, NULL, inotify_thread, &inotify_args);
    sleep(2); // Wait for 2 seconds
    return true;
  }
  return false;
}

GtkWidget *window;
GtkWidget *fixed;
GtkWidget *loginLabel;
GtkWidget *usernameLabel;
GtkWidget *passwordLabel;
GtkWidget *usernameEntry;
GtkWidget *passwordEntry;
GtkWidget *submitButton;
GtkWidget *backButton;
GtkBuilder *builder; 
InotifyThreadArgs login_inotify_args;
void on_submitButton2_clicked(GtkButton *b){
        gchar *username = gtk_entry_get_text(GTK_ENTRY(usernameEntry));
        gchar *password = gtk_entry_get_text(GTK_ENTRY(passwordEntry));
 
        printf("%s %s\n", username, password);
        strcpy(login_inotify_args.username, username);
        bool success = handle_login(":", password, login_inotify_args);
        fflush(stdout);

        if (success) {
            printf("Login successful!\n");

            GtkWidget *menuFixed = createMenuView(0,NULL);
            gtk_container_remove(GTK_CONTAINER(window), GTK_WIDGET(fixed));
            gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(menuFixed));
            gtk_widget_show_all(window);
        }
        else {
            printf("Login failed!\n");
        }
}
GtkWidget* createLoginView(InotifyThreadArgs inotify_args){
        strcpy(login_inotify_args.path_to_watch, inotify_args.path_to_watch);
        login_inotify_args.client_sock = inotify_args.client_sock;
        login_inotify_args.port = inotify_args.port;
        strcpy(login_inotify_args.server_port, inotify_args.server_port);
        login_inotify_args.inotify_tid = inotify_args.inotify_tid;
        strcpy(login_inotify_args.ip, inotify_args.ip);


        builder = gtk_builder_new_from_file("xml/login.glade");
        window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        fixed = GTK_WIDGET(gtk_builder_get_object(builder, "fixed"));
        loginLabel = GTK_WIDGET(gtk_builder_get_object(builder, "loginLabel"));
        usernameLabel = GTK_WIDGET(gtk_builder_get_object(builder, "usernameLabel"));
        passwordLabel = GTK_WIDGET(gtk_builder_get_object(builder, "passwordLabel"));
        usernameEntry = GTK_WIDGET(gtk_builder_get_object(builder, "usernameEntry"));
        passwordEntry = GTK_WIDGET(gtk_builder_get_object(builder, "passwordEntry"));
        submitButton = GTK_WIDGET(gtk_builder_get_object(builder, "submitButton"));
        backButton = GTK_WIDGET(gtk_builder_get_object(builder, "backButton"));
        g_signal_connect(submitButton, "clicked", G_CALLBACK(on_submitButton2_clicked), NULL);

        return fixed;
}


