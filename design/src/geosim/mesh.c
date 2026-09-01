#include "mesh.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ---- hash map for deduplicating midpoints during subdivision ---- */

typedef struct { int64_t key; int val; } Edge_Entry;

typedef struct {
    Edge_Entry *slots;
    int         cap;
} Edge_Map;

static int64_t edge_key(int a, int b)
{
    if (a > b) { int t = a; a = b; b = t; }
    return (int64_t)a << 32 | (int64_t)b;
}

static void emap_init(Edge_Map *m, int cap)
{
    m->cap   = cap;
    m->slots = calloc(cap, sizeof(Edge_Entry));
    for (int i = 0; i < cap; i++) m->slots[i].key = -1;
}

static void emap_free(Edge_Map *m) { free(m->slots); }

static int emap_get_or_insert(Edge_Map *m, int a, int b, int new_val)
{
    int64_t k = edge_key(a, b);
    uint32_t h = (uint32_t)(k * 2654435761u) % (uint32_t)m->cap;
    for (;;) {
        if (m->slots[h].key == k) return m->slots[h].val;
        if (m->slots[h].key == -1) {
            m->slots[h].key = k;
            m->slots[h].val = new_val;
            return new_val;
        }
        h = (h + 1) % (uint32_t)m->cap;
    }
}

/* ---- mesh construction ---- */

static void normalize(float *v)
{
    float len = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    v[0] /= len; v[1] /= len; v[2] /= len;
}

