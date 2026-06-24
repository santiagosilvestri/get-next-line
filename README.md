*This project was created as part of the 42 curriculum by sasilves.*

# Get Next Line

`get_next_line` is a custom C function that reads and returns one line at a time from a file descriptor. The project introduces file descriptors, buffered reading, dynamic memory allocation, and the use of static variables to preserve data between function calls.

## Description

The objective of this project is to implement a function capable of reading from a file descriptor line by line, regardless of the chosen `BUFFER_SIZE`.

The function returns the next available line each time it is called. If a newline character is found, the returned line includes it. If the end of the file is reached and the last line does not end with a newline, that final line is still returned. When there is nothing else to read, or if an error occurs, the function returns `NULL`.

```c
char *get_next_line(int fd);
```

Unlike a function that reads an entire file at once, `get_next_line()` reads only what is necessary to return the next line. Any extra content read after a newline is saved for the next call.

## Function behavior

| Situation | Return value |
|---|---|
| A line is read successfully | The line read from the file descriptor. |
| A newline is found | The returned line includes `\n`. |
| End of file is reached after remaining text | The final line is returned without `\n`. |
| End of file is reached with no remaining text | `NULL`. |
| An error occurs | `NULL`. |

## Project structure

```text
.
├── get_next_line.c
├── get_next_line_utils.c
├── get_next_line.h
├── get_next_line_bonus.c
├── get_next_line_utils_bonus.c
└── get_next_line_bonus.h
```

- `get_next_line.c` contains the main function and the core reading logic.
- `get_next_line_utils.c` contains helper functions for string handling and memory management.
- `get_next_line.h` contains the public prototype, required includes, and the default `BUFFER_SIZE`.
- `get_next_line_bonus.c` contains the bonus implementation for multiple file descriptors.
- `get_next_line_utils_bonus.c` contains the helper functions used by the bonus version.
- `get_next_line_bonus.h` contains the bonus header file.

## Algorithm and design choices

### Static storage

The implementation uses a static variable to preserve unread content between calls to `get_next_line()`.

This is necessary because `read()` works with a fixed-size buffer. A single read operation may stop before a full line is available, or it may read more than one line at once. When extra characters are read after a newline, they must be stored and reused in the following call.

In the mandatory version, one static pointer is used:

```c
static char *storage;
```

In the bonus version, a static array is used to keep a separate storage for each file descriptor:

```c
static char *storage[1024];
```

### Reading from the file descriptor

The function reads into a temporary buffer until one of the following conditions occurs:

- a newline character is found in the stored content;
- the end of the file is reached;
- `read()` returns an error.

After each successful call to `read()`, the buffer is appended to the static storage.

This approach avoids reading the entire file in advance and keeps the function focused on returning only the next line.

### Line extraction

Once the storage contains a newline, or the end of the file has been reached, the function allocates a new string for the line to return.

If a newline exists, the line length is calculated up to and including `\n`. If there is no newline, the whole remaining storage is returned as the last line.

### Storage update

After extracting the current line, the storage is updated.

Only the characters that appear after the newline are kept. This remaining content becomes the starting point for the next call to `get_next_line()`.

If there is no newline, or if no content remains after the newline, the storage is freed and set to `NULL`.

### Memory management

All returned lines are dynamically allocated. The caller is responsible for freeing each returned line after use.

The implementation also frees internal storage when an error occurs or when there is no remaining content to keep.

### Data structures

No custom structure is required for this project. The implementation uses:

- a static pointer to preserve unread content in the mandatory version;
- a static array of pointers for the bonus version;
- a temporary buffer for each read operation;
- helper functions to measure, search, join, and free strings.

This keeps the solution simple and aligned with the scope of the project.

## Instructions

### Compilation

The project can be compiled by defining `BUFFER_SIZE` at compilation time.

Example with `BUFFER_SIZE=42`:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl
```

Other buffer sizes can also be tested:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=1 get_next_line.c get_next_line_utils.c main.c -o gnl
cc -Wall -Wextra -Werror -D BUFFER_SIZE=9999 get_next_line.c get_next_line_utils.c main.c -o gnl
```

If `BUFFER_SIZE` is not defined during compilation, the header provides a default value.

### Bonus compilation

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main_bonus.c -o gnl_bonus
```

### Usage

Include the header in a C source file:

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int     fd;
    char    *line;

    fd = open("file.txt", O_RDONLY);
    if (fd < 0)
        return (1);
    line = get_next_line(fd);
    while (line)
    {
        printf("%s", line);
        free(line);
        line = get_next_line(fd);
    }
    close(fd);
    return (0);
}
```

Run the program with:

```bash
./gnl
```

## Bonus

The bonus implementation allows `get_next_line()` to manage several file descriptors at the same time.

Example behavior:

```c
line1 = get_next_line(fd1);
line2 = get_next_line(fd2);
line3 = get_next_line(fd1);
```

Each file descriptor keeps its own reading state, so the content of one descriptor is not mixed with the content of another one.

## Testing

The implementation should be tested with different files, descriptor types, and buffer sizes.

Useful cases include:

- an empty file;
- a file with one line;
- a file with multiple lines;
- a file without a final newline;
- a file containing only newline characters;
- very small `BUFFER_SIZE` values, such as `1`;
- larger `BUFFER_SIZE` values, such as `9999`;
- repeated calls until `NULL` is returned;
- invalid or closed file descriptors;
- multiple file descriptors for the bonus version;
- reading from standard input.

Example test loop:

```c
line = get_next_line(fd);
while (line)
{
    printf("%s", line);
    free(line);
    line = get_next_line(fd);
}
```

## Resources

- `man 2 read` - low-level input from file descriptors.
- `man 2 open` - opening files and obtaining file descriptors.
- `man 3 malloc` - dynamic memory allocation.
- `man 3 free` - releasing dynamically allocated memory.
- 42 documentation about static variables.
- Peer review and testing with different `BUFFER_SIZE` values.

### Use of AI

AI tools were used as a learning and review assistant to discuss static variables, buffered reading, memory management, and the structure and wording of this README.

## Author

**Santiago Silvestri** - `sasilves`
