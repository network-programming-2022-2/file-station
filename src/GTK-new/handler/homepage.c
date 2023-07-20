#include<gtk/gtk.h>

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
void on_registerButton_clicked(GtkButton *b,int argc, char *argv[]){
        gtk_widget_hide(window);
        gtk_widget_show(registerWindow);
       
}
void on_loginButton_clicked(GtkButton *b,int argc, char *argv[]){
        gtk_widget_hide(window);
        gtk_widget_show(loginWindow);
       
}
/// REGISTER

void on_submitRegisterButton_clicked(GtkButton *b){
        gchar *username = gtk_entry_get_text(GTK_ENTRY(usernameRegisterEntry));
        gchar *password = gtk_entry_get_text(GTK_ENTRY(passwordRegisterEntry));
        printf("%s, %s", username, password);

}
void on_backRegisterButton_clicked(GtkButton *b,int argc, char *argv[]){

        gtk_widget_hide(registerWindow);
        gtk_widget_show(window);
       
}
//Login
void on_submitLoginButton_clicked(GtkButton *b){
        gchar *username = gtk_entry_get_text(GTK_ENTRY(usernameLoginEntry));
        gchar *password = gtk_entry_get_text(GTK_ENTRY(passwordLoginEntry));
        printf("%s, %s", username, password);
        fflush(stdout);
        gtk_widget_hide(loginWindow);
        gtk_widget_show(menuWindow);

}
void on_backLoginButton_clicked(GtkButton *b,int argc, char *argv[]){
        gtk_widget_hide(loginWindow);
        gtk_widget_show(window);
}

//Menu
void on_searchMenuButton_clicked(GtkButton *b){

}


GtkWidget* createHomePageView(int argc, char *argv[]){

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

        return window;
}



