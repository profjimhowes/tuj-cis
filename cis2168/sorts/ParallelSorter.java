import java.util.concurrent.*;
import java.util.Arrays;
import java.util.List;
import java.util.ArrayList;
import java.util.function.*;
import java.util.Random;

public class ParallelSorter<E extends Comparable<? super E>> {
    private static final int THRESHOLD = 1024;
    private static final int CAS_DEFAULT = 4;

    @FunctionalInterface
    private static interface Swapper { void swap(int i, int j); }

    private IntBinaryOperator comparator;
    private Swapper swapper;
    private int casThreads = CAS_DEFAULT;

    private E[] initialize(E[] array) {
        swapper = (i, j) -> { E t = array[i]; array[i] = array[j]; array[j] = t; };
        comparator = (i, j) -> array[i].compareTo(array[j]);
        return array;
    }

    private void swap(int i, int j) { if (i != j) swapper.swap(i, j); }
    private int compare(int i, int j) { return comparator.applyAsInt(i, j); }
    private void reverse(int i, int j) { while (i < j) swap(i++, j--); }

    private class BitonicTask extends RecursiveAction {
        int start, end;
        boolean ascending;

        BitonicTask(int start, int end, boolean ascending) {
            this.start = start; this.end = end; this.ascending = ascending;
        }

        @Override
        protected void compute() {
            if (end - start > THRESHOLD) {
                int mid = (start + end) / 2;
                invokeAll(
                    new BitonicTask(start, mid, ascending),
                    new BitonicTask(mid + 1, end, !ascending)
                );
                if (compare(mid, end) > 0 == ascending)
                    new MergeTask(start, end, ascending).invoke();
                else reverse(mid + 1, end);
            } else if (end - start > 1)
                bitonic(start, end, ascending);
        }
    }

    private class MergeTask extends RecursiveAction {
        int start, end;
        boolean ascending;

        MergeTask(int start, int end, boolean ascending) {
            this.start = start; this.end = end; this.ascending = ascending;
        }

        @Override
        protected void compute() {
            if (end - start > THRESHOLD) {
                int size = end - start + 1;
                int left = start + size % 2, right = left + size / 2;
                List<CompareAndSwapTask> casTasks = new ArrayList<>(casThreads);
                for (int i = 0; i < casThreads; i++)
                    casTasks.add(new CompareAndSwapTask(left + i, right + i, end, ascending));
                invokeAll(casTasks);
                invokeAll(
                    new MergeTask(left, right - 1, ascending),
                    new MergeTask(right, end, ascending)
                );
                while (start < end && compare(start, start + 1) > 0 == ascending)
                    swap(start++, start);
            } else if (end - start > 1)
                bitonicMerge(start, end, ascending);
        }
    }

    private class CompareAndSwapTask extends RecursiveAction {
        int left, right, end;
        boolean ascending;

        CompareAndSwapTask(int left, int right, int end, boolean ascending) {
            this.left = left; this.right = right; this.end = end; this.ascending = ascending;
        }

        @Override
        protected void compute() {
            while (right <= end) {
                if (compare(left, right) > 0 == ascending) swap(left, right);
                left += casThreads; right += casThreads;
            }
        }
    }

    private void bitonic(int start, int end, boolean ascending) {
        if (end - start < 1) return;
        int mid = (end + start) / 2;
        bitonic(start, mid, ascending);
        bitonic(mid + 1, end, !ascending);
        if (compare(mid, end) > 0 == ascending)
            bitonicMerge(start, end, ascending);
        else reverse(mid + 1, end);
    }

    private void bitonicMerge(int start, int end, boolean ascending) {
        if (end - start < 1) return;
        int size = end - start + 1;
        int left = start + size % 2, right = left + size / 2;
        for (int i = left, j = right; i < right; i++, j++)
            if (compare(i, j) > 0 == ascending) swap(i, j);
        bitonicMerge(left, right - 1, ascending);
        bitonicMerge(right, end, ascending);
        while (start < end && compare(start, start + 1) > 0 == ascending)
            swap(start++, start);
    }

    public void sort(E[] array, int casThreads) { this.casThreads = casThreads; sort(array); }
    public void sort(E[] array) {
        ForkJoinPool.commonPool().invoke(new BitonicTask(0, initialize(array).length - 1, true));
    }

    public static void main(String[] args) {
        int size;
        try { size = Integer.parseInt(args[0]); }
        catch (Exception e) { size = 0; }
        size = size > 10000 ? size : 10000;

        ParallelSorter<Integer> sorter = new ParallelSorter<>();
        Integer[] array = new Integer[size], copy;

        Random rand = new Random();
        for (int i = 0; i < size; i++) {
            int j = rand.nextInt(i + 1);
            array[i] = array[j]; array[j] = i;
        }

        sorter.sort(Arrays.copyOf(array, 10000)); // JIT warm-up

        for (int i = 1; i <= 16; i++) {
            copy = Arrays.copyOf(array, array.length);
            long t0 = System.nanoTime();
            sorter.sort(copy, i);
            double elapsed = (System.nanoTime() - t0) / 1e6;
            if (i == 1) System.out.printf("Recursion only: %.3f ms%n", elapsed);
            else System.out.printf("%2d CAS threads: %.3f ms%n", i, elapsed);
        }
    }
}