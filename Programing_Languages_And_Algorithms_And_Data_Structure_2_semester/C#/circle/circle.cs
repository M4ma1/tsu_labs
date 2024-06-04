using System;

namespace Rusik {
    class TestRunner {
      
        public static void RunTests()
        {
            TestCircleInput();
            TestCircleOutput();
            TestCircleSquare();
            TestCircleCircumference();
            TestCircleMul();
            TestCircleEquation();
            TestCircleContainsPoint();
            TestCircleIntersection();
            TestCircleContains();
            TestCircleRadius();
        }

        private static void AssertEqual(double expected, double actual, double tolerance = 0.0001)
        {
            if (Math.Abs(expected - actual) > tolerance)
            {
                throw new Exception($"Assertion failed. Expected: {expected}, Actual: {actual}");
            }
        }

        private static void AssertTrue(bool condition)
        {
            if (!condition)
            {
                throw new Exception("Assertion failed. Condition is not true.");
            }
        }

        private static void AssertFalse(bool condition)
        {
            if (condition)
            {
                throw new Exception("Assertion failed. Condition is not false.");
            }
        }

        private static void TestCircleInput()
        {
            var circle = new Circle(0, 0, 5);
            AssertEqual(5, circle.r);

            try
            {
                new Circle(0, 0, -5);
                throw new Exception("Expected exception for negative radius not thrown.");
            }
            catch (Exception) { }
        }

        private static void TestCircleOutput()
        {
            var circle = new Circle(0, 0, 5);
            AssertEqual(5, circle.r);
        }

        private static void TestCircleSquare()
        {
            var circle = new Circle(0, 0, 5);
            AssertEqual(Math.PI * 25, circle.square());
        }

        private static void TestCircleCircumference()
        {
            var circle = new Circle(0, 0, 5);
            AssertEqual(2 * Math.PI * 5, Circle.circumference(circle));
        }

        private static void TestCircleMul()
        {
            var circle = new Circle(0, 0, 5);
            var newCircle = Circle.mul(circle, 2);
            AssertEqual(10, newCircle.r);
        }

        private static void TestCircleEquation()
        {
            var circle = new Circle(0, 0, 5);

            var circle2 = new Circle(1, 2, 3);
            var circle3 = new Circle(0,0,3);
            AssertFalse(Circle.equation(circle, circle2));
            AssertTrue(Circle.equation(circle2, circle3));
        }

        private static void TestCircleContainsPoint()
        {
            var circle = new Circle(0, 0, 5);
            AssertTrue(Circle.containsPoint(3, 4, circle));
            AssertTrue(Circle.containsPoint(0, 0, circle));
            AssertFalse(Circle.containsPoint(6, 0, circle));
        }

        private static void TestCircleIntersection()
        {
            var circle1 = new Circle(0, 0, 5);
            var circle2 = new Circle(8, 0, 5);
            var circle3 = new Circle(11, 0, 5);
            var circle4 = new Circle(0, 6, 1);
            var circle5 = new Circle(0, 0, 5);

            AssertTrue(Circle.intersection(circle1, circle2));
            AssertFalse(Circle.intersection(circle1, circle3));
            AssertTrue(Circle.intersection(circle1, circle4));
            AssertTrue(Circle.intersection(circle1, circle5));

        }

        private static void TestCircleContains()
        {
            var circle1 = new Circle(0, 0, 10);
            var circle2 = new Circle(0, 0, 5);
            var circle3 = new Circle(8, 0, 5);

            AssertTrue(Circle.contains(circle1, circle2));
            AssertFalse(Circle.contains(circle1, circle3));
        }

        private static void TestCircleRadius()
        {
            var circle = new Circle(0, 0, 5);
            AssertEqual(5, circle.r);

            var newCircle = Circle.mul(circle, 2);
            AssertEqual(10, newCircle.r);
        }
}

    interface Figure{
        double square();
        void input();
        void output();
    }
    public class Circle : Figure{
        public static void Main(string[] args){
            TestRunner.RunTests();
        }
        double _x, _y, _r;
        public Circle(double center_x=1, double center_y=1, double radius=1){
            this._x = center_x;
            this._y = center_y;
            if(radius <= 0){throw new Exception("Negative or null radius");}
            this._r = radius;
        }
        public double x{
            get{return _x;}
            set{_x = value;}
        }
        public double y{
            get{return _y;}
            set{_y = value;}
        }
        public double r{
            get{return _r;}
            set{
                if (value <= 0){throw new Exception("Negative or null radius");} 
                _r = value;}
        }
        void Figure.input(){
            Console.WriteLine("input x coordinate: ");
            x = Convert.ToDouble(Console.ReadLine());
            Console.WriteLine("input y coordinate: ");
            y = Convert.ToDouble(Console.ReadLine());
            Console.WriteLine("input radius: ");
            r = Math.Abs(Convert.ToDouble(Console.ReadLine()));
        }
        void Figure.output(){
            Console.WriteLine("x coordinate: {0}", x);
            Console.WriteLine("y coordinate: {0}", y);
            Console.WriteLine("r coordinate: {0}", r);
        }
        public double square(){
            return Math.PI * r * r;
        }
        public static double circumference(Circle circle){
            return 2 * Math.PI * circle.r;
        }
        public static Circle mul(Circle circle, int n){
            circle.r *= n;
            return circle;
        }
        public static bool equation(Circle circle, Circle circle2){
            return circle.r == circle2.r;
        }
        public static bool containsPoint(double xPoint, double yPoint, Circle circle){
            double distance = Math.Sqrt((xPoint - circle.x)*(xPoint - circle.x) + (yPoint - circle.y)*(yPoint - circle.y));
            return distance <= circle.r;
        }
        public static bool intersection(Circle circle1, Circle circle2){
            double tmp = (circle2.x+circle1.x)*(circle2.x+circle1.x) + (circle2.y+circle1.y)*(circle2.y+circle1.y);
            return ((circle1.r-circle2.r)*(circle1.r-circle2.r) <= tmp && tmp <= (circle2.r+circle1.r)*(circle2.r+circle1.r));
        }

        public static bool contains(Circle circle1, Circle circle2){
            if (circle1.r > circle2.r){
                double d = Math.Sqrt((circle1.x - circle2.x)*(circle1.x - circle2.x) + (circle1.y - circle2.y)*(circle1.y - circle2.y));
                return circle1.r > d + circle2.r;
            }
            else{
                double d = Math.Sqrt((circle2.x - circle1.x)*(circle2.x - circle1.x) + (circle2.y - circle1.y)*(circle2.y - circle1.y));
                return circle2.r > d + circle1.r;
            }
        }

        public static float radius(Circle circle1, Circle circle2){
            if(intersection(circle1, circle2)){
                double d = Math.Sqrt((circle1.x - circle2.x)*(circle1.x - circle2.x) + (circle1.y - circle2.y)*(circle1.y - circle2.y));
                return (float)Convert.ToDouble((circle1.r + circle2.r - d))/2;
            }
            return 0;
        }
    }
}