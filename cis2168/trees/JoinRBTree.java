import java.util.*;
import java.util.function.*;
import java.util.stream.*;

/**
 * JoinRBTree — a mutable red-black tree built on SPLIT and JOIN.
 * A CS2 lecture foil to the CLRS red-black tree.
 *
 * The one idea:  expose two primitives and derive everything else from them.
 *
 *   split(T, k) -> (L, found, R)     the PARTITION   (quicksort's divide, keyed not pivoted)
 *   join(L, k, R) -> T               the COMBINE     (rebalances; this is where the work lives)
 *
 * They are inverse for a fixed pivot: split then join reconstructs T. In PivCo-Huffman the
 * tree was *fixed*, so the combine (merge) was a pure interleave with no work to do. A balanced
 * BST's shape carries its performance guarantee, so here the combine has to do real work —
 * rebalance — to restore the invariant. join is the only place balancing logic lives.
 *
 * THE PAYOFF vs CLRS: there is exactly ONE rebalancing routine (join), and the only violation
 * it ever introduces is red-red — the same one ordinary insert already handles. The dreaded
 * delete-side "double black" case analysis simply does not exist here:
 *
 *   add(k)    = { (L, _, R) = split(root, k);  root = join(L, k, R) }
 *   remove(k) = { (L, _, R) = split(root, k);  root = join2(L, R)    }      // join2 = join, no middle key
 *   union / intersect / difference  = split / recurse-on-both-sides / join   // quicksort-shaped
 *
 * Augmentation: each node caches its subtree SIZE — the (N, +) monoid. Because + is associative
 * the cached sums compose through join, giving O(log n) select/rank for free, and making the
 * Spliterator's estimateSize exact so parallel streams split perfectly down the middle.
 *
 * Parallelism: Spliterator.trySplit() IS the divide; the stream's reduce/collect is the combine;
 * it parallelizes precisely when the reducer is associative ("find the monoid, get the log-depth
 * tree"). Run:  tree.parallelStream().reduce(id, associativeOp)
 *
 * HONEST CAVEATS (worth stating to students):
 *  - This is the MUTABLE, single-threaded teaching version. The parallel streaming here is
 *    read-only parallel traversal/reduction. The work-optimal, lock-free PARALLEL union/etc.
 *    from the "Just Join" paper relies on PERSISTENCE (path copying) so disjoint recursive calls
 *    never race. Here the set operations are destructive: they CONSUME their operands.
 *  - join recomputes black-height (O(log n)), so each point op is O(log^2 n). Threading the
 *    black-height through the recursion (as the paper does) restores O(log n); left as an exercise.
 *
 * Algorithm: Blelloch, Ferizovic & Sun, "Just Join for Parallel Ordered Sets" (SPAA 2016).
 */
public class JoinRBTree<K> {

    // red == true.  null == black NIL leaf.
    static final class Node<K> {
        K key; boolean red; Node<K> left, right; int size;
    }

    private final Comparator<K> cmp; // null => natural ordering
    private Node<K> root;

    public JoinRBTree() { this(null); }
    public JoinRBTree(Comparator<K> cmp) { this.cmp = cmp; }

    @SuppressWarnings("unchecked")
    private int cmp(K a, K b) {
        return (cmp != null) ? cmp.compare(a, b) : ((Comparable<K>) a).compareTo(b);
    }

    // ---------- tiny helpers ----------
    static <K> boolean isRed(Node<K> n) { return n != null && n.red; }
    static <K> int size(Node<K> n)      { return n == null ? 0 : n.size; }
    static <K> void upd(Node<K> n)      { n.size = 1 + size(n.left) + size(n.right); }

    private static <K> Node<K> node(boolean red, Node<K> l, K key, Node<K> r) {
        Node<K> n = new Node<>();
        n.red = red; n.left = l; n.key = key; n.right = r;
        n.size = 1 + size(l) + size(r);
        return n;
    }

