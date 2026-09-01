import java.util.*;

public final class Huffman<S> {
    /** A node of the code tree. Weight = total frequency underneath. */
    public sealed interface Node<S> permits Leaf, Branch {
        long weight();
    }

    /** A symbol occupies a leaf; its code is the path from the root. */
    public record Leaf<S>(S symbol, long weight) implements Node<S> {
        public Leaf {
            if (weight <= 0) throw new IllegalArgumentException(
                "frequency must be positive: " + symbol + " -> " + weight);
        }
    }

    /**
     * An internal node.
     */
    public record Branch<S>(Node<S> left, Node<S> right, long weight) implements Node<S> {
        /** The constructor you actually call; the weight computes itself. */
        public Branch(Node<S> left, Node<S> right) {
            this(left, right, left.weight() + right.weight());
        }
    }

    // ------------------------------------------------------------------
    // Bits: the mutable accumulator. Append-only builder + random read.
    // ------------------------------------------------------------------

    /**
     * A growable bit sequence over long[]. Why not java.util.BitSet?
     * BitSet has no logical length -- trailing zeros are invisible to
     * it -- and a Huffman stream absolutely cares about its exact
     * length. That leak is the whole reason this class exists.
     */
    public static final class Bits {
        private long[] words = new long[1];
        private int size = 0;

        public int size() { return size; }

        public void append(boolean bit) {
            if (size == words.length * 64)
                words = Arrays.copyOf(words, words.length * 2);   // amortized O(1)
            long mask = 1L << (size & 63);
            if (bit) words[size >>> 6] |=  mask;
            else     words[size >>> 6] &= ~mask;   // clear any stale bit: the
                // table builder backtracks (size--), so this slot may still
                // hold a 1 from a previous, longer prefix
            size++;
        }

        public void appendAll(Bits other) {
            for (int i = 0; i < other.size; i++) append(other.get(i));
        }

        public boolean get(int i) {
            Objects.checkIndex(i, size);
            return (words[i >>> 6] & (1L << (i & 63))) != 0;
        }

        @Override public String toString() {
            StringBuilder sb = new StringBuilder(size);
            for (int i = 0; i < size; i++) sb.append(get(i) ? '1' : '0');
            return sb.toString();
        }
    }

    // ------------------------------------------------------------------
    // Construction: greedy merge with deterministic tie-breaking.
    // ------------------------------------------------------------------

    private final Node<S> root;
    private final Map<S, Bits> table;   // symbol -> its code, staged at build

    private Huffman(Node<S> root) {
        this.root = root;
        this.table = new HashMap<>();
        // Single-symbol alphabet: the tree is one leaf and the path to it
        // is empty, which would encode everything in zero bits. Convention:
        // that symbol costs one bit, "0".
        switch (root) {
            case Leaf<S>(S symbol, var w) -> {
                Bits b = new Bits();
                b.append(false);
                table.put(symbol, b);
            }
            case Branch<S> top -> fillTable(top, new Bits());
        }
    }

    /** Walk the tree once, recording the path (0 = left, 1 = right). */
    private void fillTable(Node<S> node, Bits path) {
        switch (node) {
            case Leaf<S>(S symbol, var w) -> {
                Bits code = new Bits();
                code.appendAll(path);
                table.put(symbol, code);
            }
            case Branch<S>(var left, var right, var w) -> {
                path.append(false); fillTable(left, path);  undo(path);
                path.append(true);  fillTable(right, path); undo(path);
            }
        }
    }

    private static void undo(Bits path) { path.size--; }  // backtrack one bit

    /** PQ element: the node plus a tick that fixes the order of ties. */
    private record Ranked<S>(Node<S> node, long tick) {}

    /**
     * Build a codec from symbol frequencies. Frequencies must be positive.
     * Ties in the greedy merge are broken by arrival order, so the tree
     * is a deterministic function of (frequencies, iteration order).
     */
    public static <S> Huffman<S> of(Map<S, Long> frequencies) {
        if (frequencies.isEmpty())
            throw new IllegalArgumentException("no symbols");

        Comparator<Ranked<S>> byWeightThenAge =
            Comparator.comparingLong((Ranked<S> r) -> r.node().weight())
                      .thenComparingLong(Ranked::tick);

        PriorityQueue<Ranked<S>> pq = new PriorityQueue<>(byWeightThenAge);
        long tick = 0;
        for (var e : frequencies.entrySet())
            pq.add(new Ranked<>(new Leaf<>(e.getKey(), e.getValue()), tick++));

        while (pq.size() > 1) {
            Node<S> a = pq.poll().node();
            Node<S> b = pq.poll().node();
            pq.add(new Ranked<>(new Branch<>(a, b), tick++));
        }
        return new Huffman<>(pq.poll().node());
    }

