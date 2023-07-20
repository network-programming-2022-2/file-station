#include<gtk/gtk.h>
#include "register.c"
#include "../main.h"

GtkWidget *window;
GtkWidget *fixed;
GtkWidget *registerButton;
GtkWidget *loginButton;
GtkWidget *appNameLabel;
GtkBuilder *builder; 
void on_registerButton_clicked(GtkButton *b);
InotifyThreadArgs home_inotify_args;

int createHomePageView(InotifyThreadArgs inotify_args){
        gtk_init(NULL, NULL);
  
        strcpy(home_inotify_args.path_to_watch, inotify_args.path_to_watch);
        home_inotify_args.client_sock = inotify_args.client_sock;
        home_inotify_args.port = inotify_args.port;
        strcpy(home_inotify_args.server_port, inotify_args.server_port);
        home_inotify_args.inotify_tid = inotify_args.inotify_tid;
        strcpy(home_inotify_args.ip, inotify_args.ip);

        builder = gtk_builder_new_from_file("xml/homepage.glade");
        window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        fixed = GTK_WIDGET(gtk_builder_get_object(builder, "fixed"));
        registerButton = GTK_WIDGET(gtk_builder_get_object(builder, "registerButton"));
        loginButton = GTK_WIDGET(gtk_builder_get_object(builder, "loginButton"));
        appNameLabel = GTK_WIDGET(gtk_builder_get_object(builder, "appNameLabel"));
        g_signal_connect(registerButton, "clicked", G_CALLBACK(on_registerButton_clicked), NULL);


        gtk_widget_show(window);
        gtk_main();
        return EXIT_SUCCESS;
}

void on_registerButton_clicked(GtkButton *b){
        GtkWidget *registerWindow = createRegisterView(home_inotify_args);
        gtk_widget_show(registerWindow);
}