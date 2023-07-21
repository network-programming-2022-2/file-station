#include<gtk/gtk.h>
#include "../main.h"

bool handle_registration(const char* delimiter, const char* password, InotifyThreadArgs inotify_args)
{
  char buff[BUFF_SIZE];
  const char *register_array[6] = { "1", "register", inotify_args.username, password, inotify_args.server_port, inotify_args.ip };
  int size = sizeof(register_array) / sizeof(register_array[0]);

  char* constructed_str = construct_string(register_array, size, delimiter);

  printf("%s %d\n", constructed_str, inotify_args.client_sock);
  int bytes_sent = send(inotify_args.client_sock, constructed_str, strlen(constructed_str), 0);
  if (bytes_sent < 0) {
    printf("\nError! Cannot send data to server! Client exits immediately!\n");
    exit(1);
  }

  int bytes_received = recv(inotify_args.client_sock, buff, BUFF_SIZE, 0);
  if (bytes_received < 0) {
    printf("\nError! Cannot receive data from server! Client exits immediately!\n");
    exit(1);
  }
  buff[bytes_received] = '\0';
  if (strcmp(buff, "[server]: Registered successfully!\n") == 0)
  {
    return true;
  }
  return false;
}

GtkWidget *window;
GtkWidget *fixed;
GtkWidget *registerLabel;
GtkWidget *usernameLabel;
GtkWidget *passwordLabel;
GtkWidget *usernameEntry;
GtkWidget *passwordEntry;
GtkWidget *submitButton;
GtkWidget *backButton;
GtkBuilder *builder; 
void on_submitButton_clicked(GtkButton *b);
InotifyThreadArgs register_inotify_args;

GtkWidget* createRegisterView(InotifyThreadArgs inotify_args){
        strcpy(register_inotify_args.path_to_watch, inotify_args.path_to_watch);
        register_inotify_args.client_sock = inotify_args.client_sock;
        register_inotify_args.port = inotify_args.port;
        strcpy(register_inotify_args.server_port, inotify_args.server_port);
        register_inotify_args.inotify_tid = inotify_args.inotify_tid;
        strcpy(register_inotify_args.ip, inotify_args.ip);

        builder = gtk_builder_new_from_file("xml/register.glade");
        window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        fixed = GTK_WIDGET(gtk_builder_get_object(builder, "fixed"));
        registerLabel = GTK_WIDGET(gtk_builder_get_object(builder, "registerLabel"));
        usernameLabel = GTK_WIDGET(gtk_builder_get_object(builder, "usernameLabel"));
        passwordLabel = GTK_WIDGET(gtk_builder_get_object(builder, "passwordLabel"));
        usernameEntry = GTK_WIDGET(gtk_builder_get_object(builder, "usernameEntry"));
        passwordEntry = GTK_WIDGET(gtk_builder_get_object(builder, "passwordEntry"));
        submitButton = GTK_WIDGET(gtk_builder_get_object(builder, "submitButton"));
        backButton = GTK_WIDGET(gtk_builder_get_object(builder, "backButton"));
        g_signal_connect(submitButton, "clicked", G_CALLBACK(on_submitButton_clicked), NULL);

        return fixed;
}

void on_submitButton_clicked(GtkButton *b){
        const gchar *username = gtk_entry_get_text(GTK_ENTRY(usernameEntry));
        const gchar *password = gtk_entry_get_text(GTK_ENTRY(passwordEntry));
        strcpy(register_inotify_args.username, username);
        bool success = handle_registration(":", password, register_inotify_args);
        if (success)
        {
          printf("[server]: Registered successfully!\n");
        }
        else
        {
          printf("[server]: Username already exists!\n");
        }
        fflush(stdout); 
}
