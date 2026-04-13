import java.util.PriorityQueue;

public class HuffmanTree {
    static class HuffmanNode {
        int frequency;
        HuffmanNode left, right;

        HuffmanNode(int frequency) {
            this.frequency = frequency;
        }

        HuffmanNode(HuffmanNode left, HuffmanNode right) {
            this.frequency = left.frequency + right.frequency;
            this.left = left;
            this.right = right;
        }
    }

    static class HuffmanLeaf extends HuffmanNode {
        char symbol;

        HuffmanLeaf(int symbol, int frequency) {
            super(frequency);
            this.symbol = (char)symbol;
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
            pq.add(new HuffmanNode(pq.poll(), pq.poll()));
        }
        return pq.poll();
    }
}