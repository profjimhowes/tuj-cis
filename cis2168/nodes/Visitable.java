package nodes;

public interface Visitable<E> {
    <R> R accept(Visitor<? super E, R> visitor);
}