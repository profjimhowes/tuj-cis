import java.util.Arrays;
import java.util.Objects;
import java.util.function.*;

public final class ElasticArray<T> {
    private static final int DEFAULT_BOUND = 16 + 1;
    private static final GrowthStrategy DEFAULT_STRATEGY = GrowthStrategy::doubling;

    private T[] contents;
    private GrowthStrategy strategy;
    private int count = 0;

    // Constructors

    public ElasticArray() { this(DEFAULT_BOUND); }
    public ElasticArray(int bound) { this(bound, DEFAULT_STRATEGY); }

    @SuppressWarnings("unchecked")
    public ElasticArray(int bound, GrowthStrategy strategy) {
        if (bound < 1) throw new IllegalArgumentException("initial bound must be positive");
        contents = (T[]) new Object[bound];
        this.strategy = Objects.requireNonNull(strategy);
    }

    // Size and bounds methods

    private int nextBound(int required) { return strategy.nextBound(contents.length, required); }

    public int size()        { return count; }
    public boolean isEmpty() { return count == 0; }
    public boolean isFull()  { return count == contents.length && count >= nextBound(count + 1); }

    public void expand(int required) {
        int newBound = nextBound(required);
        if (newBound > contents.length) contents = Arrays.copyOf(contents, newBound);
        else throw new IllegalStateException("unable to expand");
    }

    public void shrink(int minimum) {
        int newBound = Math.max(count, minimum);
        if (newBound < contents.length) contents = Arrays.copyOf(contents, newBound);
    }

    // Access methods

    private T get(int index) { return contents[Objects.checkIndex(index, count)]; }
    public Accessor<T> access(int index) { return new Accessor<T>() {
        @Override public void accept(Consumer<T> c)          { c.accept(get(index)); }
        @Override public boolean test(Predicate<T> p)        { return p.test(get(index)); }
        @Override public int apply(ToIntFunction<T> f)       { return f.applyAsInt(get(index)); }
        @Override public long apply(ToLongFunction<T> f)     { return f.applyAsLong(get(index)); }
        @Override public double apply(ToDoubleFunction<T> f) { return f.applyAsDouble(get(index)); }
        @Override public <R> R apply(Function<T, R> f)       { return f.apply(get(index)); }
    }; }

    // Insertion and removal methods

    public void insertFirst(T thing) { uncheckedInsert(0, Objects.requireNonNull(thing)); }
    public void insertLast(T thing) { uncheckedInsert(count, Objects.requireNonNull(thing)); }
    public void insert(int index, T thing) {
        uncheckedInsert(Objects.checkIndex(index, count + 1), Objects.requireNonNull(thing));
    }

    private void uncheckedInsert(int index, T thing) {
        if (count == contents.length) expand(count + 1);
        System.arraycopy(contents, index, contents, index + 1, count++ - index);
        contents[index] = thing;
    }

    public T removeFirst() { return remove(0); }
    public T removeLast() { return remove(count - 1); }
    public T remove(int index) { 
        T thing = get(index);
        System.arraycopy(contents, index + 1, contents, index, count - index - 1);
        contents[--count] = null;
        return thing;
    }

    // Growth strategy determines if, and by how much, the array will expand when full
    @FunctionalInterface public static interface GrowthStrategy {
        int nextBound(int current, int required);

        // Built-in strategies
        static int doubling(int current, int required) { return Math.max(current <<< 1, required); }
        static int golden(int current, int required) { return Math.max(current + current >>> 1, required); }
        static int exact(int current, int required) { return Math.max(current, required); }
        static int none(int current, int required) { return current; }

        // Simple parameterized strategies
        static GrowthStrategy linear(int step) {
            return (current, required) -> Math.max(current + step, required);
        }

        static GrowthStrategy geometric(double factor) {
            return (current, required) -> Math.max((int)(current * factor), required);
        }

        // Combinators
        static GrowthStrategy withMinimum(int min, GrowthStrategy strategy) {
            return (current, required) -> Math.max(min, strategy.nextBound(current, required));
        }

        static GrowthStrategy withMaximum(int max, GrowthStrategy strategy) {
            return (current, required) -> Math.min(max, strategy.nextBound(current, required));
        }
    }
}