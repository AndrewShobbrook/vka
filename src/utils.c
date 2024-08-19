#include <utils/file_handling.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

char *storeFileInBuffer(FILE *fp, uint32_t *file_size) {
  *file_size = getFileSize(fp);
#ifdef DEBUF
  printf("File Size: %d\n", *file_size);
#endif
  char *file_buffer = malloc(*file_size);
  fread(file_buffer, sizeof(char), *file_size, fp);
  return file_buffer;
}

uint32_t getFileSize(FILE *fp) {
  fseek(fp, 0L, SEEK_END);
  uint32_t size = ftell(fp);
  rewind(fp);
  return size;
}
