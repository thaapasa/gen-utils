package fi.tuska.util;

/**
 * A generic array that wraps an Object-array. Useful if you need arrays that
 * store generic types (can't use normal Java primitive arrays).
 * 
 * @author thaapasa
 * 
 * @param <T>
 */
public class Array<T> {

    private Object[] array;

    public Array(int size) {
        this.array = new Object[size];
    }

    @SuppressWarnings("unchecked")
    public T get(int index) {
        return (T) array[index];
    }

    public void put(int index, T object) {
        array[index] = object;
    }

    @SuppressWarnings("unchecked")
    public static <E> E[] createArray(int size, Class<E> type) {
        return (E[]) java.lang.reflect.Array.newInstance(type, size);
    }
    
}
