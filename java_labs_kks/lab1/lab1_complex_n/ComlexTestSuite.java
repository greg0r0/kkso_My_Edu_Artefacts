package lab1_complex_n;

import java.lang.reflect.Method;

public class ComlexTestSuite
{
    public static void Run(){
        Method[] tests = ComlexTestSuite.class.getMethods();
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
    public static boolean TestEmptyConstructor(){
        Complex c = new Complex();
        return (c.getRealPart() == 0.0) && (c.getImaginaryPart() == 0.0);
    }

    public static boolean TestConstructor(){
        Complex c = new Complex(5.0, 6.0);
        return (c.getRealPart() == 5.0) && (c.getImaginaryPart() == 6.0);
    }

    public static boolean TestModulus(){
        Complex c = new Complex(5.0, 6.0);
        return (c.modulus() - 61.0) < 0.01;
    }

    public static boolean TestArgument(){
        Complex c = new Complex(5.0, 6.0);
        return (c.argument() - Math.atan(6.0/5.0)) < 0.01;
    }

    public static boolean TestAdd(){
        Complex c = new Complex(5.0, 6.0);
        c.add(new Complex(6.0, 5.0));
        return (c.getRealPart() == 11.0) && (c.getImaginaryPart() == 11.0);
    }

    public static boolean TestSub(){
        Complex c = new Complex(5.0, 6.0);
        c.sub(new Complex(3.0, 4.0));
        return (c.getRealPart() == 2.0) && (c.getImaginaryPart() == 2.0);
    }

}