using System;
using System.IO;


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
            for (int i=0; i<arr.Length-1; i++){
                do{
                    put(arr, "files/file_2", i, 1);
                    // Console.WriteLine("1");
                    Console.WriteLine(arr[i]);
                    i++;
                } while (arr[i]<arr[i+1]);
                do{
                    put(arr, "files/file_3", i, 1);
                    Console.WriteLine(arr[i]);
                    i++;
                } while (arr[i]<arr[i+1]);
            }
        }

        static void create(string file){
            using (FileStream fs = File.Create(file)){
            }
        }
        static int[] get(string file){
            int[] arr;
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
            using (StreamWriter sw = new StreamWriter(file, true)){
                for (int i = from; i < amount; i++){
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