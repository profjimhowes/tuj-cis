import java.util.Objects;
import java.util.function.*;

public final class Box<T> implements Container<T> {
    private T contents = null;

    public Box() {}
    public Box(T thing) { contents = Objects.requireNonNull(thing); }

    @Override public boolean isEmpty() { return Objects.isNull(contents); }
    @Override public boolean isFull() { return Objects.nonNull(contents); }

    @Override public void insert(T thing) {
        Containers.requireNonFull(this);
        contents = Objects.requireNonNull(thing);
    }

    @Override public T remove() {
        Containers.requireNonEmpty(this);
        T thing = contents;
        contents = null;
        return thing;
    }

    @Override public Accessor<T> access() { return new Accessor<T>() {
        @Override public void accept(Consumer<T> c) { c.accept(contents); }
        @Override public boolean test(Predicate<T> p) { return p.test(contents); }
        @Override public int apply(ToIntFunction<T> f) { return f.applyAsInt(contents); }
        @Override public long apply(ToLongFunction<T> f) { return f.applyAsLong(contents); }
        @Override public double apply(ToDoubleFunction<T> f) { return f.applyAsDouble(contents); }
        @Override public <R> R apply(Function<T, R> f) { return f.apply(contents); }
    }; }
}