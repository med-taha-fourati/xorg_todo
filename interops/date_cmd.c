#include "date_cmd.h"

#define MAX_BUFFER_SIZE 100

char* date_cmd() {
    const char* cmd = "/bin/date";
    char* buffer = (char*)malloc(MAX_BUFFER_SIZE * sizeof(char));

    FILE* file = popen(cmd, "r");
    if (file == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    if (fgets(buffer, MAX_BUFFER_SIZE, file) == NULL) {
        perror("fgets");
        exit(EXIT_FAILURE);
    }

    pclose(file);

    return buffer;
}

void free_date_cmd(char* buffer) {
    free(buffer);
}
