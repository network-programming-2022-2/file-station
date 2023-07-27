#include<gtk/gtk.h>
#include "../main.h"
#include "register.c"
#include "login.c"
#include "logout.c"
#include "search.c"

InotifyThreadArgs home_inotify_args;

GtkWidget *window;
GtkWidget *fixed;
GtkWidget *registerButton;
GtkWidget *loginButton;
GtkWidget *appNameLabel;
GtkWidget *notifyHomePageLabel;

GtkBuilder *homepageBuilder; 
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
GtkWidget *notifyRegisterLabel;

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
GtkWidget *notifyLoginLabel;

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
GtkWidget *comboBox;
GtkWidget *backSearchButton;
GtkWidget *downloadButton;
GtkListStore *list_store;
GtkWidget *notifyLabel;
typedef struct {
    char *user;
    char *ip;
    char *port;
} RowData;

RowData array[100];

void on_registerButton_clicked(GtkButton *b){
        gtk_widget_hide(window);
        gtk_widget_show(registerWindow);
        gtk_label_set_text(GTK_LABEL(notifyRegisterLabel), "");

}
void on_loginButton_clicked(GtkButton *b,int argc, char *argv[]){
        gtk_widget_hide(window);
        gtk_widget_show(loginWindow);
        gtk_label_set_text(GTK_LABEL(notifyLoginLabel), "");

        
       
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
          gtk_label_set_text(GTK_LABEL(notifyRegisterLabel), "Register failed! Try again");
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
        int peer_server_fd;
        bool ok = handle_login(":", password, home_inotify_args, &peer_server_fd);
        fflush(stdout);
        if (ok){
          home_inotify_args.peer_server_fd = peer_server_fd;
          gtk_widget_hide(loginWindow);
          gtk_widget_show(menuWindow);
        }
        else{
          gtk_label_set_text(GTK_LABEL(notifyLoginLabel), "Login failed! Try again");
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
        bool ok = handle_logout(home_inotify_args.client_sock, ":", home_inotify_args.username);
        printf("Logout: %d\n", ok);
        fflush(stdout);
        if (ok){
          close(home_inotify_args.peer_server_fd);
          printf("Logout successful!\n");
          gtk_widget_hide(menuWindow);
          gtk_widget_show(window);
        }
        else{
          printf("Logout failed!\n"); 
        }
}
//Search
void on_downloadButton_clicked(GtkWidget *button) {
        const gchar *downloadIndex = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(comboBox));
        if(downloadIndex==NULL){
                return;
        }
        RowData downloadData;
        downloadData.ip = array[g_ascii_digit_value(*downloadIndex)-1].ip;
        downloadData.port = array[g_ascii_digit_value(*downloadIndex)-1].port;
        downloadData.user = array[g_ascii_digit_value(*downloadIndex)-1].user;
        // char filename[SIZE] = "hello.txt";
        const gchar *search_entry = gtk_entry_get_text(GTK_ENTRY(searchEntry));
        char filename[SIZE];
        strcpy(filename, search_entry);
        printf("\nsearch entry:%s", filename);

        printf(" %s, %s, %s", downloadData.ip, downloadData.port, downloadData.user);

        sending(downloadData.ip, atoi(home_inotify_args.server_port), atoi(downloadData.port), filename);
        
        fflush(stdout);
        gtk_label_set_text(GTK_LABEL(notifyLabel), "Download Successfully!");
}

void on_backSearchButton_clicked(GtkButton *b,int argc, char *argv[]){
        gtk_widget_hide(searchWindow);
        gtk_widget_show(menuWindow);
}

