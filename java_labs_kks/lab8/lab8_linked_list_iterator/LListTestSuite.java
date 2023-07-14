package lab8_linked_list_iterator;

import java.lang.reflect.Method;
import java.util.Iterator;

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

    public static boolean TestSimple(){
        try {
            int[] arr = {122, 123, 124, 125, 126};
            int i = 0;
            LList<Integer> lst = new LList<>(new LListNode<Integer>(122, null), null, 1);
            lst.addLastNode(new LListNode<Integer>(123, null));
            lst.addLastNode(new LListNode<Integer>(124, null));
            lst.addLastNode(new LListNode<Integer>(125, null));
            lst.addLastNode(new LListNode<Integer>(126, null));
            Iterator<Integer> iter = lst.iterator();
            while (iter.hasNext()) {
                assert iter.next() == arr[i++];
            }
            return true;
        } catch (Exception ex) {
            return false;
        }
    }
    
    
    
}
