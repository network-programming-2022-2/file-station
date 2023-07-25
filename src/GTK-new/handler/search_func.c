#include "../../includes/common.h"
#include "../../models/postgresql/file.h"
#include "../../models/postgresql/user.h"
#include "../../models/postgresql/postgresql.h"

#define BUFF_SIZE 8192
#define SIZE 1024

#define MAX_EVENTS 1024
#define LEN_NAME 16
#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (MAX_EVENTS * (EVENT_SIZE + LEN_NAME))

int search(int client_socks,char filename[SIZE]){
    
    
    // FileResult fileResult = get_files_by_name(filename);
    // User result_user = get_user_by_id(fileResult.files->user_id);
    // if(result_user.server_port != NULL && fileResult.files != NULL){
    //     strcpy(user_name, result_user.username);
    //     *port = result_user.server_port;
    //     return 1;
    // }
    // return -1;
}
