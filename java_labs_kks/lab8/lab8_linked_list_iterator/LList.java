package lab8_linked_list_iterator;

import java.util.Iterator;

public class LList<T> implements Iterable<T>{

    private LListNode<T> first_element;
    private LListNode<T> last_element;
    private int list_length;

    //пустой конструктор
    public LList(){};

    public LList(LListNode<T> fe, LListNode<T> se, int len) {
        list_length   = len;
        first_element = fe;
        last_element  = se;

        first_element.setNextNode(last_element);
    }
    //конструктор копирования
    public LList(LList<T> another_list){
        this(another_list.first_element, another_list.last_element, another_list.list_length);
    }

    //Задать входной элемент списка
    public void addFirstNode(LListNode<T> node){
        node.setNextNode(first_element);
        first_element = node;
        list_length++;
    }

    //Задать выходной элемент списка
    public void addLastNode(LListNode<T> node){
        if (list_length == 1){
            first_element.setNextNode(node); //костыль из-за last_node
            last_element = node;
        }
        last_element.setNextNode(node);
        node.setNextNode(null);
        last_element = node;
        list_length++;
    }

    //Удалить первый элемент списка
    public void deleteFirstNode(){
        first_element = first_element.getNextNode();
        list_length--;
    }

    //удалить последний элемент списка
    public void deleteLastNode(){
        LListNode<T> node = first_element;
        while (node.getNextNode().getNextNode() != null) // у след ноды проверяем наличие след ноды 0-о
        {
            node = node.getNextNode();
        }
        node.setNextNode(null);
        list_length--;
    }

    //Удаление элемента по значению
    //Будет удален первый найденный элемент
    public void deleteByValue(T value) throws Exception{
        LListNode<T> node = first_element;
        LListNode<T> lastNode = null;
        
        while (node != null){
            if (node.getData() == value){

                if (node == first_element){
                    first_element = node.getNextNode();
                    node.setNextNode(null);
                } else if (node == this.last_element){
                    lastNode.setNextNode(null);
                    last_element = lastNode;
                } else {
                    lastNode.setNextNode(node.getNextNode());
                    node.setNextNode(null); 
                }
                list_length--;
                return;
            }
            lastNode = node;
            node = node.getNextNode();
        }
        throw new Exception("No element found for delete.");
    }

    //Получить количество элементов в списке
    public int getLength(){
        return this.list_length;
    }
    public int extendedGetLength(){
        int i = 0;
        LListNode<T> node = first_element;
        while (node!=null){
            node = node.getNextNode();
            i++;
        }
        return i;
    }

    //удаление списка
    public void deleteList(){
        //боже, благослови Garbage Collector
        first_element = null;
        last_element = null;
        list_length = 0;
    }

    //DEBUG: вывод цепочки, данных, etc
    public T getLastData(){
        return this.last_element.getData();
    }

    public T getFirstData(){
        return this.first_element.getData();
    }

    public T getData(int position) throws Exception{
        if (position > list_length){
            throw new Exception("Error: Index > list length");
        }
        LListNode<T> node = first_element;
        for (int i = 0; i < position-1; i++){
            node = node.getNextNode();
        }
        return node.getData();
    }

    private void out(LListNode<T> node){
        System.out.print( node.getData() );
        if (node.getNextNode() != null){
            System.out.print(" -> ");
            out(node.getNextNode());
        } else {
            System.out.println();
        }
    }

    public void PrintRope(){
        out(first_element);
    }


    @Override
    public Iterator<T> iterator() {
        return new Iterator<T>() {

            private LListNode<T> current = first_element;

            @Override
            public boolean hasNext() {
                return (current != null);
            }

            @Override
            public T next() {
                if (hasNext()) {
                    T data = current.getData();
                    current = current.getNextNode();
                    return data;
                }
                return null;
            }
        };
    }
}
