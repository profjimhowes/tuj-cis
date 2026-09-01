public final class Recursive {
    private Recursive() {}

    public static long factorial(int n) { return factorial(n, 1); }

    private static long factorial(int n, long a) {
        if (n <= 1) return a;
        return factorial(n - 1, n * a);
    }

    public static long rencontres(int n) { return rencontres(n, 1, -1); }

    private static long rencontres(int n, long a, int p) {
        if (n <= 0) return a;
        return rencontres(n - 1, n * a + p, -p);
    }

    public static long fibonacci(int n) {
        if (n <= 1) return n;
        return fibonacci(n - 1, 1, 0);
    }

    private static long fibonacci(int n, long c, long p) {
        if (n == 0) return c;
        return fibonacci(n - 1, c + p, c);
    }

    public static long lucas(int n) {
        if (n <= 0) return 2;
        return lucas(n - 1, 1, 2);
    }

    private static long lucas(int n, long c, long p) {
        if (n == 0) return c;
        return lucas(n - 1, c + p, c);
    }

    public static long pell(int n) {
        if (n <= 1) return n;
        return pell(n - 1, 1, 0);
    }

    private static long pell(int n, long c, long p) {
        if (n == 0) return c;
        return pell(n - 1, 2 * c + p, c);
    }

    public static long jacobsthal(int n) {
        if (n <= 1) return n;
        return jacobsthal(n - 1, 1, 0);
    }

    private static long jacobsthal(int n, long c, long p) {
        if (n == 0) return c;
        return jacobsthal(n - 1, c + 2 * p, c);
    }

    public static long tribonacci(int n) {
        if (n <= 1) return 0;
        return tribonacci(n - 2, 1, 0, 0);
    }

    private static long tribonacci(int n, long c, long p, long pp) {
        if (n == 0) return c;
        return tribonacci(n - 1, c + p + pp, c, p);
    }

    public static long padovan(int n) {
        if (n <= 2) return n > 0 ? 0 : 1;
        return padovan(n - 2, 0, 0, 1);
    }

    private static long padovan(int n, long c, long p, long pp) {
        if (n == 0) return c;
        return padovan(n - 1, p + pp, c, p);
    }

    public static long perrin(int n) {
        if (n <= 1) return n > 0 ? 0 : 3;
        return perrin(n - 2, 2, 0, 3);
    }

    private static long perrin(int n, long c, long p, long pp) {
        if (n == 0) return c;
        return perrin(n - 1, p + pp, c, p);
    }

    public static long leftFactorial(int n) {
        if (n <= 0) return 0;
        return leftFactorial(n, 1, 1, 1);
    }

    private static long leftFactorial(int n, int k, long s, long p) {
        if (k == n) return s;
        return leftFactorial(n, k + 1, s + n * p, n * p);
    }

    public static int gcd(int a, int b) {
        if (b == 0) return a;
        return gcd(b, a % b);
    }
}