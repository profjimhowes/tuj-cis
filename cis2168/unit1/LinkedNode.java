class LinkedNode<T> implements Link<T> extends Box<T> {
    private Link<T> next;

    public LinkedNode(T thing, Link<T> next) {
        super(thing);
        this.next = next;
    }

    @Override
    public Link<T> next() { return next; }
}