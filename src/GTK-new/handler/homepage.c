#include<gtk/gtk.h>
#include "../main.h"
#include "register.c"
#include "login.c"
#include "search_func.c"

InotifyThreadArgs home_inotify_args;

GtkWidget *window;
GtkWidget *fixed;
GtkWidget *registerButton;
GtkWidget *loginButton;
GtkWidget *appNameLabel;
GtkBuilder *menuBuilder; 
//Register
GtkWidget *registerWindow;
GtkWidget *registerFixed;
GtkWidget *registerLabel;
GtkWidget *usernameRegisterLabel;
GtkWidget *passwordRegisterLabel;
GtkWidget *usernameRegisterEntry;
GtkWidget *passwordRegisterEntry;
GtkWidget *submitRegisterButton;
GtkWidget *backRegisterButton;
GtkBuilder *builderRegister; 
//Login
GtkWidget *loginWindow;
GtkWidget *loginFixed;
GtkWidget *loginLabel;
GtkWidget *usernameLoginLabel;
GtkWidget *passwordLoginLabel;
GtkWidget *usernameLoginEntry;
GtkWidget *passwordLoginEntry;
GtkWidget *submitLoginButton;
GtkWidget *backLoginButton;
GtkBuilder *builderLogin; 
//Menu
GtkWidget *menuWindow;
GtkWidget *menuFixed;
GtkWidget *menuLabel;
GtkWidget *logoutMenuButton;
GtkWidget *searchMenuButton;
GtkWidget *downloadMenuButton;
GtkBuilder *menuBuilder; 
//Search
GtkWidget *searchWindow;
GtkWidget *searchFixed;
GtkWidget *searchEntry;
GtkWidget *searchButton;
GtkWidget *treeView;
GtkBuilder *searchBuilder; 
void on_registerButton_clicked(GtkButton *b){
        gtk_widget_hide(window);
        gtk_widget_show(registerWindow);
}
void on_loginButton_clicked(GtkButton *b,int argc, char *argv[]){
        gtk_widget_hide(window);
        gtk_widget_show(loginWindow);
       
}
/// REGISTER

void on_submitRegisterButton_clicked(GtkButton *b){
        const gchar *username = gtk_entry_get_text(GTK_ENTRY(usernameRegisterEntry));
        const gchar *password = gtk_entry_get_text(GTK_ENTRY(passwordRegisterEntry));

        strcpy(home_inotify_args.username, username);
        bool ok = handle_registration(":", password, home_inotify_args);
        fflush(stdout);

        if (ok){
          gtk_widget_hide(registerWindow);
          gtk_widget_show(window);
        }
        else{
          printf("Registration failed!\n"); 
        }
}
void on_backRegisterButton_clicked(GtkButton *b,int argc, char *argv[]){

        gtk_widget_hide(registerWindow);
        gtk_widget_show(window);
       
}
//Login
void on_submitLoginButton_clicked(GtkButton *b){
        const gchar *username = gtk_entry_get_text(GTK_ENTRY(usernameLoginEntry));
        const gchar *password = gtk_entry_get_text(GTK_ENTRY(passwordLoginEntry));
        strcpy(home_inotify_args.username, username);
        bool ok = handle_login(":", password, home_inotify_args);
        fflush(stdout);
        if (ok){
          gtk_widget_hide(loginWindow);
          gtk_widget_show(menuWindow);
        }
        else{
          printf("Login failed!\n"); 
        }
}
void on_backLoginButton_clicked(GtkButton *b,int argc, char *argv[]){
        gtk_widget_hide(loginWindow);
        gtk_widget_show(window);
}

//Menu
void on_searchMenuButton_clicked(GtkButton *b){
        gtk_widget_hide(menuWindow);
        gtk_widget_show(searchWindow);
}
void on_logoutMenuButton_clicked(GtkButton *b){
        gtk_widget_hide(menuWindow);
        gtk_widget_show(window);
}
//Search
void on_searchButton_clicked(GtkWidget *button) {
        const gchar *search_entry = gtk_entry_get_text(GTK_ENTRY(searchEntry));
        char search_str[SIZE];
        strcpy(search_str, search_entry);
        GtkListStore *list_store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
        GtkTreeIter iter;
        //     gtk_list_store_clear(list_store);
        gtk_list_store_append(list_store, &iter);
        gtk_list_store_set(list_store, &iter, 0, "John", 1, "1.1.1.1", 2, "1234", -1);
        gtk_list_store_append(list_store, &iter);
        gtk_list_store_set(list_store, &iter, 0, "Jane", 1, "1.2.3.4", 2, "2345", -1);
        gtk_list_store_append(list_store, &iter);
        gtk_list_store_set(list_store, &iter, 0, "Bob", 1, "2.2.2.2", 2, "8888", -1);

        // Set the model for the new tree view
        gtk_tree_view_set_model(treeView, GTK_TREE_MODEL(list_store));

        // Create the TreeViewColumns
        GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
        GtkTreeViewColumn *column_user = gtk_tree_view_column_new_with_attributes("User", renderer, "text", 0, NULL);
        GtkTreeViewColumn *column_ip = gtk_tree_view_column_new_with_attributes("IP", renderer, "text", 1, NULL);
        GtkTreeViewColumn *column_port = gtk_tree_view_column_new_with_attributes("Port", renderer, "text", 2, NULL);

        // Add the columns to the new tree view
        gtk_tree_view_append_column(treeView, column_user);
        gtk_tree_view_append_column(treeView, column_ip);
        gtk_tree_view_append_column(treeView, column_port);
}

