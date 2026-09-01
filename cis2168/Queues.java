package containers;

import java.util.function.*;

/**
 * Factory for Queue implementations.
 * Each factory method returns a Queue backed by the appropriate structure,
 * delegating enqueue/dequeue/peek to the correct structural operations.
 */
public final class Queues {
    private Queues() {}

    public static <E> Queue<E> ofArray(int initialCapacity) {
        ElasticArray<E> backing = new ElasticArray<>(initialCapacity);
        return new Queue<>() {
            public void    insert(E element)                    { backing.addLast(element); }
            public E       remove()                             { return backing.removeFirst(); }
            public <R> R   read(Function<? super E, R> reader)  { return reader.apply(backing.getFirst()); }
            public boolean isEmpty()                            { return backing.isEmpty(); }
            public boolean isFull()                             { return false; }
        };
    }

    public static <E> Queue<E> ofLinkedList() {
        LinkedList<E> backing = new LinkedList<>();
        return new Queue<>() {
            public void    insert(E element)                    { backing.addLast(element); }
            public E       remove()                             { return backing.removeFirst(); }
            public <R> R   read(Function<? super E, R> reader)  { return reader.apply(backing.getFirst()); }
            public boolean isEmpty()                            { return backing.isEmpty(); }
            public boolean isFull()                             { return false; }
        };
    }
}
