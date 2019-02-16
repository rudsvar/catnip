#include "vertex.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Return the length of the longest row
int row_count(char *str, int len) {
  int cols = 0, x = 0;
  for (int i = 0; i < len; ++i) {
    if (str[i] == '\n') {
      x = 0;
    } else {
      ++x;
    }
    if (x > cols) {
      cols = x;
    }
  }
  return cols;
}

// Return the number of rows
int column_count(char *str, int len) {
  int rows = 0;
  for (int i = 0; i < len; ++i) {
    if (str[i] == '\n') {
      ++rows;
    }
  }
  return rows;
}

int filesize(FILE *fp) {
  int size;
  fseek(fp, 0, SEEK_END); // seek to end of file
  size = ftell(fp);       // get current file pointer
  fseek(fp, 0, SEEK_SET); // seek back to beginning of file
  return size;
}

// Set up ncurses
void setup() {
  initscr();
  nodelay(stdscr, 1);
  cbreak();
  start_color();
  noecho();
  curs_set(0);
}

int main(int argc, char *argv[]) {

  // One argument required
  if (argc <= 1) {
    printf("Usage: %s <file>\n", argv[0]);
    exit(1);
  }

  // Open the input file
  const char *filename = argv[1];
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    char buf[256];
    snprintf(buf, 256, "%s: cannot access '%s'", argv[0], filename);
    perror(buf);
    exit(1);
  }

  // Read contents
  int fsize = filesize(fp);
  char *contents = malloc(fsize + 1);
  int chars_read = fread(contents, sizeof(char), fsize, fp);
  fclose(fp);

  // Check if read properly
  if (fsize != chars_read) {
    perror(argv[0]);
    exit(1);
  }

  int nvertices = fsize * 2; // Allocate memory and read vertices
  struct vertex *vertices = calloc(nvertices, sizeof(struct vertex));
  read_vertices(vertices, contents, fsize);

  // Center the vertices
  int cols = row_count(contents, fsize);
  int rows = column_count(contents, fsize);
  for (int i = 0; i < nvertices; ++i) {
    vec3_center(&vertices[i].pos, cols, rows);
  }
  free(contents);

  setup(); // Start ncurses

  while (1) {
    erase(); // Clear screen

    // Sort data based on depth
    qsort(vertices, nvertices, sizeof(struct vertex), vec3_cmp);

    draw_vertices(vertices, nvertices);

    // Graceful exit
    if (getch() == 'q') {
      break;
    }

    const int UPDATES_PER_SECOND = 60;
    const int MS = 1000 / UPDATES_PER_SECOND;
    usleep(MS * 1000);
  }

  free(vertices);
  endwin();
}
