import java.util.Objects;
import java.util.Comparator;

public class TreeSet<E> implements OrderedSet<E> {
    private TreeNode<E> root;
    private final Comparator<? super E> cmp;

    private static class TreeNode<E> {
        E element;
        TreeNode<E> left, right;

        TreeNode(E e) { this.element = e; }
    }

    public TreeSet(Comparator<? super E> cmp) { this.cmp = cmp; }

    public boolean contains(E e) {
        TreeNode<E> node = root;
        while (Objects.nonNull(node)) {
            if (node.element.equals(e)) return true;
            node = cmp.compare(node.element, e) > 0 ? node.left : node.right;
        }
        return false;
    }

    public boolean add(E e) {
        Objects.requireNonNull(e, "null elements not permitted");
        if (Objects.isNull(root)) {
            root = new TreeNode<>(e);
            return true;
        }
        for (TreeNode<E> node = root; !node.element.equals(e);)
            if (cmp.compare(node.element, e) > 0)
                if (Objects.isNull(node.left)) {
                    node.left = new TreeNode<>(e);
                    return true;
                } else node = node.left;
            else
                if (Objects.isNull(node.right)) {
                    node.right = new TreeNode<>(e);
                    return true;
                } else node = node.right;
        return false;
    }

    public boolean remove(E e) {
        TreeNode<E> node = root, parent = null;
        boolean left = false;
        while (Objects.nonNull(node) && !node.element.equals(e)) {
            parent = node;
            left = cmp.compare(node.element, e) > 0;
            node = left ? node.left : node.right;
        }
        if (Objects.isNull(node)) return false;
        if (Objects.nonNull(node.left) && Objects.nonNull(node.right))
            // node has 2 children
            if (Objects.isNull(node.left.right)) {
                // left inner grandchild is null so left child can be promoted
                node.left.right = node.right;
                node = node.left;
            } else if (Objects.isNull(node.right.left)) {
                // right inner grandchild is null so right child can be promoted
                node.right.left = node.left;
                node = node.right;
            } else {
                // node has both inner grandchildren, just replace with predecessor
                TreeNode<E> pred = removeMax(node.left);
                pred.left = node.left;
                pred.right = node.right;
                node = pred;
            }
        else
            // node has 0 or 1 children, so just remove it and promote child
            node = Objects.nonNull(node.left) ? node.left : node.right;
        if (Objects.nonNull(parent)) {
            if (left) parent.left  = node;
            else      parent.right = node;
        } else root = node;
        return true;
    }

    private TreeNode<E> removeMax(TreeNode<E> node) {
        TreeNode<E> parent = node;
        while (Objects.nonNull(node.right)) {
            parent = node;
            node = node.right;
        }
        parent.right = node.left;
        return node;
    }

    public E successor(E e) {
        TreeNode<E> node = successor(root, e);
        return Objects.nonNull(node) ? node.element : null;
    }

    private TreeNode<E> successor(TreeNode<E> node, E e) {
        if (Objects.isNull(node)) return null;
        if (node.element.equals(e)) {
            if (Objects.nonNull(node.right)) {
                node = node.right;
                while (Objects.nonNull(node.left)) node = node.left;
                return node;
            }
            return null;
        } else if (cmp.compare(node.element, e) > 0) {
            return Objects.requireNonNullElse(successor(node.left, e), node);
        } else return successor(node.right, e);
    }

    public E predecessor(E e) {
        TreeNode<E> node = predecessor(root, e);
        return Objects.nonNull(node) ? node.element : null;
    }

    private TreeNode<E> predecessor(TreeNode<E> node, E e) {
        if (Objects.isNull(node)) return null;
        if (node.element.equals(e)) {
            if (Objects.nonNull(node.left)) {
                node = node.left;
                while (Objects.nonNull(node.right)) node = node.right;
                return node;
            }
            return null;
        } else if (cmp.compare(node.element, e) < 0) {
            return Objects.requireNonNullElse(predecessor(node.right, e), node);
        } else return predecessor(node.left, e);
    }

    public Iterator<E> preorder();
    public Iterator<E> inorder();
    public Iterator<E> postorder();

    private static abstract class TreeIterator<E> implements Iterator<E> {
        protected Stack<TreeNode<E>> stack;

        TreeIterator(TreeNode<E> root) {
            if (Objects.nonNull(root)) stack.push(root);
        }

        public boolean hasNext() { return !stack.isEmpty(); }
        public abstract E next();
    }

    private class PreOrderIterator extends TreeIterator<E> {
        PreOrderIterator() { super(root); }

        public E next() {
            if (stack.isEmpty()) throw new NoSuchElementException();
            TreeNode<E> node = stack.pop();
            if (Objects.nonNull(node.right)) stack.push(node.right);
            if (Objects.nonNull(node.left))  stack.push(node.left);
            return node.element;
        }
    }

    private class InOrderIterator extends TreeIterator<E> {
        private TreeNode<E> prev = null;

        InOrderIterator() { super(root); }

        public E next() {
            E element = null;
            while (Objects.isNull(element)) {
                if (stack.isEmpty()) throw new NoSuchElementException();
                TreeNode<E> node = stack.pop();
                if (Objects.nonNull(node.right) && node.right == prev) {
                    prev = node; // just returned from right child
                } else if (Objects.nonNull(node.left) && node.left == prev) {
                    prev = node; // just returned from left child
                    element = node.element;
                    if (Objects.nonNull(node.right)) {
                        stack.push(node);
                        stack.push(node.right);
                    }
                } else {
                    // did not just return from a child
                    prev = node;
                    if (Objects.nonNull(node.left)) {
                        stack.push(node);
                        stack.push(node.left);
                    } else {
                        element = node.element;
                        if (Objects.nonNull(node.right)) {
                            stack.push(node);
                            stack.push(node.right);
                        }
                    }
                }
            }
            return element;
        }
    }

    private class PostOrderIterator extends TreeIterator<E> {
        private TreeNode<E> prev = null;

        PostOrderIterator() { super(root); }

        public E next() {
            E element = null;
            while (Objects.isNull(element)) {
                if (stack.isEmpty()) throw new NoSuchElementException();
                TreeNode<E> node = stack.pop();
                if (Objects.nonNull(node.right) && node.right == prev) {
                    prev = node; // just returned from right child
                    element = node.element;
                } else if (Objects.nonNull(node.left) && node.left == prev) {
                    prev = node; // just returned from left child
                    if (Objects.nonNull(node.right)) {
                        stack.push(node);
                        stack.push(node.right);
                    } else element = node.element;
                } else {
                    // did not just return from a child
                    prev = node;
                    if (Objects.nonNull(node.left)) {
                        stack.push(node);
                        stack.push(node.left);
                    } else if (Objects.nonNull(node.right)) {
                        stack.push(node);
                        stack.push(node.right);
                    } else {
                        element = node.element;
                    }
                }
            }
            return element;
        }
    }
}