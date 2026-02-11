package containers;

import java.util.function.*;

public interface Container<E> {
    boolean isEmpty();
    boolean isFull();

    interface Readable<E>   extends Container<E> { <R> R read(Function<? super E, R> reader); }
    interface Writable<E>   extends Container<E> { E write(Function<? super E, ? extends E> writer); }
    interface Insertable<E> extends Container<E> { void insert(E element); }
    interface Removable<E>  extends Container<E> { E remove(); }

    interface Sized<E> extends Container<E> { int size(); }
    interface Elastic<E> extends Sized<E> {
        void requireBound(int bound);

        @FunctionalInterface interface GrowthStrategy {
            int nextBound(int current, int required);

            // Built-in strategies
            static int doubling(int current, int required) { return Math.max((int)(current * 2), required); }
            static int golden(int current, int required) { return Math.max((int)(current * 1.5), required); }
            static int exact(int current, int required) { return required; }
            static int none(int current, int required) { return current; }

            // Simple parameterized strategies
            static GrowthStrategy linear(int step) {
                return (current, required) -> Math.max(current + step, required);
            }
            
            static GrowthStrategy geometric(double factor) {
                return (current, required) -> Math.max((int)(current * factor), required);
            }

            // Minimum bound
            static GrowthStrategy withMinimum(int min, GrowthStrategy strategy) {
                return (current, required) -> Math.max(min, strategy(current, required));
            }
        }
    }

    /* Checks and assertions */
    public static <E> E requireNonNull(E element) {
        return Objects.requireNonNull(element, "null elements not permitted")
    }

    public static int requireNonNegative(int value, String name) {
        if (value < 0) throw new IllegalArgumentException(name + " must be non-negative: " + value);
        return value;
    }
}
