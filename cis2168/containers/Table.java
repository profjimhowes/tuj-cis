package containers;

public interface Table<E> extends
Container.Insertable<E>,
Container.Removable<E>,
Container.Queryable<E> {
    public double occupancy();

    abstract class Base<E> implements Table<E> {
        int size, entries;
        final double maxOccupancy;

        public boolean isEmpty() { return entries == 0; }
        public boolean isFull() { return false; }
    }
}