    /** Black height: count black nodes down the left spine (all paths equal in a valid tree). */
    static <K> int blackHeight(Node<K> n) {
        int h = 0;
        for (Node<K> x = n; x != null; x = x.left) if (!x.red) h++;
        return h;
    }

    private static <K> Node<K> rotateLeft(Node<K> h)  { Node<K> x = h.right; h.right = x.left;  x.left  = h; upd(h); upd(x); return x; }
    private static <K> Node<K> rotateRight(Node<K> h) { Node<K> x = h.left;  h.left  = x.right; x.right = h; upd(h); upd(x); return x; }

    // ===================== JOIN: the one hard function =====================
    /** Combine L < k < R into one balanced red-black tree. */
    private Node<K> join(Node<K> L, K k, Node<K> R) {
        if (L != null) L.red = false;          // normalize to black roots so black-height is well-defined
        if (R != null) R.red = false;
        int hl = blackHeight(L), hr = blackHeight(R);
        Node<K> t;
        if      (hl > hr) t = joinRight(L, k, R);   // L taller: descend L's right spine
        else if (hr > hl) t = joinLeft (R, k, L);   // R taller: descend R's left spine
        else              t = node(true, L, k, R);  // equal: a red node splices them
        t.red = false;                              // root is always black
        return t;
    }

    // L is taller; walk down its right spine to a black node whose black-height matches R,
    // splice in a red node there, then fix any red-red on the way back up (the ONLY violation).
    private Node<K> joinRight(Node<K> L, K k, Node<K> R) {
        if (!isRed(L.right) && blackHeight(L.right) == blackHeight(R)) {
            L.right = node(true, L.right, k, R); upd(L);
            return L;
        }
        L.right = joinRight(L.right, k, R); upd(L);
        if (!L.red && isRed(L.right) && isRed(L.right.right)) {
            L.right.right.red = false;
            L = rotateLeft(L);
        }
        return L;
    }

    private Node<K> joinLeft(Node<K> R, K k, Node<K> L) { // mirror image
        if (!isRed(R.left) && blackHeight(R.left) == blackHeight(L)) {
            R.left = node(true, L, k, R.left); upd(R);
            return R;
        }
        R.left = joinLeft(R.left, k, L); upd(R);
        if (!R.red && isRed(R.left) && isRed(R.left.left)) {
            R.left.left.red = false;
            R = rotateRight(R);
        }
        return R;
    }

    // ===================== SPLIT and JOIN2 =====================
    static final class Split<K> { Node<K> l; boolean found; Node<K> r; }

    /** Partition t into (keys < k, whether k was present, keys > k). */
    private Split<K> split(Node<K> t, K k) {
        Split<K> s = new Split<>();
        if (t == null) return s;
        int c = cmp(k, t.key);
        if (c == 0) { s.l = t.left; s.found = true; s.r = t.right; }
        else if (c < 0) { Split<K> z = split(t.left,  k); s.l = z.l; s.found = z.found; s.r = join(z.r, t.key, t.right); }
        else            { Split<K> z = split(t.right, k); s.l = join(t.left, t.key, z.l); s.found = z.found; s.r = z.r; }
        return s;
    }

    private K lastKey;
    private Node<K> splitLast(Node<K> t) { // t != null; sets lastKey; returns the rest
        if (t.right == null) { lastKey = t.key; return t.left; }
        return join(t.left, t.key, splitLast(t.right));
    }
    /** join with no separating key (e.g. for delete): combine two trees, all of L's keys < all of R's. */
    private Node<K> join2(Node<K> l, Node<K> r) {
        if (l == null) return r;
        Node<K> rest = splitLast(l);
        return join(rest, lastKey, r);
    }

