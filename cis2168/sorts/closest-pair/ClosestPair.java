import java.util.Arrays;
import java.util.Random;

public final class ClosestPair {
    static record Point(double x, double y) {}
    static record Pair(Point p1, Point p2, double d) {}

    public static final class BruteForce {
        public static Pair findClosest(Point[] points) {
            return bruteForce(points, 0, points.length - 1);
        }

        private static Pair bruteForce(Point[] points, int start, int end) {
            Pair closest = new Pair(null, null, Double.MAX_VALUE);
            for (int i = start; i < end; i++)
                for (int j = i + 1; j <= end; j++) {
                    double d = distance(points[i], points[j]);
                    if (d < closest.d) closest = new Pair(points[i], points[j], d);
                }
            return closest;
        }

        private static double distance(Point p1, Point p2) {
            return Math.sqrt(Math.pow(p2.x - p1.x, 2) + Math.pow(p2.y - p1.y, 2));
        }
    }

    public static void main(String[] args) {
        int size = 0;
        try { size = Integer.parseInt(args[0]); }
        catch (Exception e) { size = 0; }
        size = size > 0 ? size : 1000;
        double range = Math.sqrt(size) / 2;

        Point[] points = new Point[size];
        Random rand = new Random();
        for (int i = 0; i < size; i++)
            points[i] = new Point(
                2 * range * rand.nextDouble() - range,
                2 * range * rand.nextDouble() - range
            );

        long start = System.nanoTime();
        Pair closest = BruteForce.findClosest(points);
        double elapsed = (double)(System.nanoTime() - start) / 1e6;
        System.out.printf("Brute force O(n^2): %.3f ms%n", elapsed);
        System.out.printf(
            "(%.3f, %.3f) and (%.3f, %.3f), d = %.6f\n",
            closest.p1.x, closest.p1.y, closest.p2.x, closest.p2.y, closest.d
        );
    }
}