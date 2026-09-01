import java.util.Random;
import java.util.Iterator;
import java.util.function.Supplier;

public final class Point {
    public final double x, y;
    
    Point(double x, double y) { this.x = x; this.y = y; }
    
    @Override
    public String toString() {
        return String.format("(%.3f, %.3f)", x, y);
    }

    public double distanceTo(Point other) {
        return Math.sqrt(Math.pow(other.x - this.x, 2) + Math.pow(other.y - this.y, 2));
    }

    public static int compareX(Point p1, Point p2) {
        if (p1.x < p2.x) return -1;
        if (p1.x > p2.x) return 1;
        return 0;
    }

    public static int compareY(Point p1, Point p2) {
        if (p1.y < p2.y) return -1;
        if (p1.y > p2.y) return 1;
        return 0;
    }

    public static class Generator implements Iterator<Point>, Supplier<Point> {
        private final Random random;
        private final double xmin, xmax, ymin, ymax;

        public Generator(double min, double max) { this(min, max, min, max); }
        public Generator(double min, double max, long seed) { this(min, max, min, max, seed); }
        
        public Generator(double xmin, double xmax, double ymin, double ymax) {
            this.random = new Random();
            this.xmin = xmin; this.xmax = xmax;
            this.ymin = ymin; this.ymax = ymax;
        }
        
        public Generator(double xmin, double xmax, double ymin, double ymax, long seed) {
            this.random = new Random(seed);
            this.xmin = xmin; this.xmax = xmax;
            this.ymin = ymin; this.ymax = ymax;
        }
        
        public boolean hasNext() { return true; }
        
        public Point next() {
            return new Point(
                xmin + (xmax - xmin) * random.nextDouble(),
                ymin + (ymax - ymin) * random.nextDouble()
            );
        }

        public Point get() { return next(); }
    }
}