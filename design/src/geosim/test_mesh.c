#include "mesh.h"
#include <stdio.h>
#include <math.h>

static int check(int cond, const char *msg)
{
    if (!cond) { fprintf(stderr, "FAIL: %s\n", msg); return 1; }
    return 0;
}

int main(void)
{
    int fails = 0;

    for (int s = 0; s <= 6; s++) {
        Mesh m = {0};
        if (mesh_create(&m, s)) { fails++; continue; }

        int expect_v = 10 * (1 << (2*s)) + 2;
        fails += check(m.n_verts == expect_v,
            "vertex count mismatch");

        /* check all on unit sphere */
        double max_err = 0;
        for (int i = 0; i < m.n_verts; i++) {
            double *p = m.pos + i*3;
            double r = sqrt(p[0]*p[0]+p[1]*p[1]+p[2]*p[2]);
            double e = fabs(r - 1.0);
            if (e > max_err) max_err = e;
        }
        fails += check(max_err < 1e-12, "vertex not on unit sphere");

        /* check degrees: exactly 12 verts with degree 5, rest degree 6 */
        int n5 = 0, n6 = 0, bad = 0;
        for (int i = 0; i < m.n_verts; i++) {
            int deg = m.adj_off[i+1] - m.adj_off[i];
            if (deg == 5) n5++;
            else if (deg == 6) n6++;
            else bad++;
        }
        fails += check(n5 == 12, "expected 12 pentagonal verts");
        fails += check(bad == 0, "unexpected vertex degree");
        fails += check(n5 + n6 == m.n_verts, "degree count mismatch");

        printf("s=%d  V=%d  E=%d  deg5=%d  deg6=%d  max_r_err=%.2e  %s\n",
               s, m.n_verts, m.n_edges, n5, n6, max_err,
               (fails == 0) ? "OK" : "FAIL");

        mesh_free(&m);
    }

    return fails ? 1 : 0;
}
