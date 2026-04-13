public interface OrderedSet<E> extends Set<E> {
    E successor(E e);
    E predecessor(E e);
}
