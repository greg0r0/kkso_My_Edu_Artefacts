import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

import com.lab9.Complex;
public class ComplexTest {

    Complex cmp;
    @Before
    public void InitComplex()
    {
        cmp = new Complex(5.0, 6.0);
    }

    @Test
    public void GettersTest()
    {
        Complex c = new Complex();
        Assert.assertEquals(0.0, c.getRealPart(), 0.01);
        Assert.assertEquals(0.0, c.getImaginaryPart(), 0.01);
    }

    @Test
    public void ConstructorTest()
    {
        Assert.assertEquals(5.0, cmp.getRealPart(), 0.01);
        Assert.assertEquals(6.0, cmp.getImaginaryPart(), 0.01);
    }

    @Test
    public void ModulusTest()
    {
        Assert.assertEquals(61.0, cmp.modulus(), 0.01);
    }

    @Test
    public void ArgumentTest()
    {
        Assert.assertEquals(Math.atan(6.0/5.0), cmp.argument(), 0.01);
    }

    @Test
    public void AddMethodTest()
    {
        cmp.add(new Complex(6.0, 5.0));
        Assert.assertEquals(11.0, cmp.getRealPart(), 0.01);
        Assert.assertEquals(11.0, cmp.getImaginaryPart(), 0.01);
    }
    
    @Test
    public void SubtractionMethodTest()
    {
        cmp.sub(new Complex(3.0, 4.0));
        Assert.assertEquals(2.0, cmp.getRealPart(), 0.01);
        Assert.assertEquals(2.0, cmp.getImaginaryPart(), 0.01);
    }
}

                  


