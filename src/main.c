#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

// prints errno and returns -1 on error, and the file descriptor itself on success.
static int openfd(const char* path, int oflags);

int main(int argc, char** argv)
{
  const char* path = NULL;
  if (argc != 2)
  {
    fprintf(stderr, "cat-clone:arg1(pathname)\n");
    return -1;
  }
  path = argv[1];

  // ---------------------------reading and writing-----------------------------
  


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