    // ===================== public point operations (all derived) =====================
    public boolean add(K k)    { Split<K> s = split(root, k); root = join(s.l, k, s.r); return !s.found; }
    public boolean remove(K k) { Split<K> s = split(root, k); root = join2(s.l, s.r); if (root != null) root.red = false; return s.found; }
    public boolean contains(K k) {
        for (Node<K> n = root; n != null; ) { int c = cmp(k, n.key); if (c == 0) return true; n = c < 0 ? n.left : n.right; }
        return false;
    }
    public int size() { return size(root); }
    public boolean isEmpty() { return root == null; }

    // ===================== set algebra (split / recurse / join) =====================
    // NOTE: destructive — these consume A and B. Do not reuse the operands afterward.
    public static <K> JoinRBTree<K> union(JoinRBTree<K> A, JoinRBTree<K> B)      { return wrap(A.cmp, A.unionN(A.root, B.root)); }
    public static <K> JoinRBTree<K> intersect(JoinRBTree<K> A, JoinRBTree<K> B)  { return wrap(A.cmp, A.interN(A.root, B.root)); }
    public static <K> JoinRBTree<K> difference(JoinRBTree<K> A, JoinRBTree<K> B) { return wrap(A.cmp, A.diffN(A.root, B.root)); }

    private static <K> JoinRBTree<K> wrap(Comparator<K> cmp, Node<K> root) {
        JoinRBTree<K> t = new JoinRBTree<>(cmp);
        if (root != null) root.red = false;
        t.root = root;
        return t;
    }
    // The two recursive calls are independent — this is exactly where fork/join parallelism
    // would go in the persistent version.
    private Node<K> unionN(Node<K> a, Node<K> b) {
        if (a == null) return b;
        if (b == null) return a;
        Split<K> s = split(b, a.key);
        return join(unionN(a.left, s.l), a.key, unionN(a.right, s.r));
    }
    private Node<K> interN(Node<K> a, Node<K> b) {
        if (a == null || b == null) return null;
        Split<K> s = split(b, a.key);
        Node<K> L = interN(a.left, s.l), R = interN(a.right, s.r);
        return s.found ? join(L, a.key, R) : join2(L, R);
    }
    private Node<K> diffN(Node<K> a, Node<K> b) { // a \ b
        if (a == null) return null;
        if (b == null) return a;
        Split<K> s = split(a, b.key);            // split A by B's pivot; b.key excluded from result
        return join2(diffN(s.l, b.left), diffN(s.r, b.right));
    }

    // ===================== O(n) bulk build from sorted, de-duplicated keys =====================
    public static <K> JoinRBTree<K> fromSorted(K[] keys, Comparator<K> cmp) {
        JoinRBTree<K> t = new JoinRBTree<>(cmp);
        if (keys.length == 0) return t;
        int maxDepth = 31 - Integer.numberOfLeadingZeros(keys.length); // floor(log2 n)
        t.root = build(keys, 0, keys.length - 1, 0, maxDepth);
        t.root.red = false;
        return t;
    }
    // Perfectly balanced; color only the deepest level red so every root->NIL path has equal black count.
    private static <K> Node<K> build(K[] a, int lo, int hi, int depth, int maxDepth) {
        if (lo > hi) return null;
        int mid = (lo + hi) >>> 1;
        Node<K> n = new Node<>();
        n.key = a[mid];
        n.left  = build(a, lo, mid - 1, depth + 1, maxDepth);
        n.right = build(a, mid + 1, hi, depth + 1, maxDepth);
        n.red = (depth == maxDepth);
        upd(n);
        return n;
    }

    // ===================== order statistics (the size monoid in action) =====================
    public K select(int i) {                 // 0-based: the i-th smallest key
        Node<K> n = root;
        while (n != null) {
            int ls = size(n.left);
            if (i < ls) n = n.left;
            else if (i > ls) { i -= ls + 1; n = n.right; }
            else return n.key;
        }
        throw new IndexOutOfBoundsException();
    }
    public int rank(K k) {                    // number of keys strictly less than k
        int r = 0;
        for (Node<K> n = root; n != null; ) {
            int c = cmp(k, n.key);
            if (c <= 0) n = n.left;
            else { r += size(n.left) + 1; n = n.right; }
        }
        return r;
    }

