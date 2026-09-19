#include <bits/posix1_lim.h>
#include <fcntl.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

// USING SSIZE_T FOR POSIX ERROR HANDLING

// Manual and verbose.
// Mo overflow check, nor null termination.
// Ownership goes to caller.
// Returns -1 on error and reports errno.
static int readfile(const char* pathname, char* dest, ssize_t filesz);
// Returns -1 on error and reports errno.
static ssize_t getfilesz(const char* pathname);

int main(int argc, char** argv)
{
  ssize_t filesz = 0;
  char* buffer = NULL;

  if (argc != 2) {
    fprintf(stderr, "cat-clone: arg1:[pathname]");
    return -1;
  }
  
  filesz = getfilesz(argv[1]);
  if (filesz == -1)
    abort();

  buffer = malloc((size_t)filesz + 1); // \0
  if (!buffer)
    abort();
  if (readfile(argv[1], buffer, filesz) == -1)
  {
    free(buffer);
    buffer = NULL;
    abort();
  }
  if (buffer)
  {
    free(buffer);
    buffer = NULL;
  }
  return 0;
}

static int readfile(const char* pathname, char* dest, ssize_t filesz)
{
  int fd = open(pathname, O_RDONLY);
  int retval = 0;
  ssize_t bytesread = 0;

  if (filesz < 0 || filesz >= SSIZE_MAX - 1)
  {
    fprintf(stderr, "invalid filesz, filesz:%zi\n", filesz);
    retval = -1;
    goto cleanup;
  }

  if (fd == -1)
  {
    perror("error open file");
    retval = -1;
    goto cleanup;
  }

  // read the file in a loop if bytesread != 0 (EOF) or -1 (error)
  while ((bytesread = read(fd, dest, (size_t)filesz)) > 0)
  {
    if (bytesread == -1)
    {
      if (errno == EINTR)
        continue;
      else
      {
        perror("file reading into buffer could not be done");
        retval = -1;
        goto cleanup;
      }
    }
  }

  goto cleanup;
cleanup:
  if (fd != -1)
    if (close(fd) == -1)
    {
      perror("close file");
      retval = -1;
    }
  return retval;
}

static ssize_t getfilesz(const char* pathname)
{
  int fd = open(pathname, O_RDONLY);
  ssize_t retval = 0;
  if (fd == -1)
  {
    perror("error open file for getting size");
    retval = -1;
    goto cleanup;
  }
  retval = (ssize_t)lseek(fd, 0, SEEK_END);
  if (retval == -1)
  {
    perror("error lseek()ing file");
    goto cleanup;
  }

  goto cleanup;
cleanup:
  if (fd != -1)
    if (close(fd) == -1)
    {
      perror("close file");
      retval = -1;
    }
  return retval;
}
