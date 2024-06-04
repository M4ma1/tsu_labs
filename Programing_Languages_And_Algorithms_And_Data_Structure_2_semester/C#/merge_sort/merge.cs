using System;
using System.IO;
using System.Linq;

namespace Rusik{
    public class merge{
        static void Main(string[] args){
            int[] arr = gen_arr();
            int[] arr1 = gen_arr();
            create("files/file_1");
            create("files/file_2");
            create("files/file_3");
            create("files/file_4");
            create("files/file_5");

            put(arr, "files/file_1", 0, arr.Length);
            put(arr1, "files/file_1", 0, arr.Length);

            arr = get("files/file_1");
            Console.WriteLine("[{0}]", string.Join(", ", arr));

            partition(arr);

            Console.WriteLine("[{0}]", string.Join(", ", get("files/file_2")));
            Console.WriteLine("[{0}]", string.Join(", ", get("files/file_3")));

            mergeFiles("files/file_2","files/file_3","files/file_4","files/file_5");
        }

        static void mergeFiles(string file1, string file2, string file_out1, string file_out2){

            int[] arr1 = getFirstOrderedPart(file1);
            Console.WriteLine("arr1: [{0}]", string.Join(", ", arr1));

            int[] arr2 = getFirstOrderedPart(file2);
            Console.WriteLine("arr2: [{0}]", string.Join(", ", arr2));
            
            int[] arr3 = MergeArrays(arr1, arr2);

            put(ShakerSort(arr3), file_out1, 0, arr3.Length);

            arr1 = getFirstOrderedPart(file1);
            arr2 = getFirstOrderedPart(file2);
            arr3 = MergeArrays(arr1, arr2);

            put(ShakerSort(arr3), file_out2, 0, arr3.Length);      
        }
        static int[] MergeArrays(int[] array1, int[] array2)
        {
            // Create a new array with a length equal to the sum of the lengths of the two input arrays
            int[] mergedArray = new int[array1.Length + array2.Length];

            // Copy elements from the first array into the merged array
            Array.Copy(array1, 0, mergedArray, 0, array1.Length);

            // Copy elements from the second array into the merged array
            Array.Copy(array2, 0, mergedArray, array1.Length, array2.Length);

            return mergedArray;
        }
        static int[] getFirstOrderedPart(string file){
            int[] arr;
            
            arr = get(file);

            // Find the first ordered part
            int maxLength = 1;
            for (int i = 1; i < arr.Length; i++){
                if (arr[i] < arr[i-1]){
                    break;
                }
                maxLength++;
            }

            // Create a new array for the ordered part
            int[] orderedPart = new int[maxLength];
            Array.Copy(arr, orderedPart, maxLength);

            Console.WriteLine(maxLength);
            deletePart(file, maxLength);

            return orderedPart;
        }
        static void deletePart(string file, int length){
            int[] arr = get(file);
            arr = arr.Skip(length).ToArray(); 
            File.Delete(file);
            put(arr, file, 0, arr.Length);
        }

        // elements exchange method
        static void Swap(ref int e1, ref int e2)
        {
            var temp = e1;
            e1 = e2;
            e2 = temp;
        }

        // sort by shuffle
        static int[] ShakerSort(int[] array)
        {
            for (var i = 0; i < array.Length / 2; i++)
            {
                var swapFlag = false;
                // pass from left to right
                for (var j = i; j < array.Length - i - 1; j++)
                {
                    if (array[j] > array[j + 1])
                    {
                        Swap(ref array[j], ref array[j + 1]);
                        swapFlag = true;
                    }
                }

                // pass from right to left
                for (var j = array.Length - 2 - i; j > i; j--)
                {
                    if (array[j - 1] > array[j])
                    {
                        Swap(ref array[j - 1], ref array[j]);
                        swapFlag = true;
                    }
                }

                // if there were no exchanges, exit
                if (!swapFlag)
                {
                    break;
                }
            }

            return array;
        }
        static void partition(int[] arr){
            if (arr[0]<arr[1]){put(arr, "files/file_2", 0, 1);}
            else{put(arr, "files/file_3", 0, 1);}

            for (int i=1; i<arr.Length;){
                do{
                    put(arr, "files/file_2", i, 1);
                    i++;
                } while (i<arr.Length && arr[i-1]<arr[i]);
                do{
                    put(arr, "files/file_3", i, 1);
                    i++;
                } while (i<arr.Length && arr[i-1]<arr[i]);
            }
        }
        static void create(string file){
            using (FileStream fs = File.Create(file)){
            }
        }
        static int[] get(string file){
            int[] arr;
            using (var stream = File.OpenRead(filename))
            using (var reader = new StreamReader(stream, Encoding.UTF8))
            {
                char[] buffer = new char[3];
                int n = reader.ReadBlock(buffer, 0, 3);

                char[] result = new char[n];

                Array.Copy(buffer, result, n);

                return result;
            }
            using (StreamReader sr = new StreamReader(file)){
                // Read the entire file and split it into an array of strings using space as a delimiter
                string[] strArr = sr.ReadToEnd().Split(' ');

                // Initialize the int array with the same length as the string array
                arr = new int[strArr.Length-1];

                // Parse each string element into an integer and store it in the int array
                for (int i = 0; i < strArr.Length-1; i++){
                    arr[i] = int.Parse(strArr[i]);
                }
            }
            return arr;
        }
        static void put(int[] arr, string file, int from, int amount){
            if(from >= arr.Length)return;
            using (StreamWriter sw = new StreamWriter(file, true)){
                for (int i = from; i < from+amount; i++){
                    // Write the element to the file followed by a space
                    sw.Write(arr[i]);
                    sw.Write(" ");
                }
            }
        }
        static int[] gen_arr(){
            Random random = new Random();  // to generate random array

            Console.WriteLine("enter array len: ");
            int len = Convert.ToInt32(Console.ReadLine());
            int[] array = new int[len];

            for(int i=0; i<len; i++){
                array[i] = random.Next(100); // to generate random array
            }
            Console.WriteLine("[{0}]", string.Join(", ", array));
            return array;
        }
    }
}