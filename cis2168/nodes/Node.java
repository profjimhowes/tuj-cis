package nodes;

import java.util.Objects;

public abstract class Node<E> extends Visitable<E>, Readable<E> {
    final E element;

    public Node(E element) { this.element = Objects.requireNonNull(element); }

    @Override public <R> R accept(Visitor<? super E, R> visitor) { return visitor.visit(this); }
    @Override public <R> R read(Function<? super E, R> reader) { return reader.apply(element); }

    // Link: unidirectional list node
    public static interface Linked<E> { Node<E> next(); }

    public abstract class Link<E> extends Node<E> implements Linked<E> {
        Node<E> next;

        Link(E element) { this(element, Terminal.terminal()); }
        Link(E element, Node<E> next) {
            super(element);
            this.next = Objects.requireNonNull(next);
        }

        @Override public Node<E> next() { return next; }
    }

    // BiLink: bidirectional list node
    public static interface BiLinked<E> { Node<E> prev(); }

    public abstract class BiLink<E> extends Link<E> implements BiLinked<E> {
        Node<E> prev;

        BiLink(E element) { this(element, Terminal.terminal(), Terminal.terminal()); }
        BiLink(E element, Node<E> next, Node<E> prev) {
            super(element, next);
            this.prev = Objects.requireNonNull(prev);
        }

        @Override public Node<E> prev() { return prev; }
    }

    // Forked: binary tree node
    public static interface Forked<E> {
        Node<E> left();
        Node<E> right();
    }

    public abstract class Fork<E> extends Node<E> implements Forked<E> {
        Node<E> left, right;

        Fork(E element) { this(element, Terminal.terminal(), Terminal.terminal()); }
        Fork(E element, Node<E> left, Node<E> right) {
            super(element);
            this.left = Objects.requireNonNull(left);
            this.right = Objects.requireNonNull(right);
        }

        @Override public Node<E> left() { return left; }
        @Override public Node<E> right() { return right; }
    }
}