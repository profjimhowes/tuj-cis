public final class StackQueue<T> implements Queue<T> {
    private Stack<T> inbox, outbox;

    public StackQueue(Stack<T> inbox, Stack<T> outbox) {
        this.inbox = inbox;
        this.outbox = outbox;
    }

    @Override
    public boolean isEmpty() { return inbox.isEmpty() && outbox.isEmpty(); }

    @Override
    public boolean isFull() { return inbox.isFull() && !outbox.isEmpty(); }

    @Override
    public void insert(T thing) {
        if (isFull()) throw new IllegalStateException("container is full");
        if (inbox.isFull()) flipInbox();
        inbox.push(thing);
    }

    @Override
    public T remove() {
        if (isEmpty()) throw new IllegalStateException("container is empty");
        if (outbox.isEmpty()) flipInbox();
        return outbox.pop();
    }

    @Override
    public void peek(Consumer<T> peeker) {
        if (isEmpty()) peeker.accept(null);
        if (outbox.isEmpty()) flipInbox();
        outbox.peek(peeker);
    }

    @Override
    public boolean test(Predicate<T> tester) {
        if (isEmpty()) return tester.test(null);
        if (outbox.isEmpty()) flipInbox();
        return outbox.test(tester);
    }

    @Override
    public <R> R read(Function<T, R> reader) {
        if (isEmpty()) return reader.apply(null);
        if (outbox.isEmpty()) flipInbox();
        return outbox.read(reader);
    }

    private void flipInbox() {
        while (!inbox.isEmpty()) outbox.push(inbox.pop());
    }
}