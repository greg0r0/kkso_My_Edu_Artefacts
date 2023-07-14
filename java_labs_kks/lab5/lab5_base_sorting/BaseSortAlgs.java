package lab5_base_sorting;

public class BaseSortAlgs{

    public static <T extends Comparable<T>> void sort_buble_range(T c[], int left, int right){
        for (int i = left; i < right; i++){
            for (int j = right-1; j >i;j--)
            {
                //if (c[j] < c[j-1])
                if (c[j].compareTo(c[j-1]) < 0)
                {
                    T tmp = c[j-1];
                    c[j-1] = c[j];
                    c[j] = tmp;
                }
            }
        }
    }

    public static <T extends Comparable<T>> void sort_buble(T c[]){
        sort_buble_range(c, 0, c.length);
    }

    public static <T extends Comparable<T>> void sort_insert_range(T c[], int left, int right){
        for (int i = left+1; i < right; i++){
            T value = c[i];
            int el = i - 1;
            for (; el>=left; el--) {
                //if (value < c[el])
                if (value.compareTo(c[el]) < 0)
                {
                    c[el + 1] = c[el];
                } else {
                    break;
                }
            }
            c[el+1] = value;
        }
    }

    public static <T extends Comparable<T>> void sort_insert(T c[]){
        sort_insert_range(c, 0, c.length);
    }

    public static <T extends Comparable<T>> void sort_selection_range(T c[], int left, int right){
        for (int i = left; i < right; i++){
            int minn = i;

            for (int j = i; j < right; j++){
                //if (c[j] < c[minn])
                if (c[j].compareTo(c[minn]) < 0)
                {
                    minn = j;
                }
            }
            //swap
            T tmp = c[i];
            c[i] = c[minn];
            c[minn] = tmp;
        }
    }

    public static <T extends Comparable<T>> void sort_selection(T c[]){
        sort_selection_range(c, 0, c.length);
    }
}
