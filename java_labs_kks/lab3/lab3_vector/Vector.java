package lab3_vector;

import lab3_vector.*;

@SuppressWarnings("unchecked")
public class Vector<T>{
    //our main array
    private T[] arr;
    //len for current array lenth
    private int len;
    //capacity for current maximum array length
    private int capacity;

    //basic constructor
    public Vector(){
        this.arr = (T[])new Object[1];
        this.capacity = 1;
        this.len = 0;
    }
    //constructor with custom capacity
    public Vector(int _capacity){
        this.arr = (T[])new Object[_capacity];
        this.capacity = _capacity;
        this.len = 0;
    }
    //private constructor for copy constructor
    private Vector(T[] _arr, int _len, int _cap){
        this.arr = _arr;
        this.len = _len;
        this.capacity = _cap;
    }
    //copy constructor
    public Vector(Vector<T> vec){
        this(vec.arr, vec.len, vec.capacity);
    }
    //resize function for array extension
    private void resize(){
        T[] new_arr = (T[])new Object[2*this.capacity];
        for (int i = 0; i < this.capacity; i++){
            new_arr[i] = this.arr[i];
        }
        this.arr = new_arr;
        this.capacity = 2*this.capacity;
    }

    public int getCapacity(){
        return this.capacity;
    }
    public int getLen(){
        return this.len;
    }

    //add element to end of array
    public void push(T element){
        if (this.len == this.capacity){
            resize();
        }
        this.arr[len] = element;
        this.len++;
    }
    //delete last element and get him
    public T pop() throws Exception{
        if (this.len == 0){
            throw new Exception("Error: nothing to pop");
        }
        len--;
        return this.arr[len];
    }
    //standart get element operation
    public T get(int index) throws Exception{
        if (index > this.len || index < 0){
            throw new Exception("Bad Index");
        }

        return this.arr[index];

    }

    //insert function without copy method using
    public void insert(T element, int index) throws Exception{
        if (index > this.len || index < 0){
            throw new Exception("Bad Index");
        }

        if (this.len == this.capacity){
            resize();
        }

        len++;


        for (int i = len-1; i > index; i--){
            this.arr[i] = arr[i-1];
        }


        arr[index] = element;
    }

    //insert function without copy method using
    public void delete(int index) throws Exception{
        if (index > this.len || index < 0){
            throw new Exception("Bad Index");
        }

        for (int i = index; i < len-1; i++){
            this.arr[i] = arr[i+1];
        }
        len--;
    }

    //delete by value (first found would be deleted)
    public void deleteByValue(T value) throws Exception{
        for (int i = 0; i < len-1; i++){
            if (this.arr[i] == value){
                delete(i);
                return;
            }
        }
        throw new Exception("Error: Value not found (method deleteByValue)");
    }

    public void clear(){
         //lol just can't call Vector()
        this.arr = (T[])new Object[1];
        this.capacity = 1;
        this.len = 0;
    }

    //method for debug
    @Override
    public String toString(){
        String res = " ";
        for (int i = 0; i < this.len; i++)
        {
            res+=(this.arr[i]+" ");
        }
        return res;
    }


}