void on_searchButton_clicked(GtkWidget *button) {
    gtk_combo_box_text_remove_all(comboBox);
    gtk_label_set_text(GTK_LABEL(notifyLabel), "");

    const gchar *filename = gtk_entry_get_text(GTK_ENTRY(searchEntry));
    printf("%s\n", filename);

    SearchResult result[100];
    int num_files;
    bool success = handle_search(home_inotify_args.client_sock, ":", home_inotify_args.username, filename, result, &num_files);
    if (success == true)
    {
      printf("searching ...\n");
      printf("%d\n", num_files);
    }
    else 
    {
      printf("search failed...\n");
    }

    char port_str[20];
    for (int i = 0; i < num_files; i++) {
        array[i].user = strdup(result[i].username);
        array[i].ip = strdup(result[i].ip);
        snprintf(port_str, sizeof(port_str), "%d", result[i].port);
        array[i].port = strdup(port_str);

        printf("\n%s %s %s\n", array[i].user, array[i].ip, array[i].port);
    }

    gtk_list_store_clear(list_store);

    GtkTreeIter iter;
    for(int i=0; i<num_files; i++){
        if(array[i].user==NULL){
           break;
        } 
        gtk_list_store_append(list_store, &iter);
        gtk_list_store_set(list_store, &iter, 0, array[i].user, 1, array[i].ip, 2, array[i].port, -1);
    }

    gtk_widget_queue_draw(GTK_WIDGET(list_store));

    //Init ComboBox
    for (int i = 1; i <= num_files; i++) {
        char text[10];
        snprintf(text, sizeof(text), "%d", i);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboBox), text);
    }
}

GtkWidget* createHomePageView(InotifyThreadArgs inotify_args){

        strcpy(home_inotify_args.path_to_watch, inotify_args.path_to_watch);
        home_inotify_args.client_sock = inotify_args.client_sock;
        home_inotify_args.port = inotify_args.port;
        strcpy(home_inotify_args.server_port, inotify_args.server_port);
        home_inotify_args.inotify_tid = inotify_args.inotify_tid;
        strcpy(home_inotify_args.ip, inotify_args.ip);

//Build homepage
        homepageBuilder = gtk_builder_new_from_file("xml/homepage.glade");
        window = GTK_WIDGET(gtk_builder_get_object(homepageBuilder, "window"));
        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        fixed = GTK_WIDGET(gtk_builder_get_object(homepageBuilder, "fixed"));
        registerButton = GTK_WIDGET(gtk_builder_get_object(homepageBuilder, "registerButton"));
        loginButton = GTK_WIDGET(gtk_builder_get_object(homepageBuilder, "loginButton"));
        appNameLabel = GTK_WIDGET(gtk_builder_get_object(homepageBuilder, "appNameLabel"));
        notifyHomePageLabel = GTK_WIDGET(gtk_builder_get_object(homepageBuilder, "notifyHomePageLabel"));
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
        notifyRegisterLabel = GTK_WIDGET(gtk_builder_get_object(builderRegister, "notifyRegisterLabel"));
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
        notifyLoginLabel = GTK_WIDGET(gtk_builder_get_object(builderLogin, "notifyLoginLabel"));

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
        g_signal_connect(logoutMenuButton, "clicked", G_CALLBACK(on_logoutMenuButton_clicked), NULL);

//Build search
        searchBuilder = gtk_builder_new_from_file("xml/search.glade");
        searchWindow = GTK_WIDGET(gtk_builder_get_object(searchBuilder, "searchWindow"));
        g_signal_connect(searchWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        searchFixed = GTK_WIDGET(gtk_builder_get_object(searchBuilder, "searchLayout"));

        searchEntry = GTK_WIDGET(gtk_builder_get_object(searchBuilder, "searchEntry"));
        searchButton = GTK_WIDGET(gtk_builder_get_object(searchBuilder, "searchButton"));
        treeView = GTK_WIDGET(gtk_builder_get_object(searchBuilder, "treeView"));
        
        comboBox = GTK_WIDGET(gtk_builder_get_object(searchBuilder, "comboBox"));
        backSearchButton = GTK_WIDGET(gtk_builder_get_object(searchBuilder, "backSearchButton"));
        downloadButton = GTK_WIDGET(gtk_builder_get_object(searchBuilder, "downloadButton"));
        notifyLabel = GTK_WIDGET(gtk_builder_get_object(searchBuilder, "notifyLabel"));


        g_signal_connect(searchButton, "clicked", G_CALLBACK(on_searchButton_clicked), NULL);
        g_signal_connect(backSearchButton, "clicked", G_CALLBACK(on_backSearchButton_clicked), NULL);
        g_signal_connect(downloadButton, "clicked", G_CALLBACK(on_downloadButton_clicked), NULL);


        list_store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
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

        return window;
}