    public List<K> toList() { List<K> out = new ArrayList<>(); inorder(root, out); return out; }
    private void inorder(Node<K> n, List<K> out) { if (n == null) return; inorder(n.left, out); out.add(n.key); inorder(n.right, out); }

    // ===================== splitting Spliterator =====================
    // trySplit() = the divide; reduce/collect = the combine; parallel iff the reducer is associative.
    public Spliterator<K> spliterator() { return new TreeSpliterator<>(root, cmp, 0, size(root)); }
    public Stream<K> stream()           { return StreamSupport.stream(spliterator(), false); }
    public Stream<K> parallelStream()   { return StreamSupport.stream(spliterator(), true); }

    static final class TreeSpliterator<K> implements Spliterator<K> {
        // Covers the in-order index range [lo, hi). Cached subtree sizes let trySplit cut at the
        // median index -> exact 50/50 splits with no scan.
        private final Node<K> root;
        private final Comparator<K> cmp;
        private int lo, hi;
        private ArrayDeque<Node<K>> cursor; // lazily positioned at index `lo`

        TreeSpliterator(Node<K> root, Comparator<K> cmp, int lo, int hi) { this.root = root; this.cmp = cmp; this.lo = lo; this.hi = hi; }

        private void initCursor() { // descend to the lo-th element using subtree sizes
            cursor = new ArrayDeque<>();
            Node<K> n = root; int idx = lo;
            while (n != null) {
                int ls = size(n.left);
                if (idx < ls) { cursor.push(n); n = n.left; }
                else if (idx == ls) { cursor.push(n); break; }
                else { idx -= ls + 1; n = n.right; }
            }
        }
        @Override public boolean tryAdvance(Consumer<? super K> action) {
            if (lo >= hi) return false;
            if (cursor == null) initCursor();
            Node<K> n = cursor.pop();
            action.accept(n.key);
            for (Node<K> x = n.right; x != null; x = x.left) cursor.push(x);
            lo++;
            return true;
        }
        @Override public Spliterator<K> trySplit() {
            if (hi - lo <= 1) return null;
            int mid = (lo + hi) >>> 1;
            Spliterator<K> prefix = new TreeSpliterator<>(root, cmp, lo, mid); // ORDERED: return the prefix
            this.lo = mid; this.cursor = null;                                 // this becomes the suffix
            return prefix;
        }
        @Override public long estimateSize() { return hi - lo; }
        @Override public int characteristics() { return ORDERED | SORTED | DISTINCT | SIZED | SUBSIZED | NONNULL; }
        @Override public Comparator<? super K> getComparator() { return cmp; } // null => natural ordering
    }

