package lab4_binary_tree;

public class BinaryTree<TKeyType extends Comparable<TKeyType>, TDataType>
{

    static class BinaryTreeNode<TKey, TData>
    {
        private TKey key;
        private TData data;

        private BinaryTreeNode<TKey, TData> left, right;

        public BinaryTreeNode(){}

        public BinaryTreeNode(TKey _key, TData _data ){
            this.key  = _key;
            this.data = _data;
            this.left = null;
            this.right = null;
        }

        public BinaryTreeNode(TKey _key, TData _data, BinaryTreeNode<TKey, TData> _left, BinaryTreeNode<TKey, TData> _right) {
            this.key = _key;
            this.data = _data;
            this.left = _left;
            this.right = _right;
        }

        public TKey getKey(){
            return this.key;
        }
        public TData getData(){
            return this.data;
        }
    }

    private BinaryTreeNode<TKeyType, TDataType> head;
    private int len;

    public BinaryTree(){
        this.head = null;
        this.len = 0;
    }

    public BinaryTree(TKeyType _head_key, TDataType _head_data )
    {
        this.head = new  BinaryTreeNode<TKeyType, TDataType>(_head_key, _head_data);
        this.len = 1;
    }

    private BinaryTree(BinaryTreeNode<TKeyType, TDataType> _head, int _len)
    {
        this.head = _head;
        this.len = _len;
    }

    public BinaryTree(BinaryTree<TKeyType, TDataType> another_tree)
    {
        this(another_tree.head, another_tree.len);
    }

    public TDataType get(TKeyType k){
        BinaryTreeNode<TKeyType, TDataType>  node = this.head;

        while (node != null){
            int res = k.compareTo(node.key);
            if (res == 0){
                return node.data;
            }

            if (res > 0) {
                node = node.right;
            } else {
                node = node.left;
            }
        }

        return null;
    }

    public void insert(TKeyType k, TDataType d) {
        if (this.head == null) {
            this.head = new BinaryTreeNode<TKeyType, TDataType>(k, d);
            this.len++;
            return;
        }

        BinaryTreeNode<TKeyType, TDataType> node = this.head;

        while (node != null) {
            int res = k.compareTo(node.key);

            if (res == 0) {
                node.data = d;
                return;
            } else {
                if (res > 0) {
                    if (node.right != null) {
                        node = node.right;
                    } else {
                        node.right = new BinaryTreeNode<TKeyType, TDataType>(k, d);
                        this.len++;
                        return;
                    }
                } else {
                    if (node.left != null) {
                        node = node.left;
                    } else {
                        node.left = new BinaryTreeNode<TKeyType, TDataType>(k, d);
                        this.len++;
                        return;
                    }
                }
            }
        }
    }

    public int getLen(){
        return this.len;
    }

    public void clear(){
        this.head = null;
        this.len = 0;
    }

    //DEBUG
    private void print(BinaryTreeNode<TKeyType, TDataType> node, int depth){
        if (node.right != null){
            print(node.right, depth+1);
        }
        String space = new String(new char[depth*2]).replace('\0', ' ');
        System.out.println(space + node.key + "-{" + node.data+"}");
        if (node.left != null){
            print(node.left, depth+1);
        }
    }
    public void PrintTree(){
        this.print(this.head,0);
    }
}