    /**
     * Convenience factory for text: frequencies in first-appearance
     * order (LinkedHashMap), so the whole pipeline is reproducible
     * from the corpus string alone.
     */
    public static Huffman<Character> ofText(CharSequence corpus) {
        Map<Character, Long> freq = new LinkedHashMap<>();
        for (int i = 0; i < corpus.length(); i++)
            freq.merge(corpus.charAt(i), 1L, Long::sum);
        return of(freq);
    }

    // ------------------------------------------------------------------
    // The codec proper.
    // ------------------------------------------------------------------

    /** Encode: one table lookup and one bit-append run per symbol. */
    public Bits encode(Iterable<? extends S> symbols) {
        Bits out = new Bits();
        for (S s : symbols) {
            Bits code = table.get(s);
            if (code == null)
                throw new NoSuchElementException("symbol not in alphabet: " + s);
            out.appendAll(code);
        }
        return out;
    }

    /** Decode: restart at the root after every leaf. */
    public List<S> decode(Bits bits) {
        List<S> out = new ArrayList<>();
        switch (root) {
            case Leaf<S>(S symbol, var w) -> {           // one-symbol alphabet
                for (int i = 0; i < bits.size(); i++) {
                    if (bits.get(i)) throw new IllegalArgumentException(
                        "invalid bit at " + i + " for single-symbol code");
                    out.add(symbol);
                }
            }
            case Branch<S> top -> {
                // Between bits we only ever stand on an internal node, so the
                // cursor's *type* is Branch, not Node -- the impossible state
                // (resting on a leaf) is unrepresentable, no dead switch arm.
                Branch<S> cur = top;
                for (int i = 0; i < bits.size(); i++) {
                    Node<S> next = bits.get(i) ? cur.right() : cur.left();
                    cur = switch (next) {
                        case Leaf<S>(S symbol, var w) -> { out.add(symbol); yield top; }
                        case Branch<S> b -> b;
                    };
                }
                if (cur != top)
                    throw new IllegalArgumentException("truncated code: ended mid-symbol");
            }
        }
        return out;
    }

    /**
     * Text as symbols. A static helper, not an instance method: an
     * encode(CharSequence) on Huffman&lt;S&gt; would only be valid when
     * S = Character, and the type system can't say that about an
     * instance method without a cast that lies. (Same reason create()
     * on your BSTree is a bounded *static* factory: statics get their
     * own type constraints; instance members are stuck with the class's.)
     */
    public static List<Character> chars(CharSequence text) {
        List<Character> syms = new ArrayList<>(text.length());
        for (int i = 0; i < text.length(); i++) syms.add(text.charAt(i));
        return syms;
    }

    // ------------------------------------------------------------------
    // Read-only views for inspection, printing, grading.
    // ------------------------------------------------------------------

    public Node<S> root() { return root; }

    /** Codes as strings of 0/1, in no particular order. */
    public Map<S, String> codeTable() {
        Map<S, String> view = new LinkedHashMap<>();
        table.forEach((s, b) -> view.put(s, b.toString()));
        return Collections.unmodifiableMap(view);
    }

    /** Sideways ASCII rendering of the tree (root at left, right subtree on top). */
    public String render() {
        StringBuilder sb = new StringBuilder();
        render(root, "", "", sb);
        return sb.toString();
    }

    private void render(Node<S> node, String prefix, String childPrefix, StringBuilder sb) {
        switch (node) {
            case Leaf<S>(S symbol, long w) ->
                sb.append(prefix).append(symbol).append(" (").append(w).append(")\n");
            case Branch<S>(var left, var right, long w) -> {
                render(right, childPrefix + "  1:", childPrefix + "    ", sb);
                sb.append(prefix).append("* (").append(w).append(")\n");
                render(left, childPrefix + "  0:", childPrefix + "    ", sb);
            }
        }
    }

    // ==================================================================
    // Embedded demo + self-checks (dev copy).
    // ==================================================================

    public static void main(String[] args) {
        demo();
        testRoundTripText();
        testSingleSymbol();
        testTruncated();
        testUnknownSymbol();
        testKraftAndOptimality();
        testDeterminism();
        testGenericSymbols();
        System.out.println("\nall checks passed");
    }

    static void demo() {
        String corpus = "this is an example of a huffman tree";
        Huffman<Character> h = Huffman.ofText(corpus);
        System.out.println(h.render());
        System.out.println("codes: " + h.codeTable());
        Bits enc = h.encode(chars(corpus));
        System.out.printf("original: %d bits (as 8-bit chars); encoded: %d bits%n",
            corpus.length() * 8, enc.size());
    }

