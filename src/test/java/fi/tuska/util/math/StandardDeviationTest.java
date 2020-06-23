package fi.tuska.util.math;

import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import junit.framework.TestCase;
import fi.tuska.util.Triple;

public class StandardDeviationTest extends TestCase {

    /**
     * Values: Triple<Mean, Deviation, SampleDeviation>
     */
    private static final Map<double[], Triple<Double, Double, Double>> devs = new HashMap<double[], Triple<Double, Double, Double>>();

    static {
        devs.put(new double[] { 4, 9, 11, 12, 17, 5, 8, 12, 14 },
            new Triple<Double, Double, Double>(10.222222, 3.937788, 4.17665));

        devs.put(new double[] { 73, 58, 67, 93, 33, 18, 147 },
            new Triple<Double, Double, Double>(69.8571, null, 42.2154));

    }

    public void testPredefined() {
        for (Entry<double[], Triple<Double, Double, Double>> e : devs.entrySet()) {
            double[] values = e.getKey();
            Triple<Double, Double, Double> exp = e.getValue();
            Double expectedMean = exp.getFirst();
            Double expectedDeviance = exp.getSecond();
            Double expectedSampleDeviance = exp.getThird();

            StandardDeviation d = new StandardDeviation();
            for (double v : values)
                d.add(v);

            if (expectedMean != null)
                assertEquals(expectedMean, d.getMean(), 0.0001);
            if (expectedDeviance != null)
                assertEquals(expectedDeviance, d.getDeviation(), 0.0001);
            if (expectedSampleDeviance != null)
                assertEquals(expectedSampleDeviance, d.getSampleDeviation(), 0.0001);

        }

    }

    public void testDev() {
        StandardDeviation dev = new StandardDeviation();
        assertEquals(0.0d, dev.getDeviation());
        assertEquals(0.0d, dev.getMean());
        assertEquals(0.0d, dev.getMin());
        assertEquals(0.0d, dev.getMax());

        assertFalse(dev.hasValues());
        dev.addAll(2, 4, 4, 4, 5, 5, 7, 9);
        assertTrue(dev.hasValues());
        assertEquals(5.0d, dev.getMean(), 0.0001);
        assertEquals(2.0d, dev.getDeviation(), 0.0001);
        assertEquals(2.0d, dev.getMin());
        assertEquals(9.0d, dev.getMax());

        dev.reset();
        dev.add(3);
        dev.add(7);
        dev.add(7);
        dev.add(19);
        assertEquals(9.0d, dev.getMean(), 0.0001);
        assertEquals(6.0d, dev.getDeviation(), 0.0001);
        assertEquals(3.0d, dev.getMin());
        assertEquals(19.0d, dev.getMax());

        dev.reset();
        assertFalse(dev.hasValues());
        dev.add(-3);
        assertTrue(dev.hasValues());
        dev.add(-7);
        dev.add(-7);
        dev.add(-19);
        assertTrue(dev.hasValues());
        assertEquals(-9.0d, dev.getMean(), 0.0001);
        assertEquals(6.0d, dev.getDeviation(), 0.0001);
        assertEquals(-19.0d, dev.getMin());
        assertEquals(-3.0d, dev.getMax());
    }

}
