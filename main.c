#include <stdio.h>
#include <stdlib.h>

struct Buffer {
    char *buffer;
    long size;
};


struct Buffer* read_file(const char *file_name) {
    // Create buffer struct

    // open file
    FILE *file = fopen(file_name, "r");
    
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", file_name);
        return NULL;
    }
    
    // Get the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    
    // Allocate memory for the struct
    struct Buffer* buf = (struct Buffer*)malloc(sizeof(struct Buffer));
    if (buf == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for buffer struct\n");
        fclose(file);
        return NULL;
    }
    buf->size = file_size;

    // Allocate memory for the file content
    buf->buffer = (char *)malloc(buf->size * sizeof(char) + sizeof(char));
    if(buf->buffer == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for file content\n");
        free(buf);
        fclose(file);
        return NULL;
    }
    
    // Read the file content
    size_t read_size = fread(buf->buffer, sizeof(char), buf->size, file);
    if (read_size != buf->size) {
        fprintf(stderr, "Error: Could not read file content\n");
        free(buf->buffer);
        free(buf);
        fclose(file);
        return NULL;
    }
    
    buf->buffer[file_size] = '\0';
    fclose(file);
    
    return buf;
}


void print_hex_line(const char *buffer, size_t size, size_t offset) {
    for (size_t i = offset; i < size+offset; i+=2) {
        fprintf(stdout, "%02X%02X ", buffer[i], buffer[i + 1]);
    }
    fprintf(stdout, "\t");
}

void print_address(size_t address) {
    fprintf(stdout, "%08X\t", address);
}

void print_ascii_line(const char *buffer, size_t size, size_t offset) {
    for (size_t i = offset; i < size+offset; i++) {
        if (buffer[i] >= 32 && buffer[i] <= 126) {
            fprintf(stdout, "%c", buffer[i]);
        } else {
            fprintf(stdout, ".");
        }
    }
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    const char *filename = argv[1];
    struct Buffer *buf = read_file(filename);
    if (buf == NULL) {
        return 1;
    }
    
    int chunk_size = 16;
    for (size_t i = 0; i < buf->size; i+=16) {
        chunk_size = buf->size - i < 16 ? buf->size - i : 16;
        print_address(i);
        print_hex_line(buf->buffer, chunk_size, i);
        print_ascii_line(buf->buffer, chunk_size, i);
        fprintf(stdout, "\n");
    }


    free(buf->buffer);
    free(buf);
    return 0;
}