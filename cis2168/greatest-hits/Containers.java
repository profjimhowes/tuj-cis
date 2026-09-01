public final class Containers {
    private Containers() {}

    public static <C extends Container, T> C<T> requireNonFull(C<T> c) {
        if (c.isFull()) throw new IllegalStateException("container is full");
        return c;
    }

    public static <C extends Container, T> C<T> requireNonEmpty(C<T> c) {
        if (c.isEmpty()) throw new IllegalStateException("container is empty");
        return c;
    }
}