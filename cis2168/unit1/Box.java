import java.util.Objects;
import java.util.function.Function;

public class Box<T>
implements Container<T> {
    private T contents;

    public Box() { contents = null; }
    public Box(T thing) { contents = Objects.requireNonNull(thing); }

    @Override
    public boolean isEmpty() { return Objects.isNull(contents); }

    @Override
    public boolean isFull() { return Objects.nonNull(contents); }

    @Override
    public void insert(T thing) {
        Containers.requireNonFull(this);
        contents = Objects.requireNonNull(thing);
    }

    @Override
    public T remove() {
        Containers.requireNonEmpty(this);
        T thing = contents;
        contents = null;
        return thing;
    }

    @Override
    public void peek(Consumer<T> peeker) { peeker.accept(contents); }

    @Override
    public boolean test(Predicate<T> tester) { return tester.test(contents); }

    @Override
    public <R> R read(Function<T, R> reader) { return reader.apply(contents); }

    public static void main(String[] args) {
        Box<String> stringBox = new Box<>("Hello world!");
        stringBox.peek(
            (String contents) -> {
                System.out.println(contents);
            }
        );
    }
}