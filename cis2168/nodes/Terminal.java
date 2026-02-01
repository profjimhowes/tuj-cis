package nodes;

// Terminal: type-safe null visitable
public final class Terminal<E> implements Visitable<E> {
    static final Terminal THIS = new Terminal();

    @SuppressWarnings("unchecked")
    static <E> Terminal<E> terminal() { return (Terminal<E>)Terminal.THIS; }

    private Terminal() {} // singleton

    @Override public <R> R accept(Visitor<? super E, R> visitor) {
        return visitor.visit(this);
    }
}