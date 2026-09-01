import java.util.*;

/// Pure structure: vertex identities, edge identities, incidence. No payload
/// anywhere. All data — names, weights, grades, coordinates — attaches from
/// the outside as maps keyed by these identities, and such decorations layer
/// freely and independently without this interface ever changing.
///
/// Note there are no type parameters left: once labels move out of the graph,
/// V and E have nothing to range over.
public interface Digraph {

    Set<Vertex> vertices();
    Set<Edge> edges();

    /// Out-edges of v in insertion order; parallel edges each appear.
    /// @throws IllegalArgumentException if v is not a vertex of this graph.
    List<Edge> outEdges(Vertex v);

    default Set<Vertex> successors(Vertex v) {
        var s = new LinkedHashSet<Vertex>();
        for (var e : outEdges(v)) s.add(e.to());
        return Collections.unmodifiableSet(s);
    }

    default boolean contains(Vertex v) { return vertices().contains(v); }
    default int order() { return vertices().size(); }
    default int size()  { return edges().size(); }

    static Builder builder() { return new Builder(); }

    /// Mutable codata, confined to construction time.
    final class Builder {
        private final Map<Vertex, List<Edge>> out = new LinkedHashMap<>();

        /// Mints a fresh identity. Hold on to the return value —
        /// it is the only handle to this vertex you will ever have.
        public Vertex vertex() {
            var v = new Vertex();
            out.put(v, new ArrayList<>());
            return v;
        }

        /// Both endpoints must have been minted by this builder.
        /// Calling twice with the same endpoints creates a parallel edge.
        public Edge edge(Vertex from, Vertex to) {
            require(from);
            require(to);
            var e = new Edge(from, to);
            out.get(from).add(e);
            return e;
        }

        private void require(Vertex v) {
            if (!out.containsKey(Objects.requireNonNull(v)))
                throw new IllegalArgumentException(v + " was not minted by this builder");
        }

        public Digraph build() { return new MapDigraph(out); }
    }
}

/// Hidden implementation; deep unmodifiable copy, dangling edges rejected.
record MapDigraph(Map<Vertex, List<Edge>> out) implements Digraph {

    MapDigraph {
        var copy = new LinkedHashMap<Vertex, List<Edge>>();
        out.forEach((v, es) -> copy.put(Objects.requireNonNull(v), List.copyOf(es)));
        for (var es : copy.values())
            for (var e : es)
                if (!copy.containsKey(e.from()) || !copy.containsKey(e.to()))
                    throw new IllegalArgumentException("dangling edge " + e);
        out = Collections.unmodifiableMap(copy);
    }

    @Override public Set<Vertex> vertices() { return out.keySet(); }

    @Override public Set<Edge> edges() {
        var all = new LinkedHashSet<Edge>();
        out.values().forEach(all::addAll);
        return Collections.unmodifiableSet(all);
    }

    @Override public List<Edge> outEdges(Vertex v) {
        var es = out.get(Objects.requireNonNull(v));
        if (es == null) throw new IllegalArgumentException("unknown vertex: " + v);
        return es;
    }
}