#include <stdbool.h>
#include <fcntl.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const size_t BUFSZ = 4096*4;

static int openfd(const char* pathname, int oflags);
static int readfd(int fd, void* buf, size_t n);

int main(int argc, char** argv)
{
  int fd = 0;
  int bytesread = 0;
  char* buf = malloc(BUFSZ);
  if (argc != 2)
  {
    const char* errmessage = "cat-clone: arg1(pathname)\n";
    write(STDERR_FILENO, errmessage, strlen(errmessage));
    return -1;
  }
  fd = openfd(argv[1], O_RDONLY);

  close(fd);
  free(buf);
  return 0;
}
static int openfd(const char* pathname, int oflags)
{
  int fd = open(pathname, oflags);
  if (fd == -1)
  {
    perror("open file failure");
    return -1;
  }
  return fd;
}
static int readfd(int fd, void* buf, size_t n)
{
  int bytesread = 0;
  if (n == SIZE_MAX)
  {
    fprintf(stderr, "can't read by SIZE_MAX chunks");
    return -1;
  }
  while (true)
  {
  
  }
  return bytesread;
}

