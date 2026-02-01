package nodes;

public interface Visitor<E, R> {
    R visit(Visitable<? extends E> node);
}