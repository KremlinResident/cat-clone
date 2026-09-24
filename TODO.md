
# Cat Clone

  Open files, read into a buffer, write to stdout. Handle multiple args, - for stdin.
  Teaches: open/read/write/close, errno, perror, buffer sizing.
  I'm going to use a reusable buffer that gets completely reset each time the chunk is fully read to stdout.
