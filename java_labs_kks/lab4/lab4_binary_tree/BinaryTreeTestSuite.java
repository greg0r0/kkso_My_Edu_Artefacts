package lab4_binary_tree;

import java.lang.reflect.Method;

public class BinaryTreeTestSuite {
    public static void Run() {
        Method[] tests = BinaryTreeTestSuite.class.getMethods();
        int succ_test = 0;
        int tests_num = 0;
        for (Method test : tests) {
            String test_name = test.getName();
            if (test_name.startsWith("Test")) {
                tests_num++;
                try {
                    if ((boolean) test.invoke(null)) {
                        System.out.println(String.format("[+] <%s> passed successfully", test_name));
                        succ_test++;
                    } else {
                        System.out.println(String.format("[-] <%s> not passed", test_name));
                        //добавить как-то вывод того, что тестировалось и того, что ожидалось. Прокся-метод?
                    }
                } catch (Exception e) {
                    System.out.println("[!] Failed to run test " + test_name);
                }
            }
        }
        System.out.println(String.format("Amount of tests: %d  Passage: %.2f%%\n", tests_num, ((double) succ_test / tests_num) * 100));
    }

    public static boolean TestConstructor() {
        try {
            BinaryTree<Integer, Integer> bintree = new BinaryTree<>();
            assert bintree.getLen() == 0;

            BinaryTree<Integer, Integer> bintree1 = new BinaryTree<>(10,10);
            assert bintree1.getLen() == 1;

            BinaryTree<Integer, Integer> bintree2 = new BinaryTree<>(bintree1);
            bintree2.insert(11,11);
            assert bintree2.getLen() == 2;

            return true;
        } catch (Exception ex) {
            return false;
        }
    }

    public static boolean TestGetter(){
        try {
            BinaryTree<Integer, Integer> bintree = new BinaryTree<>();

            bintree.insert(10, 0);
            assert bintree.get(10) == 0;

            bintree.insert(11, 1);
            bintree.insert(9, -1);
            assert bintree.get(11) == 1 && bintree.get(9) == -1;

            bintree.insert(10,3);   //replace head data
            bintree.insert(11,4);
            assert !(bintree.get(11) == 1) && bintree.get(10) == 3;

            return true;
        } catch (Exception ex) {
            return false;
        }
    }

    public static boolean TestInsertion(){
        try {
            BinaryTree<Integer, Integer> bintree = new BinaryTree<>();
            
            bintree.insert(10, 0);
            assert bintree.getLen() == 1;

            bintree.insert(11, 1);
            bintree.insert(9, -1);
            assert bintree.getLen() == 3;

            bintree.insert(10,3);   //replace head data
            bintree.insert(11,4);
            assert bintree.getLen() == 3;

            return true;
        } catch (Exception ex) {
            return false;
        }
    }

    public static boolean TestCleaning(){
        try {
            BinaryTree<Integer, Integer> bintree = new BinaryTree<>();

            bintree.insert(10, 0);

            bintree.insert(11, 1);
            bintree.insert(9, -1);

            bintree.clear();
            assert bintree.getLen() == 0;

            return true;
        } catch (Exception ex) {
            return false;
        }
    }


}
