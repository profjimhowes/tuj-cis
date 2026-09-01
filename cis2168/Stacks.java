package containers;

import java.util.function.*;

/**
 * Factory for Stack implementations.
 * Each factory method returns a Stack backed by the appropriate structure,
 * delegating push/pop/peek to the correct structural operations.
 */
public final class Stacks {
    private Stacks() {}

    public static <E> Stack<E> ofArray(int initialCapacity) {
        ElasticArray<E> backing = new ElasticArray<>(initialCapacity);
        return new Stack<>() {
            public void    insert(E element)                    { backing.addLast(element); }
            public E       remove()                             { return backing.removeLast(); }
            public <R> R   read(Function<? super E, R> reader)  { return backing.read(backing.size() - 1, reader); }
            public boolean isEmpty()                            { return backing.isEmpty(); }
            public boolean isFull()                             { return backing.isFull(); }
        };
    }

    public static <E> Stack<E> ofLinkedList() {
        LinkedList<E> backing = new LinkedList<>();
        return new Stack<>() {
            public void    insert(E element)                    { backing.addFirst(element); }
            public E       remove()                             { return backing.removeFirst(); }
            public <R> R   read(Function<? super E, R> reader)  { return backing.read(0, reader); }
            public boolean isEmpty()                            { return backing.isEmpty(); }
            public boolean isFull()                             { return backing.isFull(); }
        };
    }
}