    // ===================== self-test (run: java JoinRBTree.java) =====================
    public static void main(String[] args) {
        Random rnd = new Random(2026);

        // (a) randomized add/remove vs TreeSet, checking RB invariants throughout
        for (int trial = 0; trial < 120; trial++) {
            JoinRBTree<Integer> t = new JoinRBTree<>(); TreeSet<Integer> ref = new TreeSet<>();
            for (int i = 0; i < 600; i++) {
                int k = rnd.nextInt(200);
                if (rnd.nextBoolean()) { t.add(k); ref.add(k); } else { t.remove(k); ref.remove(k); }
                if ((i & 31) == 0) validate(t, ref, "add/remove");
            }
            validate(t, ref, "add/remove final");
        }
        System.out.println("PASS  add/remove + RB invariants");

        // (b) fromSorted for every size 0..512, plus select/rank
        for (int n = 0; n <= 512; n++) {
            Integer[] a = new Integer[n];
            for (int i = 0; i < n; i++) a[i] = 5 * i + 1;
            JoinRBTree<Integer> t = JoinRBTree.fromSorted(a, null);
            validate(t, new TreeSet<>(Arrays.asList(a)), "fromSorted");
            for (int i = 0; i < n; i++) if (!t.select(i).equals(a[i]) || t.rank(a[i]) != i) throw new AssertionError("rank/select");
        }
        System.out.println("PASS  fromSorted (0..512) + select/rank");

        // (c) union / intersect / difference vs TreeSet
        for (int trial = 0; trial < 300; trial++) {
            TreeSet<Integer> ra = randomSet(rnd, 120, 300), rb = randomSet(rnd, 120, 300);
            JoinRBTree<Integer> A = of(ra), B = of(rb);
            TreeSet<Integer> exp = new TreeSet<>(ra);
            switch (trial % 3) {
                case 0 -> { exp.addAll(rb);    validate(union(A, B),      exp, "union"); }
                case 1 -> { exp.retainAll(rb); validate(intersect(A, B),  exp, "intersect"); }
                default -> { exp.removeAll(rb); validate(difference(A, B), exp, "difference"); }
            }
        }
        System.out.println("PASS  union/intersect/difference");

        // (d) parallel streams: balanced split, ordered collect, associative reduce — on an irregular tree
        JoinRBTree<Integer> t = new JoinRBTree<>(); TreeSet<Integer> ref = new TreeSet<>();
        for (int i = 0; i < 50000; i++) { int k = rnd.nextInt(40000); if (rnd.nextInt(3) < 2) { t.add(k); ref.add(k); } else { t.remove(k); ref.remove(k); } }
        if (t.parallelStream().count() != ref.size()) throw new AssertionError("count");
        if (!t.parallelStream().collect(Collectors.toList()).equals(new ArrayList<>(ref))) throw new AssertionError("ordered parallel collect");
        long ps = t.parallelStream().mapToLong(Integer::longValue).sum(), ss = 0; for (int x : ref) ss += x;
        if (ps != ss) throw new AssertionError("parallel sum");
        Spliterator<Integer> sp = t.spliterator(); long tot = sp.estimateSize();
        Spliterator<Integer> pre = sp.trySplit();
        System.out.printf("PASS  parallel streams (median split %d + %d = %d)%n", pre.estimateSize(), sp.estimateSize(), tot);

        System.out.println("ALL TESTS PASSED");
    }

    // ---- test utilities ----
    private static TreeSet<Integer> randomSet(Random r, int n, int range) {
        TreeSet<Integer> s = new TreeSet<>(); for (int i = 0; i < n; i++) s.add(r.nextInt(range)); return s;
    }
    private static JoinRBTree<Integer> of(Collection<Integer> c) {
        JoinRBTree<Integer> t = new JoinRBTree<>(); for (int x : c) t.add(x); return t;
    }
    private static void validate(JoinRBTree<Integer> t, TreeSet<Integer> ref, String where) {
        if (t.root != null && t.root.red) throw new AssertionError("red root @" + where);
        checkRB(t.root);
        if (!t.toList().equals(new ArrayList<>(ref))) throw new AssertionError("contents @" + where);
    }
    private static int checkRB(Node<Integer> n) { // returns black-height; throws on any violation
        if (n == null) return 1;
        if (isRed(n) && (isRed(n.left) || isRed(n.right))) throw new AssertionError("red-red");
        if (n.size != 1 + size(n.left) + size(n.right)) throw new AssertionError("bad size cache");
        if (n.left  != null && n.left.key  >= n.key) throw new AssertionError("BST order (left)");
        if (n.right != null && n.right.key <= n.key) throw new AssertionError("BST order (right)");
        int bl = checkRB(n.left), br = checkRB(n.right);
        if (bl != br) throw new AssertionError("black-height mismatch");
        return bl + (n.red ? 0 : 1);
    }
}