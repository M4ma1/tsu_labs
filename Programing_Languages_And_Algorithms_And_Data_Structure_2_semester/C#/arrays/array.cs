using System;

namespace Rusik{
    class Array{
        static void Main(string[] args){
            Random random = new Random();  // to generate random array

            Console.WriteLine("enter array len: ");
            int len = Convert.ToInt32(Console.ReadLine());
            int[] array = new int[len];

            for(int i=0; i<len; i++){
                array[i] = random.Next(10000); // to generate random array
                // array[i] = Convert.ToInt32(Console.ReadLine());
            }

            int[,] matrix = new int[len, len];
            for(int i=0; i<len; i++){
                for(int j=0; j<len; j++){
                    matrix[i, j] = random.Next(10); // to generate random matrix
                }
            }
            for (int i = 0; i < len; i++)
            {
                for (int j = 0; j < len; j++)
                {
                    Console.Write(matrix[i, j] + "\t");
                }
                Console.WriteLine();
            }

            Console.WriteLine("[{0}]", string.Join(", ", array));
            same_num(array);
            matrix = zero_el(matrix);
            for (int i = 0; i < len; i++)
            {
                for (int j = 0; j < len; j++)
                {
                    Console.Write(matrix[i, j] + "\t");
                }
                Console.WriteLine();
            }
        }
        static int[,] zero_el(int[,] matrix){
            int rows = matrix.GetLength(0);
            int cols = matrix.GetLength(1);
            for(int i=0; i<rows; i++){
                for(int j=0; j<cols; j++){
                    if(matrix[i, j] == 0){
                        // Delete the row with the zero element by shifting all rows above it down
                        for(int l=i; l<rows-1; l++){
                            for(int m=0; m<cols; m++){
                                matrix[l, m] = matrix[l+1, m];
                            }
                        }
                        // Decrease the number of rows since we deleted one
                        rows--;
                        return matrix;
                    }
                }
            }
            return matrix;
        }
        static void same_num(int []array){
            for(int i=0; i<array.Length; i++){
                int flag = 1;
                for (int j=0; j<Convert.ToString(array[i]).Length-1; j++){
                    if(Convert.ToString(array[i])[j]!=Convert.ToString(array[i])[j+1])flag=0;
                }
                if(flag==1)Console.WriteLine(array[i]);
            }
        }
    } 
}