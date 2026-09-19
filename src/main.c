#define _LARGEFILE64_SOURCE
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <fcntl.h>

struct buffer
{
  char* src;
  ssize_t sz;
};


// assume ownership of dest goes to the caller.
// explicit and doesn't have side effects, only reads the file into dest.
static int read_file(const char* restrict pathname, char* restrict dest, ssize_t destsz, ssize_t* restrict file_sz_out);

int main(int argc, char** argv)
{
  struct buffer bf = {0};
  ssize_t filesz = 0;
  printf("arg1:pathname\n");
  if (argc != 2)
  {
    fprintf(stderr, "args must only be 2, CLI itself and pathname\n");
    exit(EXIT_FAILURE);
  }
  bf.sz = 4096;
  bf.src = malloc((size_t)bf.sz + 1); // \0
  if (!bf.src)
    exit(EXIT_FAILURE);

  // NOTE: faulty static analysis
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
  read_file(argv[1], bf.src, bf.sz, &filesz);
  bf.src[filesz] = '\0';
#pragma clang diagnostic pop
  printf("%s\n", bf.src);
  exit(EXIT_SUCCESS);
}

static int read_file(const char* restrict pathname, char* restrict dest, ssize_t destsz, ssize_t* restrict file_sz_out)
{
  int savederrno = errno;
  int retval = 0;
  
  char* tmpdest = NULL;
  ssize_t bytesread = 0;
  off_t filesz = 0;
  int fd = open(pathname, O_RDONLY);
  if (fd == -1)
  {
    perror("open file");
    retval = -1;
    goto cleanup;
  }
  if (destsz < 0)
  {
    fprintf(stderr, "size can't equal zero\n destsz is ssize_t (aka long) for POSIX handling only\n");
    retval = -1;
    goto cleanup;
  }
  if ((filesz = lseek(fd, 0, SEEK_END)) == -1)
  {
    perror("unseekable file descriptor");
    retval = -1;
    goto cleanup;
  }
  lseek(fd, 0, SEEK_SET);

  // using > assuming null termination is manual
  if (filesz > destsz)
  {
    fprintf(stderr, "filesz bigger than destsz, filesz:%zi, destsz:%zi\n", filesz, destsz);
    retval = -1;
    goto cleanup;
  }
  tmpdest = dest;
  bytesread = read(fd, tmpdest, (size_t)destsz);
  if (bytesread == -1)
  {
    perror("POSIX read function");
    retval = -1;
    goto cleanup;
  }
  if (bytesread < filesz)
  {
    fprintf(stderr, "partial read, bytes:%zi\n", bytesread);
    retval = -1;
    goto cleanup;
  }
  dest = tmpdest;
  *file_sz_out = filesz;
cleanup:
  if (fd != -1)
    close(fd);
  errno = savederrno;
  return retval;
}
