package lab7_radix_sorts;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Random;

public class RadixSortsTestSuite {

    public static void Run() {
        Method[] tests = RadixSortsTestSuite.class.getMethods();
        int succ_test = 0;
        int tests_num = 0;
        for (Method test : tests) {
            String test_name = test.getName();
            if (test_name.startsWith("Test")) {
                tests_num++;
                try {
                    long stTime = System.nanoTime();
                    if ((boolean) test.invoke(null)) {
                        System.out.println(String.format("[+] <%s> passed successfully", test_name));
                        succ_test++;
                    } else {
                        System.out.println(String.format("[-] <%s> not passed", test_name));
                        //добавить как-то вывод того, что тестировалось и того, что ожидалось. Прокся-метод?
                    }
                    long endTime = System.nanoTime();
                    System.out.println(String.format(" └─< Time: %.2f sec >",((float)(endTime-stTime)/1_000_000_000.0)));
                } catch (Exception e) {
                    System.out.println("[!] Failed to run test " + test_name);
                    System.out.println(e.getCause());
                }
            }
        }
        System.out.println(String.format("Amount of tests: %d  Passage: %.2f%%\n", tests_num, ((double) succ_test / tests_num) * 100));
    }

    private static <T extends Comparable<T>> boolean check_order(T arr[]){

        for (int i = 1; i < arr.length; i++){
            if (arr[i-1].compareTo(arr[i]) > 0){
                return false;
            }
        }

        return true;
    }

    private static Integer[] toIntArr(ArrayList<Integer> arrlist){
        Integer[] res = new Integer[arrlist.size()];
        for (int i = 0; i < res.length; i++){
            res[i] = arrlist.get(i);
        }
        return res;
    }

    private static Integer[] genIntegerArr(int len){
        Random rnd = new Random();
        Integer[] arr = new Integer[len];
        for (int i = 0; i < len; i++)
            arr[i] = rnd.nextInt(10000);
        return arr;
    }
    private static ArrayList<Integer> genArrList(int len){
        Random rnd = new Random();
        ArrayList<Integer> arr = new ArrayList<>(len);
        for (int i = 0; i < len; i++)
            arr.add(rnd.nextInt(1_000_000));
        return arr;
    }

    public static boolean TestLSDSort(){
        Integer[] arr = genIntegerArr(1000000);
        RadixSorts.sort_lsd(arr);
        return check_order(arr);
    }

    public static boolean TestMSDSort(){
        ArrayList<Integer> arr = genArrList(1000000);
        RadixSorts.sort_msd(arr);
        return check_order(toIntArr(arr));
    }

    public static boolean TestCountingSort(){
        ArrayList<Integer> arr = genArrList(1000000);
        RadixSorts.sort_counting(arr);
        return check_order(toIntArr(arr));
    }

}
