
using System;
using System.IO;

namespace Rusik
{
    class Strings
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Input string: ");
            string String = Console.ReadLine();
            Console.WriteLine("Inputted string: {0}",String);
            Console.WriteLine("Input char to find: ");
            char Char = Console.ReadKey().KeyChar;
            int count = CountCharOccurrences(String, Char);
            Console.WriteLine(" {0} times this letter was met in strings", count);
            string stringWithoutDigits = RemoveDigits(String);
            Console.WriteLine("String without digits: {0}", stringWithoutDigits);
            bool containsAllLetters = ContainsAllLetters(String, "key");
            Console.WriteLine(containsAllLetters ? "All letters from 'key' are present in the text." : "Not all letters from 'key' are present in the text.");
            bool bracketsCorrect = CheckBrackets(String);
            Console.WriteLine(bracketsCorrect ? "Brackets are correctly placed." : "Brackets are not correctly placed.");
        }

        static int CountCharOccurrences(string str, char c)
        {
            int count = 0;
            char[] strArray = str.ToCharArray();
            foreach (char ch in strArray)
            {
                if (ch == c)
                {
                    count++;
                }
            }
            return count;
        }

        static string RemoveDigits(string str)
        {
            string stringWithoutDigits = str;
            foreach (char c in str)
            {
                if (char.IsDigit(c))
                {
                    stringWithoutDigits = stringWithoutDigits.Replace(c.ToString(), "");
                }
            }
            return stringWithoutDigits;
        }

        static bool ContainsAllLetters(string str, string word)
        {
            bool containsAllLetters = true;
            foreach (char c in word)
            {
                if (!str.Contains(c))
                {
                    containsAllLetters = false;
                    break;
                }
            }
            return containsAllLetters;
        }

        static bool CheckBrackets(string str)
        {
            int openBrackets = 0;
            bool bracketsCorrect = true;
            foreach (char c in str)
            {
                if (c == '(')
                {
                    openBrackets++;
                }
                else if (c == ')')
                {
                    if (openBrackets > 0)
                    {
                        openBrackets--;
                    }
                    else
                    {
                        bracketsCorrect = false;
                        break;
                    }
                }
            }
            if (openBrackets != 0)
            {
                bracketsCorrect = false;
            }
            return bracketsCorrect;
        }
    }
}