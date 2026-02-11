package containers;

import java.util.Comparator;
import java.util.function.*;

public interface List<E> extends Container.Sized<E> {
    interface Readable<E> extends List<E> { <R> R read(int index, Function<? super E, R> reader); }
    interface Writable<E> extends List<E> { E put(int index, E element); }

    interface Insertable<E> extends List<E> { void insert(int index, E element); }
    interface Removable<E>  extends List<E> { E remove(int index); }

    interface Sortable<E> extends List<E> {
        int compare(int index, int index2, Comparator<E> comparator);
        void swap(int index, int index2);
    }
}