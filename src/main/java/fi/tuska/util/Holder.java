package fi.tuska.util;

/**
 * A holder class for a single value. The value is modifiable.
 * 
 * @author thaapasa
 * 
 * @param <T> the type of the stored value
 */
public class Holder<T> {

    private T value;
    private boolean initialized;

    public Holder() {
        this.value = null;
        this.initialized = false;
    }

    public Holder(T value) {
        this.value = value;
        this.initialized = true;
    }

    public T getValue() {
        return value;
    }

    public void setValue(T value) {
        this.value = value;
        this.initialized = true;
    }

    public boolean isInitialized() {
        return initialized;
    }

    public void clear() {
        this.value = null;
        this.initialized = false;
    }

    @Override
    public String toString() {
        return value.toString();
    }

    public int hashCode() {
        return value != null ? value.hashCode() : 0;
    }

    @SuppressWarnings("unchecked")
    public boolean equals(Object o) {
        if (o == null)
            return false;
        if (value == null)
            return false;
        if (value.equals(o))
            return true;
        if (o instanceof Holder) {
            Holder<T> v = (Holder<T>) o;
            return value.equals(v.value);
        }
        return false;
    }

}
