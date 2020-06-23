package fi.tuska.util;

public class Pair<A, B> extends Tuple {

    public Pair(A first, B second) {
        super(first, second);
    }

    /**
     * For overriding classes
     */
    protected Pair(Object... values) {
        super(values);
    }

    @SuppressWarnings("unchecked")
    public A getFirst() {
        return (A) super.get(0);
    }

    public void setFirst(A value) {
        super.set(0, value);
    }

    @SuppressWarnings("unchecked")
    public B getSecond() {
        return (B) super.get(1);
    }

    public void setSecond(B value) {
        super.set(1, value);
    }

    @Override
    public Pair<A, B> clone() {
        return new Pair<A, B>(getFirst(), getSecond());
    }

}
