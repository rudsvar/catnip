#pragma once

// A vector with 3 dimensions
struct vec3 {
  double x, y, z;
};

// Return the norm of a vec3
// based on its x and z coordinates.
double vec3_xznorm(struct vec3 *v);

// Rotate a vec3
void vec3_rotate(struct vec3 *v, double theta);

// Compare vec3
int vec3_cmp(const void *a, const void *b);

// Center the vertex, given an amount of columns and rows
void vec3_center(struct vec3 *pos, int cols, int rows);

// A vertex with a position,
struct vertex {
  struct vec3 pos;
  struct vec3 norm;
  char c;
};

// Draw a vertex
void vertex_draw(struct vertex *v, int scale, int xoffset, int yoffset);

// Draw all images
void draw_vertices(struct vertex *vertices, int nvertices);

// Read vertices from a string of length len
void read_vertices(struct vertex *vertices, char *contents, int len);
