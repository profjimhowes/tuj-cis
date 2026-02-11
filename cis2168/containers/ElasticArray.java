package containers;

import java.util.Objects;
import java.util.Arrays;
import java.util.function.*;

public sealed class ElasticArray<E> extends FixedArray<E>
implements Container.Elastic<E>, List.Insertable<E>, List.Removable<E>
permits RingArray {
    private static final Object[] EMPTY = {};

    int size = 0;
    final GrowthStrategy strategy;

    private ElasticArray(Object[] array, int size, GrowthStrategy strategy) {
        super(Objects.requireNonNullElse(array, EMPTY));
        this.size = size; this.strategy = strategy;
    }

    @Override public boolean isFull() {
        return size >= contents.length && strategy.nextBound(size, size + 1) <= size;
    }

    @Override public int size() { return size; }

    @Override public <R> R read(int index, Function<? super E, R> reader) {
        return super.read(Objects.checkIndex(index, size), reader);
    }

    @Override public E write(int index, Function<? super E, ? extends E> writer) {
        return super.write(Objects.checkIndex(index, size), writer);
    }

    /* Insertion methods */

    public void insertFirst(E element) { uncheckedInsert(0, requireNonNull(element)); }
    public void insertLast(E element) { uncheckedInsert(size, requireNonNull(element)); }
    @Override public void insert(int index, E element) {
        uncheckedInsert(Objects.checkIndex(index, size + 1), requireNonNull(element));
    }

    private void uncheckedInsert(int index, E element) {
        if (size + 1 > extend()) throw new IllegalStateException("container is full");
        System.arraycopy(contents, index, contents, index + 1, size++ - index);
        contents[index] = element;
    }

    /* Removal methods */

    public E removeFirst(E element) { return uncheckedRemove(0); }
    public E removeLast(E element) { return uncheckedRemove(size - 1); }
    @Override public E remove(int index) {
        return uncheckedRemove(Objects.checkIndex(index, size));
    }

    private E uncheckedRemove(int index, E element) {
        if (isEmpty()) throw new IllegalStateException("container is empty");
        E element = restore(index);
        System.arraycopy(contents, index + 1, contents, index, --size - index);
        contents[size] = null;
        return element;
    }

    @Override public void requireBound(int bound) { extend(bound); }
    public void trimBound() {
        if (isEmpty()) contents = EMPTY;
        else if (size < contents.length) contents = Arrays.copyOf(contents, size);
    }

    protected int extend() { return extend(size + 1); }
    protected int extend(int bound) {
        bound = bound > contents.length ? strategy.nextBound(contents.length, bound) : contents.length;
        if (bound > contents.length) contents = Arrays.copyOf(contents, bound);
        return contents.length;
    }

    public static void main(String[] args) {
        System.out.println("Geometric growth:");
        ElasticArray<Integer> arr = ElasticArray.using(GrowthStrategy::doubling);
        runTest(50000, arr);
        arr = ElasticArray.using(GrowthStrategy::doubling);
        runTest(500000, arr);
        arr = ElasticArray.using(GrowthStrategy::doubling);
        runTest(2500000, arr);
        System.out.println("Linear growth:");
        arr = ElasticArray.using(GrowthStrategy.linear(DEFAULT_BOUND));
        runTest(100000, arr);
        arr = ElasticArray.using(GrowthStrategy.linear(DEFAULT_BOUND));
        runTest(500000, arr);
        arr = ElasticArray.using(GrowthStrategy.linear(DEFAULT_BOUND));
        runTest(2500000, arr);
    }

    static void runTest(double cycles, ElasticArray<Integer> arr) {
        long elapsed = System.nanoTime();
        for (int i = 0; i < cycles; i++) arr.append(i);
        elapsed = System.nanoTime() - elapsed;
        System.out.printf(
            "%.3f microseconds per insertion\n",
            (double)elapsed / cycles / 1000
        );
    }

    public static final class Builder<E> {
        static final int DEFAULT_BOUND = 10;
        static final GrowthStrategy DEFAULT_STRATEGY = GrowthStrategy.withMinimum(DEFAULT_BOUND, GrowthStrategy::golden);

        private E[] data = null;
        private int bound = DEFAULT_BOUND;
        private GrowthStrategy strategy = DEFAULT_STRATEGY;

        public Builder<E> with(int bound) { this.bound = requireNonNegative(bound, "bound"); return this; }
        public Builder<E> using(GrowthStrategy strategy) { this.strategy = strategy; return this; }
        public Builder<E> from(E[] data) { this.data = data; return this; }
        @SafeVarargs public Builder<E> of(E... elements) { return from(elements); }

        public <E> Stack<E> asStack() {
            ElasticArray<E> base = build();
            return new Stack<>() {
                public boolean isEmpty()                           { return base.isEmpty(); }
                public boolean isFull()                            { return base.isFull(); }
                public <R> R   read(Function<? super E, R> reader) { return base.readLast(reader); }
                public void    insert(E element)                   { base.insertLast(element); }
                public E       remove()                            { return base.removeLast(); }
            };
        }

        private ElasticArray<E> build() {
            if (Objects.nonNull(data)) {
                if (bound < data.length) throw new IllegalArgumentException("initial data exceeds bound");
                for (E element : data) requireNonNull(element);
                if (bound > data.length) data = Arrays.copyOf(data, bound);
            }
            return new ElasticArray<>(data, bound, strategy);
        }
    }
}
