public interface Stack<T> extends Container<T> {
    default void push(T thing) { insert(thing); }
    default T pop() { return remove(); }

    static <T> Stack<T> over(Sequential<T> seq) { return new Stack<>() {
        @Override public boolean isEmpty()    { return seq.isEmpty(); }
        @Override public boolean isFull()     { return false; }
        @Override public Accessor<T> access() { return seq.head(); }
        @Override public void insert(T thing) { seq.insertHead(thing); }
        @Override public T remove()           { return seq.removeHead(); }
    }; }
}
