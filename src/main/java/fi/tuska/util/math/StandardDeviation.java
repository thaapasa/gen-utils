package fi.tuska.util.math;

public class StandardDeviation {

    private double Ap = 0;
    private double An = 0;

    private double Qp = 0;
    private double Qn = 0;

    private double min;
    private double max;

    private long count = 0;

    public StandardDeviation() {
        reset();
    }

    public void reset() {
        Ap = 0;
        An = 0;
        Qp = 0;
        Qn = 0;
        count = 0;
        min = 0;
        max = 0;
    }

    public boolean hasValues() {
        return count > 0;
    }

    public void add(double value) {
        if (value < min || count == 0)
            min = value;
        if (value > max || count == 0)
            max = value;

        Ap = An;
        Qp = Qn;
        count++;
        // A0 = 0
        // Ai = Ai-1 + (xi - Ai-1)/i
        An = Ap + (value - Ap) / count;

        // Q0 = 0
        // Qi = Qi-1 + (xi - Ai-1)(xi - Ai)
        Qn = Qp + (value - Ap) * (value - An);
    }

    public void addAll(double... values) {
        for (double d : values) {
            add(d);
        }
    }

    /**
     * @return the population standard deviation &sigma; of the data set
     */
    public double getDeviation() {
        if (count == 0)
            return 0;
        return Math.sqrt(Qn / count);
    }

    /**
     * @return the sample standard deviation <i>s</i> of the data set
     */
    public double getSampleDeviation() {
        if (count == 0)
            return 0;
        return Math.sqrt(Qn / (count - 1));
    }

    public double getMean() {
        return An;
    }

    public double getMin() {
        return min;
    }

    public double getMax() {
        return max;
    }

}
