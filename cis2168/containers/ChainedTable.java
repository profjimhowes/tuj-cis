import java.util.Objects;
import java.util.List;
import java.util.LinkedList;

public class ChainedTable<E> {
    private static final int DEFAULT_SIZE = 16;
    private static final double DEFAULT_MAX_OCCUPANCY = 1.0;

    private List<E>[] contents;
    private int size, entries;
    private final double maxOccupancy;

    public ChainedTable() { this(DEFAULT_SIZE, DEFAULT_MAX_OCCUPANCY); }
    public ChainedTable(int size, double maxOccupancy) {
        if (size < 1) throw new IllegalArgumentException("size must be > 0");
        if (maxOccupancy <= 0) throw new IllegalArgumentException("maxOccupancy must be > 0.0");
        this.contents = createTable(this.size = size);
        this.entries = 0;
        this.maxOccupancy = maxOccupancy;
    }

    public boolean isEmpty() { return entries == 0; }
    public double occupancy() { return (double)entries / size; }

    private int checkedIndex(E element) {
        return Objects.requireNonNull(element).hashCode() % size;
    }

    public boolean insert(E element) {
        int i = checkedIndex(element);
        if (occupancy() > maxOccupancy) rehash(size * 2);
        if (Objects.nonNull(contents[i])) {
            for (E o : contents[i])
                if (element.equals(o))
                    return false;
        } else contents[i] = new LinkedList<>();
        contents[i].add(element);
        entries++;
        return true;
    }

    public boolean contains(E element) {
        int i = checkedIndex(element);
        if (Objects.nonNull(contents[i]))
            for (E o : contents[i])
                if (element.equals(o))
                    return true;
        return false;
    }

    public boolean remove(E element) {
        int i = checkedIndex(element);
        if (Objects.nonNull(contents[i]) && contents[i].remove(element)) {
            entries--;
            if (contents[i].isEmpty()) contents[i] = null;
            return true;
        } else return false;
    }

    private void rehash(int size) {
        List<E>[] oldContents = contents;
        contents = createTable(this.size = size);
        entries = 0;
 
        for (List<E> list : oldContents)
            if (Objects.nonNull(list))
                for (E element : list)
                    insert(element);
    }

    @SuppressWarnings("unchecked") private List<E>[] createTable(int size) {
        return (List<E>[]) new List<?>[size];
    }

    public static void main(String[] args) {
        ChainedTable<String> stable = new ChainedTable<>();
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

        stable.insert("polygenelubricants");
    }
}