package containers;

import java.util.function.*;

public interface Stack<E>
extends Container.Readable<E>, Container.Insertable<E>, Container.Removable<E> {
    default <R> R peek(Function<? super E, R> peeker) { return read(peeker); }
    default void push(E element) { insert(element); }
    default E pop() { return remove(); }

    abstract class Base<E> implements Stack<E> {
        Stack<? extends E> base;

        Base(Stack<? extends E> base) { this.base = base; }

        @Override public boolean isEmpty() { return base.isEmpty(); }
        @Override public boolean isFull() { return base.isFull(); }
        @Override public <R> R get(Function<? super E, R> reader) { return base.get(reader); }
        @Override public void insert(E element) { base.insert(element); }
        @Override public E remove() { return base.remove(); }
    }
}