#ifndef GEOSIM_MESH_H
#define GEOSIM_MESH_H

#include <stdint.h>

typedef struct {
    int    n_verts;
    int    n_edges;
    float *pos;     /* [n_verts * 3]  xyz on unit sphere */
    int   *adj_off; /* [n_verts + 1]  CSR row offsets */
    int   *adj;     /* [n_edges * 2]  CSR adjacency list */
} Mesh;

/*
 * Generate an icosahedral geodesic mesh by subdividing each triangle
 * `subdivisions` times. subdivisions=0 gives the base icosahedron (12 verts).
 * Vertex count: 10 * 4^s + 2.
 *
 * Returns 0 on success. Caller owns the mesh and must call mesh_free().
 */
int  mesh_create(Mesh *m, int subdivisions);
void mesh_free(Mesh *m);

#endif
