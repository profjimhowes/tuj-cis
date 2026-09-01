import java.util.*;

/**
 * Tiny neural network / automatic differentiation demo.
 *
 * The computation is represented as a directed acyclic graph (DAG):
 *   - forward evaluation follows topological order
 *   - backpropagation follows reverse topological order
 *
 * The example trains a 2-2-1 tanh network to learn XOR.
 */
public class NeuralNetDAG {

    // ---------- Computation graph nodes ----------

    static abstract class Node {
        private static int nextId = 0;

        final int id = nextId++;
        final String name;
        final List<Node> inputs;
        double value;
        double grad;

        Node(String name, Node... inputs) {
            this.name = name;
            this.inputs = List.of(inputs);
        }

        abstract void forward();

        /**
         * Add this node's contribution to each input's gradient.
         * Gradients are accumulated because a node may feed multiple children.
         */
        abstract void backward();

        @Override
        public String toString() {
            return name;
        }
    }

    static final class Input extends Node {
        Input(String name) {
            super(name);
        }

        void set(double value) {
            this.value = value;
        }

        @Override void forward() { /* supplied externally */ }
        @Override void backward() { /* stop here */ }
    }

    static final class Parameter extends Node {
        Parameter(String name, double value) {
            super(name);
            this.value = value;
        }

        @Override void forward() { /* retained between examples */ }
        @Override void backward() { /* leaf; grad is used by optimizer */ }
    }

    static final class Add extends Node {
        Add(Node a, Node b) {
            super("add" + nextOperationId(), a, b);
        }

        @Override void forward() {
            value = inputs.get(0).value + inputs.get(1).value;
        }

        @Override void backward() {
            inputs.get(0).grad += grad;
            inputs.get(1).grad += grad;
        }
    }

    static final class Multiply extends Node {
        Multiply(Node a, Node b) {
            super("mul" + nextOperationId(), a, b);
        }

        @Override void forward() {
            value = inputs.get(0).value * inputs.get(1).value;
        }

        @Override void backward() {
            Node a = inputs.get(0);
            Node b = inputs.get(1);
            a.grad += grad * b.value;
            b.grad += grad * a.value;
        }
    }

    static final class Tanh extends Node {
        Tanh(Node x) {
            super("tanh" + nextOperationId(), x);
        }

        @Override void forward() {
            value = Math.tanh(inputs.get(0).value);
        }

        @Override void backward() {
            // d/dx tanh(x) = 1 - tanh(x)^2
            inputs.get(0).grad += grad * (1.0 - value * value);
        }
    }

    static final class SquaredError extends Node {
        SquaredError(Node prediction, Node target) {
            super("loss", prediction, target);
        }

        @Override void forward() {
            double error = inputs.get(0).value - inputs.get(1).value;
            value = 0.5 * error * error;
        }

        @Override void backward() {
            Node prediction = inputs.get(0);
            Node target = inputs.get(1);
            double error = prediction.value - target.value;
            prediction.grad += grad * error;
            target.grad -= grad * error;
        }
    }

    private static int operationId = 0;
    private static int nextOperationId() {
        return operationId++;
    }

    // Small expression-building helpers.
    static Node add(Node a, Node b) { return new Add(a, b); }
    static Node mul(Node a, Node b) { return new Multiply(a, b); }
    static Node tanh(Node x)        { return new Tanh(x); }

    /** w1*x1 + w2*x2 + b */
    static Node neuron(Node x1, Node x2,
                       Parameter w1, Parameter w2, Parameter b) {
        return add(add(mul(w1, x1), mul(w2, x2)), b);
    }

    // ---------- DAG execution ----------

    static final class Graph {
        final Node output;
        final List<Node> topologicalOrder;
        final List<Parameter> parameters;

        Graph(Node output) {
            this.output = output;
            Set<Node> nodes = collectReachableNodes(output);
            this.topologicalOrder = topologicalSort(nodes);
            this.parameters = topologicalOrder.stream()
                    .filter(n -> n instanceof Parameter)
                    .map(n -> (Parameter) n)
                    .toList();
        }

        double forward() {
            for (Node node : topologicalOrder) {
                node.forward();
            }
            return output.value;
        }

        void backward() {
            // Clear gradients from the previous training example.
            for (Node node : topologicalOrder) {
                node.grad = 0.0;
            }

            // d(output)/d(output) = 1
            output.grad = 1.0;

            // Reverse topological order applies the chain rule from output to leaves.
            for (int i = topologicalOrder.size() - 1; i >= 0; i--) {
                topologicalOrder.get(i).backward();
            }
        }

