package lab6_fast_sorting;

public class FastSortAlgs{
    public static <T extends Comparable<T>> void qsort(T a[]) {
           qsort(a,0,a.length-1);
    }

    public static <T extends Comparable<T>> void qsort(T a[], int sleft, int sright) {
        if (sleft < sright)
        {
            //make partition
            T pivot = a[(sleft + sright) / 2];
            int left = sleft;
            int right = sright;

            while (left <= right) {

                while (a[left].compareTo(pivot) < 0) {
                    left++;
                }
                while (a[right].compareTo(pivot) > 0) {
                    right--;
                }


                if (left <= right) {
                    T t = a[left];
                    a[left] = a[right];
                    a[right] = t;
                    left++;
                    right--;

                }
            }

            qsort(a, sleft, left-1);
            qsort(a, left, sright);
        }
    }

    public static <T extends Comparable<T>> void heap_sort(T arr[]) {
        heap_sort_range(arr, 0, arr.length);
    }

    public static <T extends Comparable<T>> void heap_sort_range(T arr[], int left, int right){
        int n= right-1;
        for (int i = n/2; i >=left; i--)
            heap_rebalance(arr, i, n);
        for (int i = n; i >= left; i--){
            //swap
            T t = arr[left];
            arr[left] = arr[n];
            arr[n] = t;
            n--;
            heap_rebalance(arr, 0, n);
        }
    }

    private static <T extends Comparable<T>> void heap_rebalance(T arr[], int pos, int size){
        int a = 2*pos+1;
        int b = 2*pos+2;
        int largest_el = pos;

        if (a <= size)
            if (arr[a].compareTo(arr[largest_el]) > 0){
                largest_el = a;
            }
        if (b <= size)
            if ( arr[b].compareTo(arr[largest_el]) > 0){
                largest_el = b;
            }

        if (largest_el != pos) {
            //swap
            T t = arr[pos];
            arr[pos] = arr[largest_el];
            arr[largest_el] = t;

            heap_rebalance(arr, largest_el, size);
        }
    }


    public static Integer[] merge_sort(Integer[] arr){
        if (arr.length < 2)
            return arr;
        int mid = arr.length / 2;
        //переписать все из T[] к Integer[]
        //когда берешь индекс - приводишь к T
        Integer[] arr_left = new Integer[mid]; //Arrays.copyOfRange(arr, 0, mid);
        System.arraycopy(arr,0,arr_left,0, mid );
        Integer[] arr_right =  new Integer[arr.length - mid]; // Arrays.copyOfRange(arr,  mid, arr.length);
        System.arraycopy(arr,mid,arr_right,0, arr.length - mid );
        return merge_op(merge_sort(arr_left), merge_sort(arr_right));
    }

    private static Integer[] merge_op(Integer[] arr1, Integer[] arr2){
        Integer[] buf = new Integer[arr1.length+arr2.length];

        int i_up = 0;
        int i_down = 0;

        for (int i =0; i < buf.length; i++){
            if (i_up == arr1.length){
                buf[i] = arr2[i_down++];
            } else if (i_down == arr2.length) {
                buf[i] = arr1[i_up++];
            } else {
                if ( arr1[i_up].compareTo(arr2[i_down]) < 0) {
                    buf[i] = arr1[i_up++];
                } else {
                    buf[i] = arr2[i_down++];
                }
            }
        }
        return buf;
    }
}
