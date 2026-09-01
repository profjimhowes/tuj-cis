public final class Containers {
    private Containers() {}

    public static <T> Container<T> requireNonFull(Container<T> container) {
        if (container.isFull()) throw new IllegalStateException("container is full");
        return container;
    }

    public static <T> Container<T> requireNonEmpty(Container<T> container) {
        if (container.isEmpty()) throw new IllegalStateException("container is empty");
        return container;
    }
}