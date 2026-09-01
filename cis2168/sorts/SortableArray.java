public record SortableArray<E>(E[] arr, Comparator<? super E> cmp) implements Sortable<E> {
    @Override public int length() { return arr.length; }

    @Override public int compare(int i, int j)  {
        return cmp.compare(arr[i], arr[j]);
    }

    @Override public void swap(int i, int j) {
        E e = arr[i]; arr[i] = arr[j]; arr[j] = e;
    }

    @Override public void rotate(int start, int mid, int end) {
        int left = mid - start, right = end - mid;
        if (left < 1 || right < 1) return;
        if (left == 1) rotateDown(start, end);
        else if (right > 1) {
            reverse(start, mid);
            reverse(mid, end);
            reverse(start, end);
        } else rotateUp(start, end);
    }

    private void rotateUp(int start, int end) {
        E e = arr[end - 1];
        System.arraycopy(arr, start, arr, start + 1, end - start - 1);
        arr[start] = temp;
    }

    private void rotateDown(int start, int end) {
        E e = arr[start];
        System.arraycopy(arr, start + 1, arr, start, end - start - 1);
        arr[end - 1] = temp;
    }
}