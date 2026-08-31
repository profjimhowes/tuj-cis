public interface Queue<T> extends Container<T> {
    default void enqueue(T thing) { insert(thing); }
    default T dequeue() { return remove(); }

    static <T> Queue<T> over(Sequential<T> seq) { return new Queue<>() {
        @Override public boolean isEmpty()    { return seq.isEmpty(); }
        @Override public boolean isFull()     { return false; }
        @Override public Accessor<T> access() { return seq.head(); }
        @Override public void insert(T thing) { seq.insertTail(thing); }
        @Override public T remove()           { return seq.removeHead(); }
    }; }
}
