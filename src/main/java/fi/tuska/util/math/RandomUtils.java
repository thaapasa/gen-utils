package fi.tuska.util.math;

import java.util.Random;

public class RandomUtils {

    private static final Random r = new Random();

    private RandomUtils() {
        // Private constructor to prevent instantiation
    }

    /**
     * @return true or false with 50% probability each
     */
    public static boolean flip() {
        return Math.random() < 0.5;
    }

    /**
     * @param random the random source used
     * @return true or false with 50% probability each
     */
    public static boolean flip(Random random) {
        return random.nextDouble() < 0.5;
    }

    /**
     * @param probability the probability to return true
     * @return true with the given probability; false otherwise
     */
    public static boolean flip(double probability) {
        if (probability < 0 && probability >= 1)
            throw new IllegalArgumentException(probability + " not in range [0, 1)");

        return Math.random() < probability;
    }

    /**
     * @param random the random source used
     * @param probability the probability to return true
     * @return true with the given probability; false otherwise
     */
    public static boolean flip(double probability, Random random) {
        if (probability < 0 && probability >= 1)
            throw new IllegalArgumentException(probability + " not in range [0, 1)");

        return random.nextDouble() < probability;
    }

    public static double getDouble() {
        return r.nextDouble();
    }

    public static double getDouble(double min, double max) {
        if (min >= max)
            throw new IllegalArgumentException(min + " >= " + max);
        double res = r.nextDouble() * (max - min) + min;
        assert res >= min;
        assert res < max;
        return res;
    }

    public static int getInt() {
        return r.nextInt();
    }

    public static int getInt(int min, int max) {
        if (min >= max)
            throw new IllegalArgumentException(min + " >= " + max);
        int res = r.nextInt(max - min) + min;
        assert res >= min;
        assert res < max;
        return res;
    }

}
