#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PI (3.1415926535)

struct vec3 {
  double x, y, z;
};

struct vertex {
  struct vec3 pos;
  struct vec3 norm;
  char c;
};

// Return the norm of a vector
double xznorm(struct vec3 *v) { return sqrt(pow(v->x, 2) + pow(v->z, 2)); }

// Pretty print the point
void draw_vertex(struct vertex *v, int scale, int xoffset, int yoffset) {
  if (v->c == '\0') {
    return;
  }

  // Set some colors
  const int BLACK = 232;
  const int NCOLORS = 16;
  for (int i = 1; i <= NCOLORS; ++i) {
    init_pair(i, BLACK + i + 2, COLOR_BLACK);
  }

  // Pick a color based on v->z
  float normalized;
  int n;
  if (1) {
    // Lighting from side
    normalized = v->norm.x - v->norm.z;       // Sum of vectors
    normalized /= sqrt(2) * xznorm(&v->norm); // Normalize
    normalized = (normalized + 1) / 2.0;      // Adjust to the range [0,1]
    n = normalized * NCOLORS;
  } else {
    // Depth based
    normalized = (v->pos.z / xznorm(&v->pos) + 1) / 2.0;
    n = normalized * NCOLORS + 1;
  }

  // Draw character in that color
  attron(COLOR_PAIR(n));
  mvaddch((v->pos.y * scale + yoffset), v->pos.x * scale + xoffset, v->c);
  attroff(COLOR_PAIR(n));
}

// Rotate a vec3 by theta degrees
void rotate(struct vec3 *v, double theta) {
  struct vec3 tmp = *v;
  tmp.x = v->x * cos(theta) - v->z * sin(theta);
  tmp.z = v->x * sin(theta) + v->z * cos(theta);
  *v = tmp;
}

int cmp_vec(const void *a, const void *b) {
  const struct vec3 *v1 = a;
  const struct vec3 *v2 = b;
  return v1->z > v2->z;
}

// Center the vertices
void center(struct vertex *vertices, int length, int cols, int rows) {
  for (int i = 0; i < length; ++i) {
    struct vertex *v = &vertices[i];
    v->pos.x -= (cols - 1) / 2.0;
    v->pos.y -= (rows - 1) / 2.0;
  }
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

// Draw the image
void draw(struct vertex *vertices, int length) {
  int maxy, maxx; // Place in the middle
  getmaxyx(stdscr, maxy, maxx);

  // Draw points
  for (int i = 0; i < length; ++i) {
    struct vertex *v = &vertices[i];
    draw_vertex(v, 1, maxx / 2.0, maxy / 2.0);

    // Rotate point
    rotate(&v->pos, PI / 128);
    rotate(&v->norm, PI / 128);
  }
  refresh(); // Actually draw
}

// Return the length of the longest row
int count_columns(char *str, int len) {
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
int count_rows(char *str, int len) {
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

// Read vertices from a string
void read_vertices(struct vertex *vertices, char *contents, int fsize) {
  int idx = 0, x = 0, y = 0;
  for (int i = 0; i < fsize; ++i) {
    char c = contents[i];
    char symbol = c == '\n' || c == ' ' ? '\0' : c;

    vertices[idx++] = (struct vertex){{x, y, 0}, {0, 0, 1}, symbol};
    vertices[idx++] = (struct vertex){{x, y, 0.1}, {0, 0, -1}, symbol};

    if (c == '\n') {
      x = 0;
      ++y;
    } else {
      ++x;
    }
  }
}

int main(int argc, char *argv[]) {

  // One argument required
  if (argc <= 1) {
    printf("Usage: %s <file>", argv[0]);
    exit(1);
  }

  // Open the input file
  FILE *fp = fopen(argv[1], "r");
  if (!fp) {
    perror(argv[0]);
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
  int cols = count_columns(contents, fsize);
  int rows = count_rows(contents, fsize);
  center(vertices, nvertices, cols, rows);
  free(contents);

  setup(); // Start ncurses

  while (1) {
    erase(); // Clear screen

    // Sort data based on depth
    qsort(vertices, nvertices, sizeof(struct vertex), cmp_vec);

    draw(vertices, nvertices);

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
