import java.util.Arrays;
import java.util.Comparator;
import java.util.Random;

public final class ClosestPair {
    private static final int CUTOFF = 5;

    public static record Point(double x, double y) {
        public double distanceTo(Point that) {
            double dx = that.x - this.x, dy = that.y - this.y;
            return Math.sqrt(dx * dx + dy * dy);
        }
    }

    public static record Pair(Point p, Point q, double dist) {}

    private Point[] points, buffer;
    private Comparator<Point> cmpX, cmpY;

    private int compareX(Point[] p, int i, int j) { return cmpX.compare(p[i], p[j]); }
    private int compareY(Point[] p, int i, int j) { return cmpY.compare(p[i], p[j]); }
    private static void swap(Point[] p, int i, int j) { Point t = p[i]; p[i] = p[j]; p[j] = t; }

    public Pair findClosest(Point[] points) {
        return findClosest(points,
            Comparator.comparingDouble((p) -> p.x),
            Comparator.comparingDouble((p) -> p.y)
        );
    }

    public Pair findClosest(Point[] points, Comparator<Point> cmpX, Comparator<Point> cmpY) {
        this.points = Arrays.copyOf(points, points.length);
        this.buffer = new Point[points.length];
        this.cmpX = cmpX; this.cmpY = cmpY;
        return divideAndConquer(0, points.length - 1);
    }

    private Pair divideAndConquer(int start, int end) {
        // Base case brute force
        if (end - start < CUTOFF) return bruteForce(start, end);

        // Partition by X-coord, recurse, identify closest pair
        Pair closest;
        int mid = partitionX(start, end);
        double xmid = points[mid].x;
        if (mid > start) {
            closest = divideAndConquer(start, mid - 1);
            if (mid < end) {
                Pair p = divideAndConquer(mid, end);
                closest = p.dist < closest.dist ? p : closest;
        }} else closest = divideAndConquer(++mid, end);

        // Merge by Y-coord, separating out boundary points
        int bound = boundaryMergeY(xmid - closest.dist, xmid + closest.dist, start, mid, end);
        
        // Scan boundary for closest pair
        for (int i = start; i < bound; i++)
            for (int j = i + 1; j < bound && buffer[j].y - buffer[i].y < closest.dist; j++) {
                double d = buffer[i].distanceTo(buffer[j]);
                if (d < closest.dist) closest = new Pair(buffer[i], buffer[j], d);
            }

        // Merge boundary back into rest of points
        mergeY(start, bound, end);

        return closest;
    }

    private int partitionX(int start, int end) {
        while (compareX(points, end--, start) > 0);
        int pivot = ++end;
        while (start < end) {
            swap(points, start, end);
            while (compareX(points, ++start, pivot) < 0);
            while (compareX(points, --end, pivot) > 0);
        }
        swap(points, start, pivot);
        return start;
    }

    private int boundaryMergeY(double lower, double upper, int start, int mid, int end) {
        for (int l = start, r = mid, tail = end + 1; start < tail;) {
            Point p;
            if (r > end) p = points[l++];
            else if (l >= mid || compareY(points, l, r) > 0) p = points[r++];
            else p = points[l++];
            buffer[lower <= p.x && p.x <= upper ? start++ : --tail] = p;
        }
        return start;
    }

    private void mergeY(int start, int mid, int end) {
        for (int i = start, l = start, r = end; i <= end; i++)
            if (r < mid) points[i] = buffer[l++];
            else if (l >= mid || compareY(buffer, l, r) > 0) points[i] = buffer[r--];
            else points[i] = buffer[l++];
    }

    private Pair bruteForce(int start, int end) {
        Point p = points[start], q = points[end];
        double dmin = Double.MAX_VALUE;
        for (int i = start + 1; i <= end; i++) {
            int k = i; Point pi = points[i];
            for (int j = i - 1; j >= start; j--) {
                if (compareY(points, j, i) > 0) k--;
                double d = points[i].distanceTo(points[j]);
                if (d < dmin) { p = points[i]; q = points[j]; dmin = d; }
            }
            System.arraycopy(points, k, points, k + 1, i - k);
            points[k] = pi;
        }
        return new Pair(p, q, dmin);
    }

    public static void main(String[] args) {
        int size = 0;
        try { size = Integer.parseInt(args[0]); }
        catch (Exception e) { size = 0; }
        size = size > 0 ? size : 1000;
        double range = Math.sqrt(size) * 2;

        Point[] points = new Point[size];
        Random rand = new Random();
        for (int i = 0; i < size; i++)
            points[i] = new Point(
                2 * range * rand.nextDouble() - range,
                2 * range * rand.nextDouble() - range
            );

        long start = System.nanoTime();
        Pair closest = new ClosestPair().findClosest(points);
        double elapsed = (double)(System.nanoTime() - start) / 1e6;
        System.out.printf("Divide and conquer O(n log n): %.3f ms%n", elapsed);
        System.out.printf(
            "(%.3f, %.3f) and (%.3f, %.3f), d = %.6f\n",
            closest.p.x, closest.p.y, closest.q.x, closest.q.y, closest.dist
        );
    }
}