    static void check(boolean cond, String what) {
        if (!cond) throw new AssertionError("FAILED: " + what);
        System.out.println("ok: " + what);
    }

    static void testRoundTripText() {
        Random rnd = new Random(42);
        for (int trial = 0; trial < 200; trial++) {
            int alpha = 1 + rnd.nextInt(26);
            int len = 1 + rnd.nextInt(500);
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < len; i++) sb.append((char) ('a' + rnd.nextInt(alpha)));
            String s = sb.toString();
            Huffman<Character> h = Huffman.ofText(s);
            List<Character> back = h.decode(h.encode(chars(s)));
            StringBuilder rt = new StringBuilder();
            back.forEach(rt::append);
            if (!rt.toString().equals(s))
                throw new AssertionError("round trip failed on: " + s);
        }
        check(true, "200 random round trips");
    }

    static void testSingleSymbol() {
        Huffman<Character> h = Huffman.ofText("aaaa");
        Bits e = h.encode(chars("aaaa"));
        check(e.size() == 4 && h.decode(e).size() == 4, "single-symbol alphabet costs 1 bit/symbol");
    }

    static void testTruncated() {
        // Note: truncation is only *detectable* when the cut lands mid-code.
        // (Chopping a whole trailing symbol yields a shorter, legal stream --
        // Huffman has no framing.) 'a' is rarest in this corpus, so its code
        // is >= 2 bits; cutting 1 bit off a lone 'a' must end mid-symbol.
        Huffman<Character> h = Huffman.ofText("abbccc");
        Bits e = h.encode(chars("a"));
        Bits cut = new Bits();
        for (int i = 0; i < e.size() - 1; i++) cut.append(e.get(i));
        boolean threw = false;
        try { h.decode(cut); } catch (IllegalArgumentException ex) { threw = true; }
        check(threw, "truncated stream rejected");
    }

    static void testUnknownSymbol() {
        Huffman<Character> h = Huffman.ofText("aabb");
        boolean threw = false;
        try { h.encode(chars("abz")); } catch (NoSuchElementException ex) { threw = true; }
        check(threw, "unknown symbol rejected");
    }

    static void testKraftAndOptimality() {
        String corpus = "the quick brown fox jumps over the lazy dog and the dog barks back";
        Huffman<Character> h = Huffman.ofText(corpus);

        // Kraft equality: a full binary code tree satisfies sum 2^-len == 1.
        double kraft = h.codeTable().values().stream()
            .mapToDouble(c -> Math.pow(2, -c.length())).sum();
        check(Math.abs(kraft - 1.0) < 1e-9, "Kraft sum == 1 (full tree)");

        // Encoded length == weighted path length == sum over freq * depth.
        Map<Character, Long> freq = new LinkedHashMap<>();
        for (char c : corpus.toCharArray()) freq.merge(c, 1L, Long::sum);
        long wpl = freq.entrySet().stream()
            .mapToLong(e -> e.getValue() * h.codeTable().get(e.getKey()).length()).sum();
        check(h.encode(chars(corpus)).size() == wpl, "encoded length == weighted path length");

        // Shannon bracket: H <= avg code length < H + 1.
        long n = corpus.length();
        double entropy = freq.values().stream()
            .mapToDouble(f -> { double p = (double) f / n; return -p * (Math.log(p) / Math.log(2)); })
            .sum();
        double avg = (double) wpl / n;
        check(entropy <= avg + 1e-9 && avg < entropy + 1, "H <= avg < H + 1");
    }

    static void testDeterminism() {
        String corpus = "mississippi river banks";  // plenty of ties
        String t1 = Huffman.ofText(corpus).codeTable().toString();
        String t2 = Huffman.ofText(corpus).codeTable().toString();
        check(t1.equals(t2), "same corpus -> same tree (tie-break by tick)");
    }

    static void testGenericSymbols() {
        // The codec never needed Comparable or Character: any symbol type works.
        Map<String, Long> freq = new LinkedHashMap<>();
        freq.put("GET", 70L); freq.put("POST", 20L); freq.put("PUT", 7L); freq.put("DELETE", 3L);
        Huffman<String> h = Huffman.of(freq);
        List<String> msg = List.of("GET", "GET", "POST", "DELETE", "GET", "PUT");
        check(h.decode(h.encode(msg)).equals(msg), "generic symbol type round trip");
        check(h.codeTable().get("GET").length() == 1, "most frequent symbol gets shortest code");
    }
}