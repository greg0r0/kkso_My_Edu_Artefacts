package lab2_linked_list;

import java.lang.reflect.Method;

public class LListTestSuite
{
    public static void Run(){
        Method[] tests = LListTestSuite.class.getMethods();
        int succ_test = 0;
        int tests_num = 0;
        for (Method test: tests){
            String test_name = test.getName();
            if (test_name.startsWith("Test"))
            {
                tests_num++;
                try {
                    if ((boolean) test.invoke(null)){
                        System.out.println(String.format("[+] <%s> passed successfully", test_name));
                        succ_test++;
                    } else {
                        System.out.println(String.format("[-] <%s> not passed", test_name)); 
                        //добавить как-то вывод того, что тестировалось и того, что ожидалось. Прокся-метод?
                    }
                } catch (Exception e){
                    System.out.println("[!] Failed to run test " + test_name);
                }
            }
        }
        System.out.println(String.format("Amount of tests: %d  Passage: %.2f%%", tests_num, ((double)succ_test/tests_num)*100));
    }
    //тесты выгоядят отвратно, нужно продумать дизайн. 
    //some tests here
    public static boolean TestConstructor(){
        LListNode<Integer> a = new LListNode<Integer>(1, null);
        LListNode<Integer> b = new LListNode<Integer>(2, null);
        LList<Integer> lst = new LList<Integer>(a,b,2);
        return (lst.getLength() == 2);
    }

    public static boolean TestCopyConstructor(){
        LListNode<Integer> a = new LListNode<Integer>(1, null);
        LListNode<Integer> b = new LListNode<Integer>(2, null);
        LListNode<Integer> c = new LListNode<Integer>(3, null);
        LList<Integer> lst = new LList<Integer>(a,b,2);
        LList<Integer> lst_copy = new LList<Integer>(lst);
        lst.addLastNode(c);
        return (lst_copy.getLength() == 2);
    }

    public static boolean TestAddNode(){
        LListNode<Integer> a = new LListNode<Integer>(1, null);
        LListNode<Integer> b = new LListNode<Integer>(2, null);
        LListNode<Integer> c = new LListNode<Integer>(3, null);
        LListNode<Integer> d = new LListNode<Integer>(4, null);
        LList<Integer> lst = new LList<Integer>(a,b,2);
        lst.addFirstNode(c);
        lst.addLastNode(d);
        return (lst.getFirstData() == 3) && (lst.getLastData() == 4) && (lst.getLength() == 4);
    }
    public static boolean TestDeleteFirstNode(){
        LListNode<Integer> a = new LListNode<Integer>(1, null);
        LListNode<Integer> b = new LListNode<Integer>(2, null);
        LList<Integer> lst = new LList<Integer>(a,b,2);
        lst.deleteFirstNode();
        return (lst.getFirstData() == 2) && (lst.getLength() == 1);
    }
    public static boolean TestDeleteLastNode(){
        LListNode<Integer> a = new LListNode<Integer>(1, null);
        LListNode<Integer> b = new LListNode<Integer>(2, null);
        LList<Integer> lst = new LList<Integer>(a,b,2);
        lst.deleteLastNode();
        return (lst.getLength() == 1);
    }
    public static boolean TestDeleteByValue(){
        LListNode<Integer> a = new LListNode<Integer>(1, null);
        LListNode<Integer> b = new LListNode<Integer>(2, null);
        LListNode<Integer> c = new LListNode<Integer>(3, null);
        LListNode<Integer> d = new LListNode<Integer>(4, null);
        LListNode<Integer> e = new LListNode<Integer>(5, null);
        LList<Integer> lst = new LList<Integer>(a, b, 2);
        lst.addLastNode(c);
        lst.addLastNode(d);
        lst.addLastNode(e);
        try {
            lst.deleteByValue(3);
            lst.deleteByValue(5);
            lst.deleteByValue(1);

            lst.deleteByValue(1337);
        } catch (Exception ex) {
            
        }
        return (lst.getLength() == 2) && (lst.getFirstData() == 2) && (lst.getLastData() == 4);
    }
    public static boolean TestDeleteList(){
        LListNode<Integer> a = new LListNode<Integer>(1, null);
        LListNode<Integer> b = new LListNode<Integer>(2, null);
        LList<Integer> lst = new LList<Integer>(a,b,2);
        lst.deleteList();
        return (lst.getLength() == 0) && (lst.extendedGetLength() == 0);
    }
    
}