GtkWidget* createHomePageView(InotifyThreadArgs inotify_args){

        strcpy(home_inotify_args.path_to_watch, inotify_args.path_to_watch);
        home_inotify_args.client_sock = inotify_args.client_sock;
        home_inotify_args.port = inotify_args.port;
        strcpy(home_inotify_args.server_port, inotify_args.server_port);
        home_inotify_args.inotify_tid = inotify_args.inotify_tid;
        strcpy(home_inotify_args.ip, inotify_args.ip);

//Build homepage
        menuBuilder = gtk_builder_new_from_file("xml/homepage.glade");
        window = GTK_WIDGET(gtk_builder_get_object(menuBuilder, "window"));
        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        fixed = GTK_WIDGET(gtk_builder_get_object(menuBuilder, "fixed"));
        registerButton = GTK_WIDGET(gtk_builder_get_object(menuBuilder, "registerButton"));
        loginButton = GTK_WIDGET(gtk_builder_get_object(menuBuilder, "loginButton"));
        appNameLabel = GTK_WIDGET(gtk_builder_get_object(menuBuilder, "appNameLabel"));
        g_signal_connect(registerButton, "clicked", G_CALLBACK(on_registerButton_clicked), NULL);
        g_signal_connect(loginButton, "clicked", G_CALLBACK(on_loginButton_clicked), NULL);

//Build Register
        builderRegister = gtk_builder_new_from_file("xml/register.glade");
        registerWindow = GTK_WIDGET(gtk_builder_get_object(builderRegister, "registerWindow"));
        g_signal_connect(registerWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        registerFixed = GTK_WIDGET(gtk_builder_get_object(builderRegister, "registerFixed"));
        registerLabel = GTK_WIDGET(gtk_builder_get_object(builderRegister, "registerLabel"));
        usernameRegisterLabel = GTK_WIDGET(gtk_builder_get_object(builderRegister, "usernameLabel"));
        passwordRegisterLabel = GTK_WIDGET(gtk_builder_get_object(builderRegister, "passwordLabel"));
        usernameRegisterEntry = GTK_WIDGET(gtk_builder_get_object(builderRegister, "usernameEntry"));
        passwordRegisterEntry = GTK_WIDGET(gtk_builder_get_object(builderRegister, "passwordEntry"));
        submitRegisterButton = GTK_WIDGET(gtk_builder_get_object(builderRegister, "submitButton"));
        backRegisterButton = GTK_WIDGET(gtk_builder_get_object(builderRegister, "backButton"));
        g_signal_connect(submitRegisterButton, "clicked", G_CALLBACK(on_submitRegisterButton_clicked), NULL);
        g_signal_connect(backRegisterButton, "clicked", G_CALLBACK(on_backRegisterButton_clicked), NULL);
//Build Login
        builderLogin = gtk_builder_new_from_file("xml/login.glade");
        loginWindow = GTK_WIDGET(gtk_builder_get_object(builderLogin, "loginWindow"));
        g_signal_connect(loginWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        loginFixed = GTK_WIDGET(gtk_builder_get_object(builderLogin, "loginFixed"));
        loginLabel = GTK_WIDGET(gtk_builder_get_object(builderLogin, "loginLabel"));
        usernameLoginLabel = GTK_WIDGET(gtk_builder_get_object(builderLogin, "usernameLabel"));
        passwordLoginLabel = GTK_WIDGET(gtk_builder_get_object(builderLogin, "passwordLabel"));
        usernameLoginEntry = GTK_WIDGET(gtk_builder_get_object(builderLogin, "usernameEntry"));
        passwordLoginEntry = GTK_WIDGET(gtk_builder_get_object(builderLogin, "passwordEntry"));
        submitLoginButton = GTK_WIDGET(gtk_builder_get_object(builderLogin, "submitLoginButton"));
        backLoginButton = GTK_WIDGET(gtk_builder_get_object(builderLogin, "backButton"));
        g_signal_connect(submitLoginButton, "clicked", G_CALLBACK(on_submitLoginButton_clicked), NULL);
        g_signal_connect(backLoginButton, "clicked", G_CALLBACK(on_backLoginButton_clicked), NULL);

//Build menu
        menuBuilder = gtk_builder_new_from_file("xml/menu.glade");
        menuWindow = GTK_WIDGET(gtk_builder_get_object(menuBuilder, "menuWindow"));
        g_signal_connect(menuWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        menuFixed = GTK_WIDGET(gtk_builder_get_object(menuBuilder, "menuFixed"));
        menuLabel = GTK_WIDGET(gtk_builder_get_object(menuBuilder, "menuLabel"));
        logoutMenuButton = GTK_WIDGET(gtk_builder_get_object(menuBuilder, "logoutButton"));
        searchMenuButton = GTK_WIDGET(gtk_builder_get_object(menuBuilder, "searchButton"));
        downloadMenuButton = GTK_WIDGET(gtk_builder_get_object(menuBuilder, "downloadButton"));

        g_signal_connect(searchMenuButton, "clicked", G_CALLBACK(on_searchMenuButton_clicked), NULL);

//Build search
        searchBuilder = gtk_builder_new_from_file("xml/search.glade");
        searchWindow = GTK_WIDGET(gtk_builder_get_object(searchBuilder, "window"));
        g_signal_connect(searchWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        searchFixed = GTK_WIDGET(gtk_builder_get_object(searchBuilder, "fixed"));

        searchEntry = GTK_WIDGET(gtk_builder_get_object(searchBuilder, "searchEntry"));
        searchButton = GTK_WIDGET(gtk_builder_get_object(searchBuilder, "searchButton"));
        treeView = GTK_WIDGET(gtk_builder_get_object(searchBuilder, "treeView"));

        g_signal_connect(searchButton, "clicked", G_CALLBACK(on_searchButton_clicked), NULL);

        return window;
}



