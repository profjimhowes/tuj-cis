package containers;

import java.util.Objects;
import java.util.function.*;

public sealed class FixedArray<E>
implements List.Readable<E>, List.Writable<E>, List.Sortable<E>
permits ElasticArray {
    Object[] contents;

    FixedArray(Object[] array) { contents = array; }

    @Override public boolean isEmpty() { return size() == 0; }
    @Override public boolean isFull() { return true; }
    @Override public int size() { return contents.length; }

    public <R> R readFirst(Function<? super E, R> reader) { return read(0, reader); }
    public <R> R readLast(Function<? super E, R> reader) { return read(size() - 1, reader); }
    @Override public <R> R read(int index, Function<? super E, R> reader) {
        return reader.apply(restore(index));
    }

    public E writeFirst(Function<? super E, ? extends E> writer) { return write(0, writer); }
    public E writeLast(Function<? super E, ? extends E> writer) { return write(size() - 1, writer); }
    @Override public E write(int index, Function<? super E, ? extends E> writer) {
        E temp = restore(index);
        contents[index] = requireNonNull(writer.apply(temp));
        return temp;
    }

    @Override public void swap(int index, int index2) {
        Object temp = contents[index2];
        contents[index2] = contents[index];
        contents[index] = temp;
    }

    @SuppressWarnings("unchecked") E restore(int index) { return (E) contents[index]; }
}
