#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>

void log_message(const char *message, const char *file) {
    syslog(LOG_DEBUG, "Writing %s to %s", message, file);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file> <string>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *filename = argv[1];
    const char *content = argv[2];

    // Open the file for writing
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        syslog(LOG_ERR, "Error opening file %s: %s", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Write the content to the file
    fprintf(file, "%s\n", content);
    log_message(content, filename);

    // Close the file
    if (fclose(file) != 0) {
        syslog(LOG_ERR, "Error closing file %s: %s", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return 0;
}
