import java.util.function.*;

public interface Accessor<T> {
    void    accept(Consumer<T> c);
    boolean test(Predicate<T> p);
    int     apply(ToIntFunction<T> f);
    long    apply(ToLongFunction<T> f);
    double  apply(ToDoubleFunction<T> f);
    <R> R   apply(Function<T, R> f);
}