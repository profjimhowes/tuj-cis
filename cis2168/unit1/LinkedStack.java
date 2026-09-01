public final class LinkedStack<T> implements Stack<T> {
    private Link<T> head;

    public LinkedStack() { head = null; }

    @Override
    public boolean isEmpty() { return Objects.isNull(head); }

    @Override
    public boolean isFull() { return false; }

    @Override
    public void insert(T thing) { head = new LinkedNode(thing, head); }
    
    @Override
    public T remove() {
        Containers.requireNonEmpty(this);
        T thing = head.remove();
        head = head.next();
        return thing;
    }
    
    @Override
    public void peek(Consumer<T> peeker) {
        if (!isEmpty()) head.peek(peeker);
        else peeker.accept(head);
    }

    @Override
    public boolean test(Predicate<T> tester) {
        if (!isEmpty()) return head.test(tester);
        else return tester.test(null);
    }

    @Override
    public <R> R read(Function<T, R> reader) {
        if (!isEmpty()) return head.read(reader);
        else return reader.apply(null);
    }
}