#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

void error(const char *msg) {
  perror(msg);
  exit(1);
}

char* read_file(char *filename) {
  char *buffer = NULL;
  int string_size, read_size;
  FILE *handler = fopen(filename, "r");

  if (handler) {
    // Seek the last byte of the file
    fseek(handler, 0, SEEK_END);
    // Offset from the first to the last byte, or in other words, filesize
    string_size = ftell(handler);
    // go back to the start of the file
    rewind(handler);

    // Allocate a string that can hold it all
    buffer = (char*) malloc(sizeof(char) * (string_size + 1) );

    // Read it all in one operation
    read_size = fread(buffer, sizeof(char), string_size, handler);

    // fread doesn't set it so put a \0 in the last position
    // and buffer is now officially a string
    buffer[string_size] = '\0';

    if (string_size != read_size) {
      free(buffer);
      buffer = NULL;
    }

    // Always remember to close the file.
    fclose(handler);
  }
  return buffer; 
}

int read_port(char *filename) {
  int port;
  char* buffer = read_file(filename);
  if (buffer == NULL) {
    printf("Couldn't read port from file: %s", filename);
    error("could not read file");
  }
  else {
    port = atoi(buffer);
    free(buffer);
  }
  return port;
}
