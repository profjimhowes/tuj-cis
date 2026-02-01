package nodes;

import java.util.Objects;

public interface Readable<E> {
    <R> R read(Function<? super E, R> reader);
}