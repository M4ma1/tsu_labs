using System;

namespace Rusik{
    public class Circle{
        public static void Main(string[] args){
            Circle asd = new Circle();
            asd.input();
            Circle qwe = new Circle();
            qwe.input();
            asd.output();
            qwe.output();

            Console.WriteLine(asd.square());
            Console.WriteLine(asd.circumference());
            asd = asd.mul(3);
            asd.output();

            Console.WriteLine(qwe.equation(asd));
            Console.WriteLine(qwe.containsPoint(0, 0));
            Console.WriteLine(qwe.contains(asd));
            Console.WriteLine(asd.intersection(qwe));

            Console.WriteLine(asd.radius(qwe));
        }
        int x, y, r;
        Circle(int center_x=1, int center_y=1, int radius=1){
            x = center_x;
            y = center_y;
            r = radius;
        }
        void input(){
            Console.WriteLine("input x coordinate: ");
            x = Convert.ToInt32(Console.ReadLine());
            Console.WriteLine("input y coordinate: ");
            y = Convert.ToInt32(Console.ReadLine());
            Console.WriteLine("input radius: ");
            r = Math.Abs(Convert.ToInt32(Console.ReadLine()));
        }
        void output(){
            Console.WriteLine("x coordinate: {0}", x);
            Console.WriteLine("y coordinate: {0}", y);
            Console.WriteLine("r coordinate: {0}", r);
        }
        int square(){
            return Convert.ToInt32(Math.PI * r * r);
        }
        int circumference(){
            return Convert.ToInt32(2 * Math.PI * r);
        }
        Circle mul(int n){
            r *= n;
            return this;
        }
        bool equation(Circle circle2){
            return r == circle2.r;
        }
        public bool containsPoint(int xPoint, int yPoint){
            double distance = Math.Sqrt(Math.Pow(xPoint - x, 2) + Math.Pow(yPoint - y, 2));
            return distance <= r;
        }
        bool intersection(Circle circle1){
            int tmp = (x+circle1.x)*(x+circle1.x) + (y+circle1.y)*(y+circle1.y);
            return ((r-circle1.r)*(r-circle1.r) <= tmp && tmp <= (r+circle1.r)*(r+circle1.r));
        }

        bool contains(Circle circle1){
            int d = Convert.ToInt32(Math.Sqrt((x - circle1.x)*(x - circle1.x) + (y - circle1.y)*(y - circle1.y)));
            return r > d + circle1.r;
        }

        float radius(Circle circle1){
            if(intersection(circle1)){
                int d = Convert.ToInt32(Math.Sqrt((x - circle1.x)*(x - circle1.x) + (y - circle1.y)*(y - circle1.y)));
                return (float)Convert.ToDouble((r + circle1.r - d))/2;
            }
            return 0;
        }
    }
}