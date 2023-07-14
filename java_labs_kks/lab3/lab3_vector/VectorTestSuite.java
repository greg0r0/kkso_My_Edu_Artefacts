package lab3_vector;

import lab3_vector.*;

import java.lang.reflect.Method;


public class VectorTestSuite
{
    public static void Run(){
        Method[] tests = VectorTestSuite.class.getMethods();
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
        System.out.println(String.format("Amount of tests: %d  Passage: %.2f%%\n", tests_num, ((double)succ_test/tests_num)*100));
    }
    //тесты выгоядят отвратно, нужно продумать дизайн. 
    //some tests here
    // public static boolean TestConstructor(){
    //     LListNode<Integer> a = new LListNode<Integer>(1, null);
    //     LListNode<Integer> b = new LListNode<Integer>(2, null);
    //     LList<Integer> lst = new LList<Integer>(a,b,2);
    //     return (lst.getLength() == 2);
    // }

    public static boolean TestConstructor(){
        lab3_vector.Vector<Integer> vec1 = new lab3_vector.Vector<Integer>();
        lab3_vector.Vector<Integer> vec2 = new lab3_vector.Vector<Integer>(10);
        lab3_vector.Vector<Integer> vec3 = new lab3_vector.Vector<Integer>(vec2);
        return (vec1.getCapacity() == 1) && (vec2.getCapacity() == 10) && (vec3.getCapacity() == 10);
    }

    public static boolean TestResize(){
        lab3_vector.Vector<Integer> vec = new lab3_vector.Vector<Integer>(2);
        for (int i = 0; i<=10; i++){
            vec.push(i);
        }
        return (vec.getCapacity() == 16);
    }

    public static boolean TestBasicMethods(){
        lab3_vector.Vector<Integer> vec = new lab3_vector.Vector<Integer>(2);
        try{
            for (int i = 1; i<=10; i++){
                vec.push(i);
            }
            return (vec.get(3) == 4) && ( vec.pop() == 10);
        } catch (Exception ex){
            return false;
        }
    }

    public static boolean TestArrayChangingFunctions(){
        lab3_vector.Vector<Integer> vec = new lab3_vector.Vector<Integer>(2);
        for (int i = 1; i<=10; i++){
            vec.push(i);
        }
        try{
            vec.insert(15,3);
            vec.delete(5);
            return (vec.get(3) == 15) && (vec.get(5) == 6) && (vec.getLen() == 10);
        } catch (Exception ex) {
            return false;
        }
    }

    public static boolean TestClear(){
        lab3_vector.Vector<Integer> vec = new lab3_vector.Vector<Integer>(2);
        for (int i = 1; i<=10; i++){
            vec.push(i);
        }
        vec.clear();
        return (vec.getCapacity() == 1);
    }
}
