package fi.tuska.util.math;

import java.util.Random;

public class GaussianRandom extends Random {

    private static final long serialVersionUID = 1664812780209137936L;

    private final Random source;
    private double widthScale = 0.2;

    private static final float FLOAT_ALMOST_ONE = Float
        .intBitsToFloat(Float.floatToIntBits(1) - 1);
    private static final double DOUBLE_ALMOST_ONE = Double.longBitsToDouble(Double
        .doubleToLongBits(1) - 1L);

    static {
        assert FLOAT_ALMOST_ONE > 0.99;
        assert FLOAT_ALMOST_ONE < 1;
        assert DOUBLE_ALMOST_ONE > 0.99;
        assert DOUBLE_ALMOST_ONE < 1;
    }

    public GaussianRandom() {
        source = new UniformRandom();
    }

    @Override
    public int nextInt(int max) {
        double x = source.nextGaussian();
        double mid = max / 2.0;
        double res = mid + (x * mid * widthScale);
        return Math.max(Math.min((int) res, max - 1), 0);
    }

    @Override
    public int nextInt() {
        return nextInt(Integer.MAX_VALUE);
    }

    @Override
    public long nextLong() {
        double x = source.nextGaussian();
        double mid = Long.MAX_VALUE / 2.0;
        double res = mid + (x * mid * widthScale);
        return Math.max(Math.min((long) res, Long.MAX_VALUE - 1), 0l);
    }

    @Override
    public float nextFloat() {
        double x = source.nextGaussian();
        double mid = 0.5d;
        double res = mid + (x * mid * widthScale);
        return Math.max(Math.min((float) res, FLOAT_ALMOST_ONE), 0f);
    }

    @Override
    public double nextDouble() {
        double x = source.nextGaussian();
        double mid = 0.5d;
        double res = mid + (x * mid * widthScale);
        return Math.max(Math.min(res, DOUBLE_ALMOST_ONE), 0d);
    }

    @Override
    public boolean nextBoolean() {
        return source.nextBoolean();
    }

}