        void step(double learningRate) {
            for (Parameter parameter : parameters) {
                parameter.value -= learningRate * parameter.grad;
            }
        }

        void printTopologicalOrder() {
            System.out.println("Topological order:");
            for (int i = 0; i < topologicalOrder.size(); i++) {
                System.out.printf("%2d. %s%n", i + 1, topologicalOrder.get(i));
            }
        }

        private static Set<Node> collectReachableNodes(Node output) {
            Set<Node> visited = new LinkedHashSet<>();
            Deque<Node> stack = new ArrayDeque<>();
            stack.push(output);

            while (!stack.isEmpty()) {
                Node node = stack.pop();
                if (visited.add(node)) {
                    for (Node input : node.inputs) {
                        stack.push(input);
                    }
                }
            }
            return visited;
        }

        /** Kahn's algorithm, with edges input -> operation. */
        private static List<Node> topologicalSort(Set<Node> nodes) {
            Map<Node, Integer> indegree = new LinkedHashMap<>();
            Map<Node, List<Node>> children = new LinkedHashMap<>();

            for (Node node : nodes) {
                indegree.put(node, node.inputs.size());
                children.put(node, new ArrayList<>());
            }

            for (Node node : nodes) {
                for (Node input : node.inputs) {
                    children.get(input).add(node);
                }
            }

            Deque<Node> ready = new ArrayDeque<>();
            for (Node node : nodes) {
                if (indegree.get(node) == 0) {
                    ready.addLast(node);
                }
            }

            List<Node> order = new ArrayList<>();
            while (!ready.isEmpty()) {
                Node node = ready.removeFirst();
                order.add(node);

                for (Node child : children.get(node)) {
                    int remaining = indegree.merge(child, -1, Integer::sum);
                    if (remaining == 0) {
                        ready.addLast(child);
                    }
                }
            }

            if (order.size() != nodes.size()) {
                throw new IllegalArgumentException("Computation graph contains a cycle");
            }
            return List.copyOf(order);
        }
    }

    // ---------- XOR network ----------

    public static void main(String[] args) {
        Random random = new Random(7);

        Input x1 = new Input("x1");
        Input x2 = new Input("x2");
        Input target = new Input("target");

        // Hidden neuron 1 parameters.
        Parameter w11 = parameter("w11", random);
        Parameter w12 = parameter("w12", random);
        Parameter b1  = parameter("b1", random);

        // Hidden neuron 2 parameters.
        Parameter w21 = parameter("w21", random);
        Parameter w22 = parameter("w22", random);
        Parameter b2  = parameter("b2", random);

        // Output neuron parameters.
        Parameter v1 = parameter("v1", random);
        Parameter v2 = parameter("v2", random);
        Parameter bo = parameter("bo", random);

        Node h1 = tanh(neuron(x1, x2, w11, w12, b1));
        Node h2 = tanh(neuron(x1, x2, w21, w22, b2));
        Node prediction = tanh(add(add(mul(v1, h1), mul(v2, h2)), bo));
        Node loss = new SquaredError(prediction, target);

        Graph graph = new Graph(loss);
        graph.printTopologicalOrder();

        // Bipolar XOR: equal inputs -> -1, different inputs -> +1.
        double[][] examples = {
                {-1, -1, -1},
                {-1, +1, +1},
                {+1, -1, +1},
                {+1, +1, -1}
        };

        int epochs = 10_000;
        double learningRate = 0.05;

        for (int epoch = 1; epoch <= epochs; epoch++) {
            double totalLoss = 0.0;

            for (double[] example : examples) {
                x1.set(example[0]);
                x2.set(example[1]);
                target.set(example[2]);

                totalLoss += graph.forward();
                graph.backward();
                graph.step(learningRate);
            }

            if (epoch == 1 || epoch % 1_000 == 0) {
                System.out.printf("epoch %5d  average loss = %.6f%n",
                        epoch, totalLoss / examples.length);
            }
        }

        System.out.println("\nPredictions after training:");
        for (double[] example : examples) {
            x1.set(example[0]);
            x2.set(example[1]);
            target.set(example[2]);
            graph.forward();

            System.out.printf("%+.0f XOR %+.0f -> %+.4f  (target %+.0f)%n",
                    example[0], example[1], prediction.value, example[2]);
        }
    }

    private static Parameter parameter(String name, Random random) {
        // Small symmetric initialization.
        return new Parameter(name, random.nextDouble(-0.5, 0.5));
    }
}
