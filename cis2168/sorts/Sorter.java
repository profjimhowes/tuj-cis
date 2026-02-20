import java.util.Arrays;
import java.util.function.*;
import java.util.Random;

public class Sorter<E extends Comparable<? super E>> {
    @FunctionalInterface
    private static interface Swapper { void swap(int i, int j); }

    private IntBinaryOperator comparator;
    private Swapper swapper;
    public long compares, swaps;

    private E[] initialize(E[] array) {
        swapper = (i, j) -> { E t = array[i]; array[i] = array[j]; array[j] = t; };
        comparator = (i, j) -> array[i].compareTo(array[j]);
        compares = 0; swaps = 0;
        return array;
    }

    private void swap(int i, int j) {
        if (i == j) return;
        swaps++;
        swapper.swap(i, j);
    }

    private int compare(int i, int j) {
        compares++;
        return comparator.applyAsInt(i, j);
    }

    private void reverse(int i, int j) {
        while (i < j) swap(i++, j--);
    }

    public boolean isSorted(E[] array, int start, int end) {
        while (start < end)
            if (comparator.applyAsInt(start++, start) > 0)
                return false;
        return true;
    }

    /* Selection sort
     * 1. For i=0, find minimum of [i, size) and swap it with i.
     * 2. Increment i and repeat until i==end-1.
     */ 
    public void selection(E[] array) { selection(initialize(array), 0, array.length - 1); }
    private void selection(E[] array, int start, int end) {
        while (start < end)
            swap(start, imin(start++, end));
    }

    private int imin(int start, int end) {
        int min = start;
        while (start++ < end)
            if (compare(start, min) < 0)
                min = start;
        return min;
    }

    /* Bubble sort
     * 1. Bubble up from 0 to end-1.
     * 2. Decrement end and repeat if not sorted already.
     */ 
    public void bubble(E[] array) { bubble(initialize(array), 0, array.length - 1); }
    private void bubble(E[] array, int start, int end) {
        while (start < end && !bubbleUp(array, start, end--));
    }

    private boolean bubbleUp(E[] array, int start, int end) {
        boolean sorted = true;
        while (start < end)
            if (compare(start++, start) > 0) {
                swap(start - 1, start);
                sorted = false;
            }
        return sorted;
    }

    private boolean bubbleDown(E[] array, int start, int end) {
        boolean sorted = true;
        while (start < end)
            if (compare(end--, end) < 0) {
                swap(end, end + 1);
                sorted = false;
            }
        return sorted;
    }

    /* Insertion sort
     * 1. For i=1, insert element i into [0, i].
     * 2. Increment i and repeat until i=end-1.
     */ 
    public void insertion(E[] array) { insertion(initialize(array), 0, array.length - 1); }
    private void insertion(E[] array, int start, int end) {
        for (int i = start; i < end; i++)
            insertDown(i + 1, start);
    }

    private void insertDown(int i, int bound) {
        while (i > bound && compare(i - 1, i) > 0)
            swap(i--, i);
    }

    private void insertUp(int i, int bound) {
        while (i < bound && compare(i, i + 1) > 0)
            swap(i++, i);
    }

    /* Optimized insertion sort "DX"
     * 1. Bubble down once while detecting and reversing descending runs
     * 2. Perform insertion sort from i=1 with unchecked array bounds
     */ 
    public void insertionDX(E[] array) { insertionDX(initialize(array), 0, array.length - 1); }
    private void insertionDX(E[] array, int start, int end) {
        bubbleDownRRD(array, start, end);
        while (start++ < end) insertDown(start);
    }

    private void insertDown(int i) { while (compare(i - 1, i) > 0) swap(i--, i); }
    private void insertUp(int i) { while (compare(i, i + 1) > 0) swap(i++, i); }
    private void bubbleDownRRD(E[] array, int start, int end) {
        for (int i = end; start < i; i--) {
            if (i < end && compare(i - 1, i + 1) <= 0) {
                reverse(i + 1, end);
                end = i;
            }
            if (compare(i, i - 1) < 0) swap(i, i - 1);
        }
        if (start < end) reverse(start + 1, end);
    }

    private static void printHeader(String label, int size) {
        System.out.printf("%n%6s, N=%-8dcompare\\element   swap\\element%n", label, size);
        System.out.println("================================================");
    }

    private static void printStats(String label, int size, long compares, long swaps) {
        System.out.printf("%-20s%10.1f%15.1f%n", label, (double)compares / (double)size, (double)swaps / (double)size);
    }

    public static void main(String[] args) {
        int size = 0;
        try { size = Integer.parseInt(args[0]); }
        catch (Exception e) { size = 0; }
        size = size > 0 ? size : 1000;

        Sorter<Integer> sorter = new Sorter<>();
        Integer[] array = new Integer[size];

        for (int i = 0; i < size; i++) array[i] = i;
        printHeader("Sorted", size);
        sorter.selection(array);
        printStats("Selection", size, sorter.compares, sorter.swaps);
        sorter.bubble(array);
        printStats("Bubble", size, sorter.compares, sorter.swaps);
        sorter.insertion(array);
        printStats("Insertion", size, sorter.compares, sorter.swaps);
        sorter.insertionDX(array);
        printStats("InsertionDX", size, sorter.compares, sorter.swaps);

        for (int i = 0; i < size; i++) array[i] = size - i - 1;
        printHeader("Revrsd", size);
        sorter.selection(Arrays.copyOf(array, array.length));
        printStats("Selection", size, sorter.compares, sorter.swaps);
        sorter.bubble(Arrays.copyOf(array, array.length));
        printStats("Bubble", size, sorter.compares, sorter.swaps);
        sorter.insertion(Arrays.copyOf(array, array.length));
        printStats("Insertion", size, sorter.compares, sorter.swaps);
        sorter.insertionDX(array);
        printStats("InsertionDX", size, sorter.compares, sorter.swaps);

        Random rand = new Random();
        for (int i = 0; i < size; i++) {
            int j = rand.nextInt(i + 1);
            array[i] = array[j]; array[j] = i;
        }
        printHeader("Random", size);
        sorter.selection(Arrays.copyOf(array, array.length));
        printStats("Selection", size, sorter.compares, sorter.swaps);
        sorter.bubble(Arrays.copyOf(array, array.length));
        printStats("Bubble", size, sorter.compares, sorter.swaps);
        sorter.insertion(Arrays.copyOf(array, array.length));
        printStats("Insertion", size, sorter.compares, sorter.swaps);
        sorter.insertionDX(Arrays.copyOf(array, array.length));
        printStats("InsertionDX", size, sorter.compares, sorter.swaps);

        for (int i = 0; i < size;) {
            int step = (int)Math.min(Math.round(-Math.pow(size, 0.8) * Math.log(Math.random()) + 0.5), size - i);
            Arrays.sort(array, i, i += step);
            if (Math.random() > 0.5) for (int j = i - step, k = i - 1; j < k;) {
                int t = array[j]; array[j++] = array[k]; array[k--] = t;
            }
        }
        printHeader("Spiky", size);
        sorter.selection(Arrays.copyOf(array, array.length));
        printStats("Selection", size, sorter.compares, sorter.swaps);
        sorter.bubble(Arrays.copyOf(array, array.length));
        printStats("Bubble", size, sorter.compares, sorter.swaps);
        sorter.insertion(Arrays.copyOf(array, array.length));
        printStats("Insertion", size, sorter.compares, sorter.swaps);
        sorter.insertionDX(array);
        printStats("InsertionDX", size, sorter.compares, sorter.swaps);
    }
}
