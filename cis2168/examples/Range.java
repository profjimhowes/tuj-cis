public record Range(
    int start, int end, int step
) implements Iterable<Integer> {
    Range {
        if (step == 0) throw new IllegalArgumentException("step must be non-zero");
    }

    @Override public Iterator<Integer> iterator() {
        return new Iterator<>() {
            private int current = start;
            @Override public boolean hasNext() {
                return step > 0 ? current < end : current > end;
            }
            @Override public Integer next() {
                if (!hasNext()) throw new NoSuchElementException();
                int v = current; current += step; return v;
            }
        };
    }
}