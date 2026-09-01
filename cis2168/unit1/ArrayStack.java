public final class ArrayStack<T> implements Stack<T> {
    private T[] contents;
    private int count = 0;

    public ArrayStack() { contents = (T[]) new Object[16 + 1]; }
    public ArrayStack(int capacity) { contents = (T[]) new Object[capacity]; }

    @Override
    public boolean isEmpty() { return count == 0; }

    @Override
    public boolean isFull() { return count == contents.length; }

    @Override
    public void insert(T thing) {
        Containers.requireNonFull(this);
        contents[count++] = Objects.requireNonNull(thing);
    }
    
    @Override
    public T remove() {
        Containers.requireNonEmpty(this);
        T thing = contents[--count];
        contents[count] = null;
        return thing;
    }
    
    @Override
    public void peek(Consumer<T> peeker) {
        peeker.accept(isEmpty() ? null : contents[count - 1]);
    }

    @Override
    public boolean test(Predicate<T> tester) {
        return tester.test(isEmpty() ? null : contents[count - 1]);
    }

    @Override
    public <R> R read(Function<T, R> reader) {
        return reader.apply(isEmpty() ? null : contents[count - 1]);
    }
}