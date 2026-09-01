import java.util.PriorityQueue;

public class HuffmanTree {
    interface HuffmanNode {
        void accept(HuffmanVisitor visitor);
    }

    class HuffmanInternalNode implements HuffmanNode {
        int frequency;
        HuffmanNode left, right;

        HuffmanNode(HuffmanNode left, HuffmanNode right) {
            this.frequency = left.frequency + right.frequency;
            this.left = left;
            this.right = right;
        }

        void accept(HuffmanVisitor visitor) {
            visitor.visit(this);
        }
    }

    class HuffmanLeaf extends HuffmanInternalNode {
        char symbol;

        HuffmanLeaf(int symbol, int frequency) {
            super(frequency);
            this.symbol = (char)symbol;
        }

        void accept(HuffmanVisitor visitor) {
            visitor.visit(this);
        }
    }

    private HuffmanNode buildTree(String data) {
        // step 1: count frequencies
        int[] frequency = new int[256];
        for (int i = 0; i < data.length(); i++) {
            frequency[data.charAt(i)]++;
        }

        // step 2: create leaf nodes for each character with non-zero frequency
        PriorityQueue<HuffmanNode> pq = PriorityQueue<>(
            (left, right) -> left.frequency - right.frequency
        );
    
        for (int i = 0; i < frequency.length(); i++) {
            if (frequency[i] > 0) {
                pq.add(new HuffmanLeaf(i, frequency[i]));
            }
        }

        // step 3: while pq has >1 element, combine top two
        while (pq.size() > 1) {
            pq.add(new HuffmanInternalNode(pq.poll(), pq.poll()));
        }
        return pq.poll();
    }

    public char[] compress(String src) {
        HuffmanNode compressionTree = buildTree(src);
        int[] symbolValue = new int[256];
        int[] bitWidth = new int[256];
    }

    interface HuffmanVisitor {
        void visit(HuffmanInternalNode node);
        void visit(HuffmanLeaf leaf);
    }

    class GetSymbolsVisitor implements HuffmanVisitor {
        Stack<HuffmanNode<E>> stack;
        int[] symbolValue = new int[256];
        int[] bitWidth = new int[256];
        int bitLength = 0;

        public void traverse() {
            while (!stack.isEmpty()) {
                HuffmanNode<E> node = stack.pop();
                node.accept(this);
            }
        }

        public void visit(HuffmanInternalNode node) {
            // put both children on the stack
            // keep track of the symbols recorded so far
            stack.push(node.right);
            stack.push(node.left);
            bitLength++;
        }

        public void visit(HuffmanLeaf leaf) {
            // don't put anything on the stack, just record the
            // symbol's value and bitlength
            symbolValue[leaf.symbol] = value; // chain of bits
            bitWidth[leaf.symbol] = bitLength;
        }
    }
}
