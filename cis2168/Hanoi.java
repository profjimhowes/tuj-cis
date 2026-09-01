public class Hanoi {
    static class Stack<E extends Comparable<? super E>> {
        Deque<E> stack;

        Stack(int size) { stack = new ArrayDeque<>(size); }

        void push(E element) {
            if (!stack.isEmpty() && stack.peek().compare(element) < 0)
                throw new IllegalStateException();
            stack.push(element);
        }

        E pop() { return stack.pop(); }
    }

    public static void solve(int n) {
        Stack<Integer> left = new Stack<>(n);
        Stack<Integer> mid = new Stack<>(n);
        Stack<Integer> right = new Stack<>(n);
        while (n > 0) left.push(n--);
        move(n, left, right, mid);
    }

    private static void move(int discs, Stack<Integer> from, Stack<Integer> to, Stack<Integer> using) {
        if (discs > 1) move(discs - 1, from, using, to);
        to.push(from.pop());
        if (discs > 1) move(discs - 1, using, to, from);
    }
}