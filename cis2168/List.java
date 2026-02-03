import java.util.Objects;
import java.util.Arrays;

public interface List<E> {
    int size();
    default boolean isEmpty() { return size() == 0; }
    E get(int i);
    void set(int i, E e);

    // Insertion methods
    default void insertFirst(E e) { insert(0, e); }
    default void insertLast(E e) { insert(size(), e); }
    void insert(int i, E e);

    // Removal methods
    default E removeFirst() { return remove(0); }
    default E removeLast() { return remove(size() - 1); }
    E remove(int i);

    // Search methods
    default boolean contains(E e) { return indexOf(e) >= 0; }
    int indexOf(E e);

    // List.Array: elastic array implementation
    public class Array<E> implements List<E> {
        private static final int DEFAULT_SIZE = 10;
        private Object[] array = new Object[DEFAULT_SIZE];
        private int size = 0;

        @SuppressWarnings("unchecked") private E recover(int i) { return (E) array[i]; }

        private void grow() { grow(size + 1); }
        private void grow(int minimum) {
            array = Arrays.copyOf(array, Integer.max(array.length * 2, minimum));
        }

        /* Public interface: List */

        @Override public int size() { return size; }
        // public boolean isEmpty(): default implementation from List
        @Override public E get(int i) { return recover(Objects.checkIndex(i, size)); }
        @Override public void set(int i, E e) { array[Objects.checkIndex(i, size)] = e; }

        // public void insertFirst(E e): default implementation from List
        // public void insertLast(E e): default implementation from List
        @Override public void insert(int i, E e) {
            Objects.checkIndex(i, size + 1);
            if (size >= array.length) grow();
            System.arraycopy(array, i, array, i + 1, size++ - i);
            array[i] = e;
        }

        // public E removeFirst(): default implementation from List
        // public E removeLast(): default implementation from List
        @Override public E remove(int i) {
            E removed = recover(Objects.checkIndex(i, size));
            System.arraycopy(array, i + 1, array, i, --size - i);
            array[size] = null;
            return removed;
        }

        // public boolean contains(E e): default implementation from List
        @Override public int indexOf(E e) {
            for (int i = 0; i < size; i++)
                if (array[i].equals(e)) return i;
            return -1;
        }
    }

    // List.Linked: linked implementation
    public class Linked<E> implements List<E> {
        private Node<E> head = null;
        private int size = 0;

        private static class Node<E> {
            E element;
            Node<E> next;

            Node(E e, Node<E> next) {
                this.element = e;
                this.next = next;
            }
        }

        private Node<E> walk(int steps) {
            Node<E> node = head;
            while (steps-- > 0) node = node.next;
            return node;
        }

        /* Public interface: List */

        @Override public int size() { return size; }
        // public boolean isEmpty(): default implementation from List
        @Override public E get(int i) { return walk(Objects.checkIndex(i, size)).element; }
        @Override public void set(int i, E e) { walk(Objects.checkIndex(i, size)).element = e; }

        // public void insertFirst(E e): default implementation from List
        // public void insertLast(E e): default implementation from List
        @Override public void insert(int i, E e) {
            if (Objects.checkIndex(i, size + 1) > 0) {
                Node<E> node = walk(i - 1);
                node.next = new Node<>(e, node.next);
            } else head = new Node<>(e, head);
            size++;
        }

        // public E removeFirst(): default implementation from List
        // public E removeLast(): default implementation from List
        @Override public E remove(int i) {
            Node<E> removed = head;
            if (Objects.checkIndex(i, size) > 0) {
                Node<E> node = walk(i - 1);
                removed = node.next;
                node.next = removed.next;
            } else head = removed.next;
            size--;
            return removed.element;
        }

        // public boolean contains(E e): default implementation from List
        @Override public int indexOf(E e) {
            Node<E> node = head;
            for (int i = 0; node != null; node = node.next, i++)
                if (node.element.equals(e)) return i;
            return -1;
        }
    }
}