int mesh_create(Mesh *m, int subdivisions)
{
    /* base icosahedron */
    const float phi = (1.0f + sqrt(5.0f)) / 2.0f;
    float base_v[12][3] = {
        {  -1, phi,   0},{   1, phi,   0},{  -1,-phi,   0},{   1,-phi,   0},
        {   0,  -1, phi},{   0,   1, phi},{   0,  -1,-phi},{   0,   1,-phi},
        { phi,   0,  -1},{ phi,   0,   1},{-phi,   0,  -1},{-phi,   0,   1},
    };
    int base_f[20][3] = {
        { 0,11, 5},{ 0, 5, 1},{ 0, 1, 7},{ 0, 7,10},{ 0,10,11},
        { 1, 5, 9},{ 5,11, 4},{11,10, 2},{10, 7, 6},{ 7, 1, 8},
        { 3, 9, 4},{ 3, 4, 2},{ 3, 2, 6},{ 3, 6, 8},{ 3, 8, 9},
        { 4, 9, 5},{ 2, 4,11},{ 6, 2,10},{ 8, 6, 7},{ 9, 8, 1},
    };

    /* count final sizes */
    int nf = 20;
    for (int s = 0; s < subdivisions; s++) nf *= 4;
    int nv = nf / 2 + 2;   /* Euler: V = F/2 + 2 for closed triangulation */


    /* working buffers for subdivision */
    int v_used = 12;
    int f_used = 20;
    int v_cap  = nv + 1;
    int f_cap  = nf + 1;

    float *verts  = malloc(v_cap * 3 * sizeof(float));
    int   *faces  = malloc(f_cap * 3 * sizeof(int));
    int   *faces2 = malloc(f_cap * 3 * sizeof(int));
    if (!verts || !faces || !faces2) goto fail;

    for (int i = 0; i < 12; i++) {
        memcpy(verts + i*3, base_v[i], 3 * sizeof(float));
        normalize(verts + i*3);
    }
    for (int i = 0; i < 20; i++)
        memcpy(faces + i*3, base_f[i], 3 * sizeof(int));

    /* subdivide */
    for (int s = 0; s < subdivisions; s++) {
        Edge_Map em;
        int est_edges = f_used * 3;  /* upper bound on unique edges */
        emap_init(&em, est_edges * 3);

        int f2_used = 0;

        for (int i = 0; i < f_used; i++) {
            int a = faces[i*3], b = faces[i*3+1], c = faces[i*3+2];
            int mid[3];

            /* find or create midpoint for each edge */
            int pairs[3][2] = {{a,b},{b,c},{c,a}};
            for (int e = 0; e < 3; e++) {
                int p = pairs[e][0], q = pairs[e][1];
                int new_idx = v_used;
                int got = emap_get_or_insert(&em, p, q, new_idx);
                if (got == new_idx) {
                    float *vp = verts + p*3, *vq = verts + q*3;
                    float *vn = verts + new_idx*3;
                    vn[0] = (vp[0]+vq[0])*0.5;
                    vn[1] = (vp[1]+vq[1])*0.5;
                    vn[2] = (vp[2]+vq[2])*0.5;
                    normalize(vn);
                    v_used++;
                }
                mid[e] = got;
            }
            int ab = mid[0], bc = mid[1], ca = mid[2];

            int *f = faces2 + f2_used*3;
            f[0]=a;   f[1]=ab;  f[2]=ca;   f2_used++;
            f = faces2 + f2_used*3;
            f[0]=ab;  f[1]=b;   f[2]=bc;   f2_used++;
            f = faces2 + f2_used*3;
            f[0]=ca;  f[1]=bc;  f[2]=c;    f2_used++;
            f = faces2 + f2_used*3;
            f[0]=ab;  f[1]=bc;  f[2]=ca;   f2_used++;
        }
        emap_free(&em);
        int *tmp = faces; faces = faces2; faces2 = tmp;
        f_used = f2_used;
    }
    free(faces2);

    /* build CSR adjacency from face list.
       Use the edge map to enumerate unique undirected edges,
       then count degrees, then fill. */
    Edge_Map edge_enum;
    int edge_cap = f_used * 2;  /* ~3*f/2 unique edges, leave headroom */
    emap_init(&edge_enum, edge_cap);

    int unique_edges = 0;
    int *edge_a = malloc(f_used * 3 * sizeof(int));
    int *edge_b = malloc(f_used * 3 * sizeof(int));
    if (!edge_a || !edge_b) goto fail;

    for (int i = 0; i < f_used; i++) {
        int tri[3] = { faces[i*3], faces[i*3+1], faces[i*3+2] };
        for (int e = 0; e < 3; e++) {
            int a = tri[e], b = tri[(e+1)%3];
            int idx = emap_get_or_insert(&edge_enum, a, b, unique_edges);
            if (idx == unique_edges) {
                edge_a[unique_edges] = a;
                edge_b[unique_edges] = b;
                unique_edges++;
            }
        }
    }
    emap_free(&edge_enum);
    free(faces);

    /* count degree per vertex */
    int *degree = calloc(v_used, sizeof(int));
    int *adj_off = calloc(v_used + 1, sizeof(int));
    if (!degree || !adj_off) goto fail;

    for (int i = 0; i < unique_edges; i++) {
        degree[edge_a[i]]++;
        degree[edge_b[i]]++;
    }

    adj_off[0] = 0;
    for (int i = 0; i < v_used; i++)
        adj_off[i+1] = adj_off[i] + degree[i];

    int total_adj = adj_off[v_used];
    int *adj_buf = malloc(total_adj * sizeof(int));
    int *cursor  = calloc(v_used, sizeof(int));
    if (!adj_buf || !cursor) goto fail;

    for (int i = 0; i < unique_edges; i++) {
        int a = edge_a[i], b = edge_b[i];
        adj_buf[adj_off[a] + cursor[a]++] = b;
        adj_buf[adj_off[b] + cursor[b]++] = a;
    }

    free(cursor);
    free(degree);
    free(edge_a);
    free(edge_b);

    m->n_verts = v_used;
    m->n_edges = total_adj / 2;
    m->pos     = verts;
    m->adj_off = adj_off;
    m->adj     = adj_buf;
    return 0;

fail:
    free(verts); free(faces); free(faces2);
    return -1;
}

void mesh_free(Mesh *m)
{
    free(m->pos);
    free(m->adj_off);
    free(m->adj);
    memset(m, 0, sizeof(*m));
}
