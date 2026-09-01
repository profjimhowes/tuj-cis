import java.util.function.*;

/**
 * A bounded or unbounded buffer: a structure that holds elements in order to enforce
 * <em>sequenced</em> access rather than random access.
 *
 * <p>A {@code Container} is the control-flow counterpart to a collection. Where a
 * collection answers questions of the form "is <var>x</var> present?" or "what lives at
 * position <var>i</var>?" — that is, lookup and retrieval — a container answers only
 * "what comes next?". It exposes no notion of position, key, or membership; the single
 * element ever made available is whichever one the structure's access discipline
 * selects. The concrete discipline (last-in-first-out, first-in-first-out, by-priority,
 * &hellip;) is fixed by the implementing type, not by this interface.
 *
 * @param <T> the type of element held
 * @see Stack
 * @see Queue
 * @see PriorityQueue
 */
public interface Container<T> {
    boolean isEmpty();
    boolean isFull();
    
    void insert(T thing);
    default boolean offer(T thing) {
        if (isFull()) return false;
        insert(thing);
        return true;
    }
    
    T remove();
    default T poll() {
        if (isEmpty()) return null;
        return remove();
    }

    void peek(Consumer<T> peeker);
    boolean test(Predicate<T> tester);
    <R> R read(Function<T, R> reader);
}