package fi.tuska.util.math;

public class MinMaxInt {

    private boolean initialized = false;
    private final boolean acceptInfty;
    private int min = Integer.MAX_VALUE;
    private int max = Integer.MIN_VALUE;

    public MinMaxInt(boolean acceptInfty) {
        this.acceptInfty = acceptInfty;
    }

    public MinMaxInt(int initialValue, boolean acceptInfty) {
        this.acceptInfty = acceptInfty;
        process(initialValue);
    }

    public void process(int value) {
        if (!acceptInfty) {
            if (value == Integer.MAX_VALUE || value == Integer.MIN_VALUE)
                return;
        }
        if (!initialized) {
            min = value;
            max = value;
            initialized = true;
        } else {
            if (value < min)
                min = value;
            if (value > max)
                max = value;
        }
    }

    public int getMin() {
        if (!initialized)
            throw new IllegalStateException("Counter not yet initialized");
        return min;
    }

    public int getMax() {
        if (!initialized)
            throw new IllegalStateException("Counter not yet initialized");
        return max;
    }

    public boolean isInitialized() {
        return initialized;
    }
}
