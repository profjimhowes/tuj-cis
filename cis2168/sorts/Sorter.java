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

    public boolean isSorted(int start, int end) {
        while (start < end)
            if (comparator.applyAsInt(start++, start) > 0)
                return false;
        return true;
    }

    /* Selection sort
     * 1. For i=0, find minimum of [i, size) and swap it with i.
     * 2. Increment i and repeat until i==end-1.
     */ 
    public void selection(E[] array) { selection(0, initialize(array).length - 1); }
    private void selection(int start, int end) {
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
    public void bubble(E[] array) { bubble(0, initialize(array).length - 1); }
    private void bubble(int start, int end) {
        while (start < end && !bubbleUp(start, end--));
    }

    private boolean bubbleUp(int start, int end) {
        boolean sorted = true;
        while (start < end)
            if (compare(start++, start) > 0) {
                swap(start - 1, start);
                sorted = false;
            }
        return sorted;
    }

    private boolean bubbleDown(int start, int end) {
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
    public void insertion(E[] array) { insertion(0, initialize(array).length - 1); }
    private void insertion(int start, int end) {
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
    public void insertionDX(E[] array) { insertionDX(0, initialize(array).length - 1); }
    private void insertionDX(int start, int end) {
        bubbleDownRRD(start, end);
        while (start++ < end) insertDown(start);
    }

    private void insertDown(int i) { while (compare(i - 1, i) > 0) swap(i--, i); }
    private void insertUp(int i) { while (compare(i, i + 1) > 0) swap(i++, i); }
    private void bubbleDownRRD(int start, int end) {
        for (int i = end; start < i; i--) {
            if (i < end && compare(i - 1, i + 1) <= 0) {
                reverse(i + 1, end);
                end = i;
            }
            if (compare(i, i - 1) < 0) swap(i, i - 1);
        }
        if (start < end) reverse(start + 1, end);
    }

    /* Top-down (recursive) mergesort
     * 1. Recursively sort left and right halves
     * 2. Merge sorted halves
     */ 
    public void mergeDown(E[] array) {
        mergeDown(initialize(array), Arrays.copyOf(array, array.length), 0, array.length - 1);
    }

    private void mergeDown(E[] array, E[] buffer, int start, int end) {
        if (end - start < 1) return;
        int mid = (start + end + 1) / 2;
        mergeDown(buffer, array, start, mid - 1);
        mergeDown(buffer, array, mid, end);
        merge(array, buffer, start, mid, end);
    }

    private void merge(E[] array, E[] buffer, int start, int mid, int end) {
        comparator = (i, j) -> buffer[i].compareTo(buffer[j]);
        if (compare(mid - 1, mid) > 0) {
            int left = start, right = mid;
            for (int i = start; i <= end; i++)
                if (right > end) array[i] = buffer[left++];
                else if (left >= mid || compare(left, right) > 0) array[i] = buffer[right++];
                else array[i] = buffer[left++];
        } else System.arraycopy(buffer, start, array, start, end - start + 1);
        swaps += (end - start) / 2 + 1;
    }

    /* Bottom-up (iterative) mergesort
     * 1. Sort blocks of size n = 2
     * 2. Merge pairs of blocks
     * 3. Double n and repeat until n >= length / 2
     */ 
    public void mergeUp(E[] array) {
        mergeUp(initialize(array), Arrays.copyOf(array, array.length), 0, array.length - 1);
    }

    private void mergeUp(E[] array, E[] buffer, int start, int end) {
        for (int n = 1, swaps = 0; n < end - start + 1 || swaps % 2 == 1; n *= 2, swaps++) {
            for (int i = start + n; i <= end; i += n + 1)
                merge(array, buffer, i - n, i, Math.min(i += n - 1, end));
            E[] t = array; array = buffer; buffer = t;
        }
    }

    /* Bitonic sort (recursive)
     * 1. Recursively sort left and right halves in opposite directions
     * 2. Recursively merge halves using a bitonic partition
     */
    public void bitonicDown(E[] array) { bitonicDown(0, initialize(array).length - 1); }
    private void bitonicDown(int start, int end) {
        if (end - start < 1) return;
        int mid = (end + start) / 2;
        bitonicDown(start, mid);
        bitonicDown(mid + 1, end);
        if (compare(mid, end) > 0)
            bitonicMerge(start, end, true);
        else reverse(mid + 1, end);
    }

    private void bitonicMerge(int start, int end, boolean initial) {
        if (end - start < 1) return;
        int size = end - start + 1;
        int left = start + size % 2, right = left + size / 2;
        for (int i = left, j = initial ? end : right; i < right; i++, j += initial ? -1 : 1)
            if (compare(i, j) > 0) swap(i, j);
        bitonicMerge(left, right - 1, false);
        bitonicMerge(right, end, false);
        while (start < end && compare(start, start + 1) > 0)
            swap(start++, start);
    }

    /* Bitonic sort (iterative)
     */
    public void bitonicUp(E[] array) { bitonicUp(0, initialize(array).length - 1); }
    private void bitonicUp(int start, int end) {
        int n = end - start + 1;
        for (int k = 2; k < n * 2; k <<= 1)
            for (int j = k; j > 0; j >>= 1)
			    for (int i = 0; i < n; i++) {
				    int ij = j == k ? i ^ j - 1 : i ^ j;
                    if (i < ij && ij < n && compare(i, ij) > 0) swap(i, ij);
                }
    }

    /* Quicksort with tail-call loop optimization
     * 1. Partition array
     * 2. Recursively sort smaller partition
     * 3. Adjust bounds to reuse stack frame for larger partition
     */ 
    public void quick(E[] array) { quick(0, initialize(array).length - 1); }
    private void quick(int start, int end) {
        while (end - start > 0) {
            int mid = partition(start, end);
            if (2 * mid < start + end) {
                quick(start, mid - 1);
                start = mid + 1;
            } else {
                quick(mid + 1, end);
                end = mid - 1;
            }
        }
    }

    private int partition(int start, int end) {
        for (int i = start--; i < end; i++)
            if (compare(i, end) <= 0)
                swap(i, ++start);
        swap(++start, end);
        return start;
    }

    /* Comparative performance testing */

    private static void printHeader(String label, int size) {
        System.out.printf("%n%-14sN=%-8dcompare\\element   swap\\element%n", label, size);
        System.out.println("======================================================");
    }

    private static void printStats(String label, int size, long compares, long swaps) {
        System.out.printf("%-26s%10.1f%15.1f%n", label, (double)compares / (double)size, (double)swaps / (double)size);
    }

    private static void runTest(String label, Integer[] array, Sorter<Integer> sorter) {
        printHeader(label, array.length);
        sorter.selection(Arrays.copyOf(array, array.length));
        printStats("Selection", array.length, sorter.compares, sorter.swaps);
        sorter.bubble(Arrays.copyOf(array, array.length));
        printStats("Bubble", array.length, sorter.compares, sorter.swaps);
        sorter.insertion(Arrays.copyOf(array, array.length));
        printStats("Insertion", array.length, sorter.compares, sorter.swaps);
        sorter.insertionDX(Arrays.copyOf(array, array.length));
        printStats("InsertionDX", array.length, sorter.compares, sorter.swaps);
        sorter.mergeDown(Arrays.copyOf(array, array.length));
        printStats("Mergesort (recursive)", array.length, sorter.compares, sorter.swaps);
        sorter.mergeUp(Arrays.copyOf(array, array.length));
        printStats("Mergesort (iterative)", array.length, sorter.compares, sorter.swaps);
        sorter.bitonicDown(Arrays.copyOf(array, array.length));
        printStats("Bitonic (recursive)", array.length, sorter.compares, sorter.swaps);
        sorter.bitonicUp(Arrays.copyOf(array, array.length));
        printStats("Bitonic (iterative)", array.length, sorter.compares, sorter.swaps);
        sorter.quick(Arrays.copyOf(array, array.length));
        printStats("Quicksort", array.length, sorter.compares, sorter.swaps);
    }

    public static void main(String[] args) {
        int size = 0;
        try { size = Integer.parseInt(args[0]); }
        catch (Exception e) { size = 0; }
        size = size > 0 ? size : 1000;

        Sorter<Integer> sorter = new Sorter<>();
        Integer[] array = new Integer[size];

        for (int i = 0; i < size; i++) array[i] = i;
        runTest("Sorted", array, sorter);

        for (int i = 0; i < size; i++) array[i] = size - i - 1;
        runTest("Reversed", array, sorter);

        Random rand = new Random();
        for (int i = 0; i < size; i++) {
            int j = rand.nextInt(i + 1);
            array[i] = array[j]; array[j] = i;
        }
        runTest("Random", array, sorter);

        for (int i = 0; i < size;) {
            int step = (int)Math.min(Math.round(-Math.pow(size, 0.8) * Math.log(rand.nextDouble()) + 0.5), size - i);
            Arrays.sort(array, i, i += step);
            if (rand.nextDouble() > 0.5) for (int j = i - step, k = i - 1; j < k;) {
                int t = array[j]; array[j++] = array[k]; array[k--] = t;
            }
        }
        runTest("Spiky", array, sorter);
    }
}
