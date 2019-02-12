#include "vertex.h"
#include <math.h>
#include <ncurses.h>

#define PI (3.1415926535)

double vec3_xznorm(struct vec3 *v) { return sqrt(pow(v->x, 2) + pow(v->z, 2)); }

void vec3_rotate(struct vec3 *v, double theta) {
  struct vec3 tmp = *v;
  tmp.x = v->x * cos(theta) - v->z * sin(theta);
  tmp.z = v->x * sin(theta) + v->z * cos(theta);
  *v = tmp;
}

int vec3_cmp(const void *a, const void *b) {
  const struct vec3 *v1 = a;
  const struct vec3 *v2 = b;
  return v1->z > v2->z;
}

void vec3_center(struct vec3 *pos, int cols, int rows) {
  pos->x -= (cols - 1) / 2.0;
  pos->y -= (rows - 1) / 2.0;
}

void vertex_draw(struct vertex *v, int scale, int xoffset, int yoffset) {
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
    normalized = v->norm.x - v->norm.z;            // Sum of vectors
    normalized /= sqrt(2) * vec3_xznorm(&v->norm); // Normalize
    normalized = (normalized + 1) / 2.0;           // Adjust to the range [0,1]
    n = normalized * NCOLORS;
  } else {
    // Depth based
    normalized = (v->pos.z / vec3_xznorm(&v->pos) + 1) / 2.0;
    n = normalized * NCOLORS + 1;
  }

  // Draw character in that color
  attron(COLOR_PAIR(n));
  mvaddch((v->pos.y * scale + yoffset), v->pos.x * scale + xoffset, v->c);
  attroff(COLOR_PAIR(n));
}

void draw_vertices(struct vertex *vertices, int nvertices) {
  int maxy, maxx; // Place in the middle
  getmaxyx(stdscr, maxy, maxx);

  // Draw points
  for (int i = 0; i < nvertices; ++i) {
    struct vertex *v = &vertices[i];
    vertex_draw(v, 1, maxx / 2.0, maxy / 2.0);

    // Rotate point
    vec3_rotate(&v->pos, PI / 128);
    vec3_rotate(&v->norm, PI / 128);
  }
  refresh();
}

void read_vertices(struct vertex *vertices, char *contents, int len) {
  int idx = 0, x = 0, y = 0;
  for (int i = 0; i < len; ++i) {
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
