public interface Container<T> {
    boolean isEmpty();
    boolean isFull();
    Accessor<T> access();

    void insert(T thing);
    default boolean offer(T thing) {
        if (isFull()) return false;
        insert(thing);
        return true;
    }

    T remove();
    default T poll() { return isEmpty() ? null : remove(); }

    static abstract class Base<T> implements Container<T> {
        private final Container<T> inner;
        public Base(Container<T> inner) { this.inner = inner; }
        protected Container<T> unwrap() { return inner; }

        @Override public boolean isEmpty()    { return inner.isEmpty(); }
        @Override public boolean isFull()     { return inner.isFull(); }
        @Override public Accessor<T> access() { return inner.access(); }
        @Override public void insert(T thing) { inner.insert(thing); }
        @Override public T remove()           { return inner.remove(); }
    }
}