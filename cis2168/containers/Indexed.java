package containers;

import java.util.function.*;

public interface Indexed<E> extends Container<E> {
    int size();

    interface Searchable<E> extends Indexed<E> { int find(E element); }
    interface Readable<E>   extends Indexed<E> {
        <R> R read(int index, Function<? super E, R> reader);
    }

    interface Insertable<E> extends Indexed<E> {
        void insert(int index, E element);
        default void append(E element) { insert(size(), element); }
    }
    interface Removable<E>  extends Indexed<E> { E remove(int index); }
    interface Writable<E>   extends Indexed<E> { E put(int index, E element); }
    interface Swappable<E>  extends Indexed<E> {
        <R> R read(int index, int index2, BiFunction<? super E, ? super E, R> reader);
        void swap(int index, int index2);
    }
}