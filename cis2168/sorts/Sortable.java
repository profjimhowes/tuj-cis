public interface Sortable<E> {
    int length();
    int compare(int i, int j);
    void swap(int i, int j);
    void rotate(int start, int mid, int end);

    default boolean lesser(int i, int j) { return compare(i, j) < 0; }
    default boolean swapLesser(int i, int j) {
        if (lesser(i, j)) { swap(i, j); return true; }
        return false;
    }

    default boolean greater(int i, int j) { return compare(i, j) > 0; }
    default boolean swapGreater(int i, int j) {
        if (greater(i, j)) { swap(i, j); return true; }
        return false;
    }

    default void reverse(int start, int end) {
        while (start < end) swap(start++, --end);
    }

    default boolean isSorted() {
        for (int i = 1; i < length(); i++)
            if lesser(i, i - 1) return false;
        return true;
    }
}