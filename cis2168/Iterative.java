public final class Iterative {
    private Iterative() {}

    public static long factorial(int n) {
        long a = 1;
        while (n > 1) a *= n--;
        return a;
    }

    public static long rencontres(int n) {
        long a = 1;
        int p = -1;
        while (n-- > 0) {
            a = a * n + p;
            p = -p;
        }
        return a;
    }

    public static long fibonacci(int n) {
        if (n < 1) return 0;
        long p = 0, c = 1;
        while (n-- > 1) {
            long t = c + p;
            p = c; c = t;
        }
        return c;
    }

    public static long lucas(int n) {
        if (n < 1) return 2;
        long p = 2, c = 1;
        while (n-- > 1) {
            long t = c + p;
            p = c; c = t;
        }
        return c;
    }

    public static long pell(int n) {
        if (n < 1) return 0;
        long p = 0, c = 1;
        while (n-- > 1) {
            long t = 2 * c + p;
            p = c; c = t;
        }
        return c;
    }

    public static long jacobsthal(int n) {
        if (n < 1) return 0;
        long p = 0, c = 1;
        while (n-- > 1) {
            long t = c + 2 * p;
            p = c; c = t;
        }
        return c;
    }

    public static long tribonacci(int n) {
        if (n <= 1) return 0;
        long c = 1, p = 0, pp = 0;
        while (n-- > 2) {
            long t = c + p + pp;
            p = c; c = t;
        }
        return c;
    }

    public static long padovan(int n) {
        if (n <= 2) return n > 0 ? 0 : 1;
        long c = 0, p = 0, pp = 1;
        while (n-- > 2) {
            long t = p + pp;
            p = c; c = t;
        }
        return c;
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
        while (b != 0) {
            int t = b;
            b = a % b;
            a = t;
        }
        return a;
    }
}