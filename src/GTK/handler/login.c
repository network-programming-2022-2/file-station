#include<gtk/gtk.h>
#include"menu.c"
#include "../main.h"
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
        GtkWidget *menuFixed = createMenuView(0,NULL);
        gtk_container_remove(GTK_CONTAINER(window), GTK_WIDGET(fixed));
        gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(menuFixed));
        gtk_widget_show_all(window);

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


