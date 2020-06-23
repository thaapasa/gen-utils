package fi.tuska.util;

public class Triple<A, B, C> extends Pair<A, B> {

    public Triple(A first, B second, C third) {
        super(first, second, third);
    }

    /**
     * For overriding classes
     */
    protected Triple(Object... values) {
        super(values);
    }

    @SuppressWarnings("unchecked")
    public C getThird() {
        return (C) super.get(2);
    }

    public void setThird(C value) {
        super.set(2, value);
    }

    @Override
    public Triple<A, B, C> clone() {
        return new Triple<A, B, C>(getFirst(), getSecond(), getThird());
    }

}
