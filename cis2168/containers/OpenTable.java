import java.util.Objects;

public class OpenTable<E> {
    @FunctionalInterface public static interface ProbeStrategy {
        /**
         * Compute the table index for the given probe attempt.
         *
         * @param hash     the hash of the element
         * @param attempt  the probe attempt number, starting from 0
         * @param size     the current table size
         * @return         a valid table index in [0, size)
         */
        int probe(int hash, int attempt, int size);

        public static int sequential(int hash, int attempt, int size) {
            return (hash + attempt) % size;
        }

        // Probe step follows triangular numbers, which will cover the
        // entire table when size is a power of 2.
        public static int triangular(int hash, int attempt, int size) {
            return (hash + (attempt * (attempt + 1) / 2)) % size;
        }

        // Probe step is derived from the upper bits of the hash itself
        // because in typical table sizes the upper bits aren't used.
        public static int splitHash(int hash, int attempt, int size) {
            int step = (hash ^ (hash >>> 16)) | 1;
            return (hash + step * attempt) % size;
        }

        public static ProbeStrategy linear(int c) {
            if (c == 0) throw new IllegalArgumentException("probe step cannot be 0");
            return (hash, attempt, size) -> (hash + c * attempt) % size;
        }

        public static ProbeStrategy quadratic(int c1, int c2) {
            if (c2 == 0) throw new IllegalArgumentException("probe step cannot be 0");
            return (hash, attempt, size) ->
                (hash + (c1 + c2 * attempt) * attempt) % size;
        }
    }

    private static final Object GHOST = new Object();

    private static final ProbeStrategy DEFAULT_STRATEGY = ProbeStrategy::sequential;
    private static final int DEFAULT_SIZE = 16;
    private static final double DEFAULT_MAX_OCCUPANCY = 0.5;

    private final ProbeStrategy prober;
    private Object[] contents;
    private int size, entries, ghosts;
    private final double maxOccupancy;

    public OpenTable() { this(DEFAULT_STRATEGY, DEFAULT_SIZE, DEFAULT_MAX_OCCUPANCY); }
    public OpenTable(ProbeStrategy prober, int size, double maxOccupancy) {
        if (size < 1) throw new IllegalArgumentException("size must be > 0");
        if (maxOccupancy <= 0 || maxOccupancy >= 1)
            throw new IllegalArgumentException("maxOccupancy must be between 0.0 and 1.0");
        this.prober = Objects.requireNonNull(prober);
        this.contents = new Object[this.size = size];
        this.entries = this.ghosts = 0;
        this.maxOccupancy = maxOccupancy;
    }

    public double occupancy() { return (double)(entries + ghosts) / size; }

    public boolean insert(E element) {
        int hash = Objects.requireNonNull(element).hashCode();
        if (occupancy() > maxOccupancy)
            rehash(ghosts > entries ? size : size * 2);
        int pos = -1; boolean replace_ghost = false;
        for (int i = 0; i < size; i++) {
            int p = prober.probe(hash, i, size);
            if (Objects.isNull(contents[p])) {
                pos = pos < 0 ? p : pos; break;
            } else if (GHOST.equals(contents[p]) && pos < 0) {
                pos = p; replace_ghost = true;
            } else if (element.equals(contents[p]))
                return false;
        }
        if (pos >= 0) {
            contents[pos] = element; entries++;
            if (replace_ghost) ghosts--;
            return true;
        } else return false;
    }

    public boolean contains(E element) {
        int hash = Objects.requireNonNull(element).hashCode();
        for (int i = 0; i < size; i++) {
            int p = prober.probe(hash, i, size);
            if (Objects.isNull(contents[p])) return false;
            else if (element.equals(contents[p])) return true;
        }
        return false;
    }

    public boolean remove(E element) {
        int hash = Objects.requireNonNull(element).hashCode();
        for (int i = 0; i < size; i++) {
            int p = prober.probe(hash, i, size);
            if (Objects.isNull(contents[p])) return false;
            else if (element.equals(contents[p])) {
                contents[p] = GHOST;
                entries--; ghosts++;
                return true;
            }
        }
        return false;
    }

    @SuppressWarnings("unchecked") private void rehash(int size) {
        Object[] oldContents = contents;
        contents = new Object[this.size = size];
        entries = ghosts = 0;
 
        for (Object element : oldContents)
            if (Objects.nonNull(element) && !GHOST.equals(element))
                insert((E)element);
    }

    public static void main(String[] args) {
        OpenTable<String> stable = new OpenTable<>();
        stable.insert("Hello");
        stable.insert("World");
        System.out.printf("Occupancy: %.3f%n", stable.occupancy());
        System.out.println("Contains \"Hello\": " + stable.contains("Hello"));
        System.out.println("Contains \"Goodbye\": " + stable.contains("Goodbye"));
        stable.insert("Goodbye");
        stable.insert("World");
        stable.remove("Hello");
        System.out.printf("Occupancy: %.3f%n", stable.occupancy());
        System.out.println("Contains \"Hello\": " + stable.contains("Hello"));
        System.out.println("Contains \"Goodbye\": " + stable.contains("Goodbye"));
    }
}
