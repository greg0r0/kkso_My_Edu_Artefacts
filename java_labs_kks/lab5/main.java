
import lab5_base_sorting.*;

class Program{

    public static void print_arr(Integer[] c){
        for (int i = 0; i < c.length; i++){
            System.out.println(c[i]);
        }
    }

    public static void main(String[] args)
    {
        BaseSortAlgsTestSuite tests = new BaseSortAlgsTestSuite();
        tests.Run();
    }

}
