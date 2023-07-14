package lab2_linked_list;

public class LListNode<T>{
    private T data;
    private LListNode<T> nextNode;

    public LListNode(){}
    public LListNode(T _data, LListNode<T> _NextNode){
        data = _data;
        nextNode = _NextNode;
    }
    public LListNode(LListNode<T> node){
        this(node.getData(), node.getNextNode());
    }

    public T getData(){
        return data;
    }

    public LListNode<T> getNextNode(){
        return nextNode;
    }
    
    public void setNextNode(LListNode<T> node){
        nextNode = node;
    }
}
