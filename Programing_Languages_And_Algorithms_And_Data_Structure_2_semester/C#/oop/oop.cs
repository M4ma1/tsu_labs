using System;

namespace Rusik{
    class Worker 
    {
        public int age=0;
        public string name; 
        int _experience=0;
        public Worker(int age, string name)  //конструктор с аргументом
        {
            this.age = age;
            this.name = name;
        }

        public int experience{
            get{ return _experience;}
            set{
                if(experience>100 || experience < 0){
                    throw;
                }
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
    
        public override string ToString(){
            return $"({X}, {Y})";
        }
    }

    public abstract class Figure{
        abstract double square();
        abstract double perimeter();
    }

    class Cirlce: Figure{
        int x, y, r;
        public Circle(int a, int b, int r)
        {
            this.x=a;
            this.y=b;
            this.r=r;
        }
        public override double square()
        {
             return Convert.ToInt32(Math.PI * r * r);
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
            if (Math.Pow(a, 2) + Math.Pow(b, 2) != Math.Pow(c, 2) &&
                Math.Pow(a, 2) + Math.Pow(c, 2) != Math.Pow(b, 2) &&
                Math.Pow(b, 2) + Math.Pow(c, 2) != Math.Pow(a, 2))
            {
                throw new ArgumentException("The given sides do not form a right triangle.");
            }
        }
    }
    
}