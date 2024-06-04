using System;
using Rusik;

class Program{
        static void Main(string[] args)
        {
            // Test Worker class
            TestWorkerClass();
        
            // Test Vector class
            TestVectorClass();
        
            // Test Circle class
            TestCircleClass();
        
            // Test Triangle class
            TestTriangleClass();
        
            // Test RightTriangle class
            TestRightTriangleClass();
        }
        
        static void TestWorkerClass()
        {
            try
            {
                Worker worker1 = new Worker(30, "John Doe", 5);
                Console.WriteLine($"Worker: {worker1.name}, {worker1.age}, {worker1.experience}");
        
                worker1.name = "Jane Smith";
                worker1.age = 35;
                worker1.experience = 10;
                Console.WriteLine($"Worker: {worker1.name}, {worker1.age}, {worker1.experience}");
            }
            catch (ArgumentOutOfRangeException ex)
            {
                Console.WriteLine(ex.Message);
            }
        }
        
        static void TestVectorClass()
        {
            Vector v1 = new Vector(1, 2);
            Vector v2 = new Vector(1, 2);
            Vector v3 = new Vector(3, 4);
        
            Console.WriteLine($"v1 == v2: {v1 == v2}");
            Console.WriteLine($"v1 != v3: {v1 != v3}");
        
            double scalar = 2.0;
            Vector v4 = scalar * v1;
            Console.WriteLine($"v4 = {scalar} * v1: ({v4.X}, {v4.Y})");
        
            Vector v5 = v1 * scalar;
            Console.WriteLine($"v5 = v1 * {scalar}: ({v5.X}, {v5.Y})");
        
            double dotProduct = v1 * v3;
            Console.WriteLine($"v1 · v3 = {dotProduct}");
        
            Vector v6 = v1 - v3;
            Console.WriteLine($"v6 = v1 - v3: ({v6.X}, {v6.Y})");
        }
        
        static void TestCircleClass()
        {
            Circle circle = new Circle(0, 0, 5);
            Console.WriteLine($"Circle: ({circle.x}, {circle.y}), radius = {circle.r}, area = {circle.square()}");
        }
        
        static void TestTriangleClass()
        {
            Triangle triangle = new Triangle(3, 4, 5);
            Console.WriteLine($"Triangle: sides = {triangle.SideA}, {triangle.SideB}, {triangle.SideC}, area = {triangle.square()}, perimeter = {triangle.perimeter()}");
        }
        
        static void TestRightTriangleClass()
        {
            try
            {
                RightTriangle rightTriangle = new RightTriangle(3, 4, 5);
                Console.WriteLine($"Right Triangle: sides = {rightTriangle.SideA}, {rightTriangle.SideB}, {rightTriangle.SideC}, area = {rightTriangle.square()}, perimeter = {rightTriangle.perimeter()}");
            }
            catch (ArgumentException ex)
            {
                Console.WriteLine(ex.Message);
            }
        }
        
    }


namespace Rusik{
    class Worker 
    {
        int _age=0;
        string _name; 
        int _experience=0;
        public Worker(int age, string name, int experience)  //конструктор с аргументом
        {
            this._age = age;
            this._name = name;
            this._experience = experience;
        }
        public string name{
            get{return _name;}
            set{
                if(value.Length<1 || value.Length>100){
                    throw new ArgumentOutOfRangeException(nameof(value), "Name length must be between 0 and 100."); 
                }
                _name = value;
            }
        }
        public int age{
            get{return _age;}
            set{
                if(value>100 || value < 0){
                    throw new ArgumentOutOfRangeException(nameof(value), "Age must be between 0 and 100.");
                }
                _experience = value;
            }
        }
        public int experience{
            get{ return _experience;}
            set{
                if(value>100 || value < 0 || value >= this._age){
                    throw new ArgumentOutOfRangeException(nameof(value), "Experience must be between 0 and 100.");
                }
                _experience = value;
            }
        } 

        public Worker()   // конструктор по умолчанию
        {
            this.age=0;
            this.name="";
        }
    }

    class Vector
    {
        public double X { get; set; }
        public double Y { get; set; }
    
        public Vector(double x, double y){
            X = x;
            Y = y;
        }
    
        public static bool operator ==(Vector v1, Vector v2){
            return v1.X == v2.X && v1.Y == v2.Y;
        }
    
        public static bool operator !=(Vector v1, Vector v2){
            return !(v1 == v2);
        }
    
        public static Vector operator *(double scalar, Vector v){
            return new Vector(scalar * v.X, scalar * v.Y);
        }
    
        public static Vector operator *(Vector v, double scalar){
            return scalar * v;
        }
    
        public static double operator *(Vector v1, Vector v2){
            return v1.X * v2.X + v1.Y * v2.Y;
        }
    
        public static Vector operator -(Vector v1, Vector v2){
            return new Vector(v1.X - v2.X, v1.Y - v2.Y);
        }
    }

    public abstract class Figure{
        public abstract double square();
        public abstract double perimeter();
    }

    class Circle: Figure{
        public double x { get; set; }
        public double y { get; set; }
        public double r { get; set; }
        public Circle(double a, double b, double r1)
        {
            this.x=a;
            this.y=b;
            this.r=r1;
        }
        public override double square()
        {
            return Math.PI * r * r;
        }
        public override global::System.Double perimeter()
        {
            throw new System.NotImplementedException();
        }
    }

    class Triangle : Figure
    {
        public double SideA { get; set; }
        public double SideB { get; set; }
        public double SideC { get; set; }
    
        public Triangle(double a, double b, double c)
        {
            SideA = a;
            SideB = b;
            SideC = c;
        }
    
        public override double square()
        {
            double s = (SideA + SideB + SideC) / 2;
            return Math.Sqrt(s * (s - SideA) * (s - SideB) * (s - SideC));
        }
    
        public override double perimeter()
        {
            return SideA + SideB + SideC;
        }
    }
    
    class RightTriangle : Triangle
    {
        public RightTriangle(double a, double b, double c) : base(a, b, c)
        {
            // Check if the triangle is a right triangle
            if (a*a + b*b != c*c &&
                a*a + c*c != b*b &&
                b*b + c*c != a*a)
            {
                throw new ArgumentException("The given sides do not form a right triangle.");
            }
        }
    }
    
}