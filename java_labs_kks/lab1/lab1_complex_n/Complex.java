package lab1_complex_n;

public class Complex{

    private double real_part;
    private double imaginary_part;

    public double getRealPart(){
        return real_part;
    }

    public double getImaginaryPart(){
        return imaginary_part;
    }
    
    public Complex()
    {
        real_part = 0.0;
        imaginary_part = 0.0;
    }

    public Complex(double r_part, double i_part)
    {
        real_part         = r_part;
        imaginary_part    = i_part;
    }

    public double modulus(){
        return (Math.pow(real_part, 2)+Math.pow(imaginary_part, 2));
    }

    public double argument(){
        return Math.atan(imaginary_part/real_part);
    }

    public void add(Complex c){
        real_part      += c.real_part;
        imaginary_part += c.imaginary_part;
    }

    public void sub(Complex c){
        real_part -= c.real_part;
        imaginary_part -= c.imaginary_part;
    }

}