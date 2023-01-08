#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <time.h>

#define BUFFER_WIDTH 1920
#define BUFFER_HEIGHT 1080

static const int buffer_width = BUFFER_WIDTH;
static const int buffer_height = BUFFER_HEIGHT;
static uint16_t (*mem)[BUFFER_WIDTH];

enum mode {
  Fill,
  Outline,
  Clear
} typedef mode;

struct color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} typedef color;

void write (int x, int y, color color) {
  uint16_t bits = (color.r >> 3) << 11 | (color.g >> 2) << 5 | color.b >> 3;
  mem[y][x] = bits;
}

void repl () {
  char command[1000];

  int cursor_x = 0;
  int cursor_y = 0;
  mode mode = Fill;
  color color = {
    .r = 255,
    .g = 255,
    .b = 255,
  };

  while (1) {
    if (feof(stdin)) break;
    fgets(command, 999, stdin);

    int arg1, arg2, arg3;
    if (3 == sscanf(command, "color %d %d %d\n", &arg1, &arg2, &arg3)) {
      color.r = arg1;
      color.g = arg2;
      color.b = arg3;
    } else if (2 == sscanf(command, "move %d %d\n", &arg1, &arg2)) {
      cursor_x = arg1;
      cursor_y = arg2;
    } else if (2 == sscanf(command, "rect %d %d\n", &arg1, &arg2)) {
      for (int dx = 0; dx < arg1; dx++) {
        for (int dy = 0; dy < arg2; dy++) {
          write(cursor_x + dx, cursor_y + dy, color);
        }
      }
    } else if (0 == sscanf(command, "exit\n")) {
      break;
    } else {
      printf("Unrecognized command '%s'\n", command);
    }
  }

  if (!feof(stdin)) fclose(stdin);
}

int main (int argc, char** argv) {
  int fd = open("/dev/fb0", O_RDWR);
  mem = (uint16_t (*)[buffer_width]) mmap(NULL, buffer_width * buffer_height * 2, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

  repl();

  munmap(mem, buffer_width * buffer_height * 2);
}
