import java.util.concurrent.atomic.AtomicLong;

public final class Graph {
    private Graph() {}   // no instances, just a container for Vertex and Edge

    /// A vertex is nothing but identity: it equals only itself.
    /// No data, no name — a name is just one more decoration (a Map<Vertex, String>
    /// living outside the graph). Instances are created only by Digraph.Builder,
    /// so a vertex cannot be forged and always belongs to the graph that made it.
    public final class Vertex {
        private static final AtomicLong IDS = new AtomicLong();
        private final long id = IDS.incrementAndGet();
        Vertex() {}   // package-private: only the builder creates identities
        @Override public String toString() { return "v" + id; }
    }

    /// An edge is an identity with two endpoints — and nothing else.
    /// Identity (not value) equality means two parallel edges u -> v are distinct
    /// objects and therefore distinct map keys: the base layer is a multigraph,
    /// and a simple graph is merely the discipline of not adding duplicates.
    public record Edge(Vertex from, Vertex to) {
        public Edge {
            if (Objects.isNull(from) || Objects.isNull(to))
                throw new IllegalArgumentException("Endpoints cannot be null");
        }

        @Override public String toString() { return from + "->" + to; }
    }
}