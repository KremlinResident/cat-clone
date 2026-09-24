#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>

#define BUFSZ 4096

// prints errno and returns -1 on error, and the file descriptor itself on success.
static int openfd(const char* path, int oflags);
// read an open file descriptor into a buffer using [size] bytes.
// returns -1 on error with errno printing, or the bytes read on success. 
static ssize_t readfd(int fd, void* buf, size_t size);

int main(int argc, char** argv)
{
  const char* path = NULL;
  int fd = 0;
  void* buf = NULL;
  if (argc != 2)
  {
    fprintf(stderr, "cat-clone:arg1(pathname)\n");
    return -1;
  }
  path = argv[1];
  fd = open(path, O_RDONLY);
  if (fd == -1)
  {
    return -1; // already printed in open's function body
  }
  buf = malloc(BUFSZ);
  if (!buf)
  {
    if (errno == ENOMEM)
      perror("malloc failure");
    return -1;
  }



  free(buf);
  buf = NULL;
  if (close(fd) == -1)
  {
    perror("failure to close fd");
    abort();
  }
  return 0;
}
static int openfd(const char* path, int oflags)
{
  int fd = open(path, oflags);
  if (fd == -1)
  {
    perror("failure to open file descriptor");
  }
  return fd;  
}
static ssize_t readfd(int fd, void* buf, size_t size)
{
  ssize_t bytesread = 0; // error handling type, not the actual type
  off_t offset = 0;
  // main loop for read and write
  while (true)
  {
    bytesread = read(fd, buf, size);
    if (bytesread == 0)
      break;
    else if (bytesread == -1)
    {
      if (errno == EINTR)
        continue;
      else
      {
        perror("failure to read fd");
        break;
      }
    }
    // write loop, offset is an off_t that starts for buf[0] and continuously writes through buf
    while (offset < bytesread)
    {
      ssize_t byteswritten = write(STDOUT_FILENO, buf, size);
      if (byteswritten == 0)
        break;
      else if (byteswritten == -1)
      {
        if (errno == EINTR)
          continue;
        else
        {
          perror("failure to write fd");
          break;
        }
      }
    }
  }

  return bytesread;
}
