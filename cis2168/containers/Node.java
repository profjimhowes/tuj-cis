public interface Sequence<E> {
    Optional<E> next();
}

public interface Range<E> extends Sequence<E> {
    Optional<E> prev();
}

public interface BinaryTree<E> {
    Optional<E> parent();
    Optional<E> leftChild();
    Optional<E> rightChild();
}