import java.util.Objects;
import java.util.Comparator;

public class BSTree<E> implements OrderedSet<E> {
    private TreeNode<E> root = null;
    private final Comparator<? super E> cmp;

    private static class TreeNode<E> {
        E element;
        TreeNode<E> left = null, right = null;
        TreeNode(E element) { this.element = element; }
    }

    public BSTree(Comparator<? super E> cmp) {
        this.cmp = Objects.requireNonNull(cmp);
    }
    
    @Override public boolean contains(E element) {
        TreeNode<E> node = root;
        while (Objects.nonNull(node)) {
            int c = cmp.compare(element, node.element);
            if (c == 0) return true;
            node = c < 0 ? node.left : node.right;
        }
        return false;
    }

    @Override public void include(E element) {
        root = include(root, Objects.requireNonNull(element));
    }
    
    private TreeNode<E> include(TreeNode<E> node, E element) {
        if (Objects.isNull(node)) return new TreeNode<>(element);
        int c = cmp.compare(element, node.element);
        if      (c < 0) node.left = include(node.left, element);
        else if (c > 0) node.right = include(node.right, element);
        else            node.element = element;
        return node;
    }

    @Override public void exclude(E element) { root = exclude(root, element); }
    
    private TreeNode<E> exclude(TreeNode<E> node, E element) {
        if (Objects.isNull(node)) return null;
        int c = cmp.compare(element, node.element);
        if      (c < 0) node.left = exclude(node.left, element);
        else if (c > 0) node.right = exclude(node.right, element);
        else if (Objects.isNull(node.left)) node = node.right;
        else if (Objects.isNull(node.right)) node = node.left;
        else node.right = exclude(node.right, node.element = min(node.right));
        return node;
    }
    
    private E min(TreeNode<E> node) {
        while (Objects.nonNull(node.left)) node = node.left;
        return node.element;
    }

    @Override public E predecessor(E element) {
        TreeNode<E> node = root, pred = null;
        while (Objects.nonNull(node))
            if (cmp.compare(element, node.element) > 0) {
                pred = node;
                node = node.right;
            } else node = node.left;
        return Objects.nonNull(pred) ? pred.element : null;
    }

    @Override public E successor(E element) {
        TreeNode<E> node = root, succ = null;
        while (Objects.nonNull(node))
            if (cmp.compare(element, node.element) < 0) {
                succ = node;
                node = node.left;
            } else node = node.right;
        return Objects.nonNull(succ) ? succ.element : null;
    }
}
