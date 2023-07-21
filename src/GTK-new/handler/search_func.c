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

int search(char filename[SIZE]){
    FileResult fileResult = get_files_by_name(filename);
    printf("%s", fileResult.files->filename);
    return 1;
}
