package containers;

import java.util.Objects;
import java.util.Arrays;
import java.util.function.*;

public final class ElasticArray<E> extends FixedArray<E>
implements Container.Elastic, Indexed.Insertable<E>, Indexed.Removable<E> {
    static final int DEFAULT_BOUND = 10;
    static final GrowthStrategy DEFAULT_STRATEGY = GrowthStrategy::golden;

    int size;
    final GrowthStrategy strategy;

    ElasticArray(int bound, GrowthStrategy strategy) { this(new Object[bound], 0, strategy); }
    ElasticArray(Object[] array, int size, GrowthStrategy strategy) {
        super(array); this.size = size; this.strategy = strategy;
    }

    static <E> ElasticArray<E> with(int bound) {
        return new ElasticArray<>(requireNonNegative(bound, "bound"), DEFAULT_STRATEGY);
    }

    static <E> ElasticArray<E> using(GrowthStrategy strategy) {
        return new ElasticArray<>(DEFAULT_BOUND, strategy);
    }

    static <E> ElasticArray<E> from(E[] array) {
        for (E e : array) Objects.requireNonNull(e, "null elements not permitted");
        return new ElasticArray<E>(array, array.length, DEFAULT_STRATEGY);
    }

    @SafeVarargs static <E> ElasticArray<E> of(E... elements) {
        return from(elements);
    }

    @Override public boolean isFull() {
        return size >= contents.length && strategy.nextBound(size, size + 1) <= size;
    }

    @Override public int size() { return size; }

    @Override public <R> R get(int index, Function<? super E, R> reader) {
        Objects.checkIndex(index, size);
        return super.get(index, reader);
    }

    @Override public <R> R get(int index, int index2, BiFunction<? super E, ? super E, R> reader) {
        Objects.checkIndex(index, size);
        Objects.checkIndex(index2, size);
        return super.get(index, index2, reader);
    }

    @Override public E put(int index, E element) {
        Objects.checkIndex(index, size);
        return super.put(index, element);
    }

    @Override public void swap(int index, int index2) {
        Objects.checkIndex(index, size);
        Objects.checkIndex(index2, size);
        super.swap(index, index2);
    }

    @Override public void insert(int index, E element) {
        if (isFull()) throw new IllegalStateException("container is full");
        Objects.checkIndex(index, size + 1);
        Objects.requireNonNull(element, "null elements not permitted");
        if (size + 1 > contents.length) grow();
        System.arraycopy(contents, index, contents, index + 1, size++ - index);
        contents[index] = element;
    }

    @Override public E remove(int index) {
        if (isEmpty()) throw new IllegalStateException("container is empty");
        Objects.checkIndex(index, size);
        E element = restore(index);
        System.arraycopy(contents, index + 1, contents, index, --size - index);
        contents[size] = null;
        return element;
    }

    public void requireBound(int bound) { if (bound > contents.length) grow(bound); }

    private void grow() { grow(size + 1); }
    private void grow(int minBound) {
        final int newBound = strategy.nextBound(contents.length, minBound);
        if (newBound > contents.length)
            contents = Arrays.copyOf(contents, newBound);
    }

    static int requireNonNegative(int value, String name) {
        if (value < 0)
            throw new IllegalArgumentException(name + " must be non-negative: " + value);
        return value;
    }

    public static void main(String[] args) {
        ElasticArray<Integer> arr = ElasticArray.using(GrowthStrategy::doubling);
        for (int i = 0; i < 1000; i++) arr.append(i);
        System.out.printf("1000 elements added, current size: %d\n", arr.size());
        for (int i = 0; i < 9000; i++) arr.append(i);
        System.out.printf("10000 elements added, current size: %d\n", arr.size());
        arr = ElasticArray.using(GrowthStrategy::none);
        System.out.println("Overflowing a bounded array should throw");
        for (int i = 0; i < 20; i++) {
            System.out.printf("Appending to size %d array...\n", arr.size());
            arr.append(i);
        }
    }
}