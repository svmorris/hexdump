#include <stdio.h>
#include <stdlib.h>


void *read_file(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", file_name);
        return NULL;
    }
    
    // Get the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    // Allocate memory for the file content
    char *buffer = (char *)malloc(file_size * sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for file content\n");
        return NULL;
    }
    
    // Read the file content
    size_t read_size = fread(buffer, sizeof(char), file_size, file);
    if (read_size != file_size) {
        fprintf(stderr, "Error: Could not read file content\n");
        free(buffer);
        fclose(file);
        return NULL;
    }
    
    buffer[file_size] = '\0';
    fclose(file);
    
    return buffer;
}



int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    const char *filename = argv[1];
    char *file_content = read_file(filename);
    // print file content
    fprintf(stdout, "%s\n", file_content);
    free(file_content);
    return 0;
}