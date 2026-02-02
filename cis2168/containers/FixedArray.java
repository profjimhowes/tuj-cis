package containers;

import java.util.Objects;
import java.util.function.*;

public sealed class FixedArray<E>
implements Indexed.Readable<E>, Indexed.Writable<E>, Indexed.Swappable<E>
permits ElasticArray {
    Object[] contents;

    FixedArray(Object[] array) { contents = array; }

    @SafeVarargs static <E> FixedArray<E> of(E... elements) {
        for (E e : elements) Objects.requireNonNull(e, "null elements not permitted");
        return new FixedArray<>(elements);
    }

    @Override public boolean isEmpty() { return size() == 0; }
    @Override public boolean isFull() { return true; }
    @Override public int size() { return contents.length; }

    @Override public <R> R read(int index, Function<? super E, R> reader) {
        return reader.apply(restore(index));
    }

    @Override public <R> R read(int index, int index2, BiFunction<? super E, ? super E, R> reader) {
        return reader.apply(restore(index), restore(index2));
    }

    @Override public E put(int index, E element) {
        Objects.requireNonNull(element, "null elements not permitted");
        E temp = restore(index);
        contents[index] = element;
        return temp;
    }

    @Override public void swap(int index, int index2) {
        Object temp = contents[index2];
        contents[index2] = contents[index];
        contents[index] = temp;
    }

    @SuppressWarnings("unchecked") E restore(int index) { return (E) contents[index]; }
}
