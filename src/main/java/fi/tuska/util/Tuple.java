package fi.tuska.util;

import java.util.Arrays;

public class Tuple implements Cloneable, Comparable<Tuple> {

    private Object[] values;

    public Tuple(Object... values) {
        this.values = values;
    }

    public Object get(int index) {
        if (index < 0 || index >= values.length)
            throw new IndexOutOfBoundsException();
        return values[index];
    }

    public void set(int index, Object value) {
        if (index < 0 || index >= values.length)
            throw new IndexOutOfBoundsException();
        values[index] = value;
    }

    @Override
    public String toString() {
        StringBuilder b = new StringBuilder(values.length * 30);
        b.append("<");
        for (int i = 0; i < values.length; i++) {
            if (i != 0)
                b.append(",");
            b.append(values[i]);
        }
        b.append(">");
        return b.toString();
    }

    @Override
    public int hashCode() {
        int code = 0;
        for (int i = 0; i < values.length; i++) {
            code += values[i].hashCode();
        }
        return code;
    }

    @Override
    public boolean equals(Object o) {
        if (!(o instanceof Tuple))
            return false;
        Tuple t = (Tuple) o;
        if (values.length != t.values.length)
            return false;
        for (int i = 0; i < values.length; i++) {
            if (!Equalities.equalsOrNull(values[i], t.values[i]))
                return false;
        }
        return true;
    }

    @Override
    public Tuple clone() {
        Tuple clone = new Tuple();
        clone.values = Arrays.copyOf(values, values.length);
        return clone;
    }

    /**
     * Comparison first by the first value, and then by the second value (if
     * the corresponding values are comparable themselves). If both are equal,
     * then the pairs are considered equal.
     */
    @Override
    @SuppressWarnings("unchecked")
    public int compareTo(Tuple t) {
        if (values.length < t.values.length)
            return -1;
        if (values.length > t.values.length)
            return 1;

        for (int i = 0; i < values.length; i++) {
            if (values[i] == null)
                if (t.values[i] != null)
                    return 1;

            if (values[i] instanceof Comparable) {
                int c = ((Comparable<Object>) values[i]).compareTo(t.values[i]);
                if (c != 0)
                    return c;
            }
        }
        return equals(t) ? 0 : -1;
    }

}
