using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

class GenerateNumberFile
{
    static public void generateFile(int n)
    {
        string outputFileName = "f.txt";
        int numberOfLines = n; // Number of lines to generate
        int minValue = 1; // Minimum value of the random number
        int maxValue = 100; // Maximum value of the random number

        GenerateRandomNumberFile(outputFileName, numberOfLines, minValue, maxValue);

        Console.WriteLine($"File '{outputFileName}' generated with {numberOfLines} random numbers.");
    }

    static void GenerateRandomNumberFile(string fileName, int numberOfLines, int minValue, int maxValue)
    {
        using (StreamWriter writer = new StreamWriter(fileName))
        {
            Random random = new Random();

            for (int i = 0; i < numberOfLines; i++)
            {
                int randomNumber = random.Next(minValue, maxValue + 1);
                writer.WriteLine(randomNumber);
            }
        }
    }
}

class NaturalMergeSort
{
    static void Main(string[] args)
    {
        GenerateNumberFile.generateFile(Convert.ToInt32(Console.ReadLine()));
        string inputFile = "f.txt";
        PrintData(inputFile);
        string fileA = "fa.txt";
        string fileB = "fb.txt";
        string fileC = "fc.txt";
        string fileD = "fd.txt";

        // Step 1: Split the input file into fa and fb based on natural runs
        SplitIntoNaturalRuns(inputFile, fileA, fileB);
        
        PrintData(fileA);
        PrintData(fileB);
        
        // Steps 2, 3, and 4: Perform alternating merges
        bool sorted = false;
        while (!sorted)
        {
            MergeFiles(fileA, fileB, fileC, fileD);
            PrintData(fileC);
            PrintData(fileD);
            sorted = CountRuns(fileC) == 1 && CountRuns(fileD) == 0;
            if (!sorted)
            {
                MergeFiles(fileC, fileD, fileA, fileB);
                sorted = CountRuns(fileA) == 1 && CountRuns(fileB) == 0;
            }
        }

        bool test = Test(fileA);

        Console.WriteLine(test ? "file is not sorted" : "file is  sorted");

        Console.WriteLine("Sorting complete. Check the sorted file in fa.txt or fc.txt.");
    }

    static bool Test(string fileName){
        int? previousNumber = null;
        using (StreamReader reader = new StreamReader(fileName)){
            while(!reader.EndOfStream){
                int currentNumber = int.Parse(reader.ReadLine());
                if (previousNumber == null || previousNumber <= currentNumber)
                {
                    return true;
                }
                previousNumber = currentNumber;
            }
            return false;
        }

    }

    static int CountRuns(string fileName)
    {
        int runCount = 0;
        int? previousNumber = null;

        using (StreamReader reader = new StreamReader(fileName))
        {
            while (!reader.EndOfStream)
            {
                int currentNumber = int.Parse(reader.ReadLine());
                if (previousNumber == null || previousNumber > currentNumber)
                {
                    runCount++;
                }
                previousNumber = currentNumber;
            }
        }

        return runCount;
    }


    static void PrintQueue(Queue<int> queue)
    {
        Console.WriteLine("Queue elements:");
        foreach (int item in queue)
        {
            Console.WriteLine(item);
        }
    }
    static void PrintData(string File)
    {
        Console.WriteLine("content if the {0}", File);
        using (StreamReader readtext = new StreamReader(File))
        {
            while(!readtext.EndOfStream){
                string readMeText = readtext.ReadLine();
                Console.WriteLine(readMeText);
            }
        }
    }

    static void SplitIntoNaturalRuns(string inputFile, string fileA, string fileB)
    {
        using (StreamReader reader = new StreamReader(inputFile))
        using (StreamWriter writerA = new StreamWriter(fileA))
        using (StreamWriter writerB = new StreamWriter(fileB))
        {
            int? previousNumber = null;
            StreamWriter currentWriter = writerA;
            
            while (!reader.EndOfStream)
            {
                int currentNumber = int.Parse(reader.ReadLine());

                if (previousNumber == null || previousNumber <= currentNumber)
                {
                    currentWriter.WriteLine(currentNumber);
                }
                else
                {
                    // Switch writer
                    currentWriter = currentWriter == writerA ? writerB : writerA;
                    currentWriter.WriteLine(currentNumber);
                }

                previousNumber = currentNumber;
            }
        }
    }

    static void MergeFiles(string inputA, string inputB, string outputC, string outputD)
    {
        using (StreamReader readerA = new StreamReader(inputA))
        using (StreamReader readerB = new StreamReader(inputB))
        using (StreamWriter writerC = new StreamWriter(outputC))
        using (StreamWriter writerD = new StreamWriter(outputD))
        {
            StreamWriter currentWriter = writerC;

            Queue<int> runA = ReadRunAndRemove(new StreamReader(inputA), inputA);
            PrintData(inputA);
            Queue<int> runB = ReadRunAndRemove(new StreamReader(inputB), inputB);

            while (runA.Count > 0 || runB.Count > 0)
            {
                PrintQueue(runA);
                PrintQueue(runB);
                while (runA.Count > 0 && runB.Count > 0)
                {
                    if (runA.Peek() <= runB.Peek())
                    {
                        currentWriter.WriteLine(runA.Dequeue());
                    }
                    else
                    {
                        currentWriter.WriteLine(runB.Dequeue());
                    }
                }

                while (runA.Count > 0)
                {
                    currentWriter.WriteLine(runA.Dequeue());
                }

                while (runB.Count > 0)
                {
                    currentWriter.WriteLine(runB.Dequeue());
                }

                currentWriter = currentWriter == writerC ? writerD : writerC;
                
                runA = ReadRunAndRemove(new StreamReader(inputA), inputA);
                runB = ReadRunAndRemove(new StreamReader(inputB), inputB);
                
            }
        }
    }

    static Queue<int> ReadRunAndRemove(StreamReader reader, string fileName)
    {
        Queue<int> run = new Queue<int>();
        int? previousNumber = null;
        long lastPosition = reader.BaseStream.Position;
        string tempFile = Path.GetTempFileName();

        using (StreamWriter writer = new StreamWriter(tempFile))
        {
            while (!reader.EndOfStream)
            {
                string line = reader.ReadLine();
                int currentNumber = int.Parse(line);

                if (previousNumber == null || previousNumber <= currentNumber)
                {
                    run.Enqueue(currentNumber);
                    previousNumber = currentNumber;
                    lastPosition = reader.BaseStream.Position;
                }
                else
                {
                    writer.WriteLine(line);
                    break;
                }
            }

            // Write remaining lines to temp file
            while (!reader.EndOfStream)
            {
                string line = reader.ReadLine();
                writer.WriteLine(line);
            }
        }

        // Replace the original file with the temp file
        File.Delete(fileName);
        File.Move(tempFile, fileName);

        return run;
    }
}
