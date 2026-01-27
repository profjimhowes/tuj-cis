package containers;

import java.util.function.Function;

public interface Container<E> {
    boolean isEmpty();

    interface Queryable<E>  extends Container<E> { boolean contains(E element); }
    interface Readable<E>   extends Container<E> { <R> R get(Function<? super E, R> reader); }
    interface Insertable<E> extends Container<E> { void insert(E element); }
    interface Removable<E>  extends Container<E> { E remove(); }

    interface Bounded { boolean isFull(); }
    interface Elastic extends Bounded {
        void requireBound(int bound);

        @FunctionalInterface interface GrowthStrategy {
            int nextBound(int current, int required);

            // Built-in strategies
            static GrowthStrategy linear(int step) {
                return (current, required) -> Math.max(current + step, required);
            }
            
            static GrowthStrategy geometric(double factor) {
                return (current, required) -> Math.max((int)(current * factor), required);
            }
            
            static int doubling(int current, int required) { return Math.max((int)(current * 2), required); }
            static int golden(int current, int required) { return Math.max((int)(current * 1.5), required); }
            static int none(int current, int required) { return current; }
        }
    }
}