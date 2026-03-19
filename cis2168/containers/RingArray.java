package containers;

public final class RingArray<E> extends ElasticArray<E> {
    private int first, last;

    /* Insertion methods */

    public void insertFirst(E element) { uncheckedInsert(0, requireNonNull(element)); }
    public void insertLast(E element) { uncheckedInsert(size, requireNonNull(element)); }
    @Override public void insert(int index, E element) {
        uncheckedInsert(Objects.checkIndex(index, size + 1), requireNonNull(element));
    }

    private void uncheckedInsert(int index, E element) {
        if (size + 1 > extend()) throw new IllegalStateException("container is full");
        int bound = contents.length, bias = Integer.signum(index - size + index);
        if (bias == 0 && first > last) bias = Integer.signum(first + last - bound);
        int offset = (bound + first + index + (bias - 1) / 2) % bound;
        if (bias < 0) {
            if (offset < last) {
                if (first == 0) first = bound - 1;
                else System.arraycopy(contents, first, contents, first - 1, Math.min(index, bound - first--));
                if (first >= last) contents[bound - 1] = contents[0];
                System.arraycopy(contents, 1, contents, 0, Math.clamp(first - offset, 0, offset));
            } else System.arraycopy(contents, first, contents, --first, index);
        } else if (offset > last) {
            System.arraycopy(contents, 0, contents, 1, last++);
            contents[0] = contents[bound - 1];
            System.arraycopy(contents, offset, contents, offset + 1, bound - offset - 1);
        } else System.arraycopy(contents, offset, contents, offset + 1, last++ - offset);
        contents[offset] = element;
    }

    @Override public void requireBound(int bound) { extend(bound); }
    @Override public void trimBound() {
        if (isEmpty()) contents = EMPTY;
        else if (size < contents.length) {
            if (first < last) {
                contents = Arrays.copyOfRange(contents, first, last);
                last = 0;
            } else if (last < contents.length - first) {
                System.arraycopy(contents, 0, contents, first - last, first = last);
                contents = Arrays.copyOfRange(contents, contents.length - size, contents.length);
            } else {
                System.arraycopy(contents, first, contents, last, contents.length - first);
                contents = Arrays.copyOfRange(contents, 0, size);
            }
            first = last;
        }
    }

    @Override protected int extend(int bound) {
        if (contents.length < super.extend(bound) && first > 0)
            if (size - first > last) {
                System.arraycopy(contents, 0, contents, size, last);
                Arrays.fill(contents, 0, last, null);
                last += size;
            } else {
                System.arraycopy(contents, first, contents, first + contents.length - size, size - first);
                Arrays.fill(contents, first, size, null);
                first += contents.length - size;
            }
        return contents.length;
    }
}