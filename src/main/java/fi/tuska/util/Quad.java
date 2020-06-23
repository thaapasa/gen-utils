package fi.tuska.util;

public class Quad<A, B, C, D> extends Triple<A, B, C> {

    public Quad(A first, B second, C third, D fourth) {
        super(first, second, third, fourth);
    }

    @SuppressWarnings("unchecked")
    public D getFourth() {
        return (D) super.get(3);
    }

    public void setFourth(D value) {
        super.set(3, value);
    }

    @Override
    public Quad<A, B, C, D> clone() {
        return new Quad<A, B, C, D>(getFirst(), getSecond(), getThird(), getFourth());
    }

}
