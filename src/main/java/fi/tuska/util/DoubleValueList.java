package fi.tuska.util;

import java.util.Iterator;

/**
 * @param <K> the key type, used as key in the backing sorted list
 * @param <V1> the first value type
 * @param <V2> the second value type
 */
public class DoubleValueList<K extends Comparable<K>, V1, V2> extends SortedList<K, Pair<V1, V2>> {

    public DoubleValueList() {
    }

    public void add(K key, V1 value1, V2 value2) {
        if (value1 == null || value2 == null)
            throw new IllegalArgumentException("One of the values is null: " + value1 + ", "
                + value2);
        add(key, new Pair<V1, V2>(value1, value2));
    }

    public boolean remove(K key, V1 value1, V2 value2) {
        if (value1 == null || value2 == null)
            return false;
        return remove(key, new Pair<V1, V2>(value1, value2));
    }

    /**
     * Removes any entry with the given key from the sorted list whose first
     * value is equal to the given value.
     * 
     * @return null if not found
     */
    public Pair<V1, V2> removeByFirst(K key, V1 value) {
        for (Iterator<Pair<V1, V2>> it = iterator(key); it.hasNext();) {
            Pair<V1, V2> current = it.next();
            if (value.equals(current.getFirst())) {
                it.remove();
                return current;
            }
        }
        return null;
    }

    /**
     * Removes any entry with the given key from the sorted list whose second
     * value is equal to the given value.
     * 
     * @return null if not found
     */
    public Pair<V1, V2> removeBySecond(K key, V2 value) {
        for (Iterator<Pair<V1, V2>> it = iterator(key); it.hasNext();) {
            Pair<V1, V2> current = it.next();
            if (value.equals(current.getSecond())) {
                it.remove();
                return current;
            }
        }
        return null;
    }

    /**
     * Returns a value with the given key and whose first value equals the
     * given value.
     * 
     * Note that you can update the values in the pair; the updates will be
     * reflected back into this list.
     * 
     * @return null if not found
     */
    public Pair<V1, V2> getByFirst(K key, V1 value) {
        for (Iterator<Pair<V1, V2>> it = iterator(key); it.hasNext();) {
            Pair<V1, V2> current = it.next();
            if (value.equals(current.getFirst())) {
                return current;
            }
        }
        return null;
    }

    /**
     * Returns a value with the given key and whose second value equals the
     * given value.
     * 
     * Note that you can update the values in the pair; the updates will be
     * reflected back into this list.
     * 
     * @return null if not found
     */
    public Pair<V1, V2> getBySecond(K key, V2 value) {
        for (Iterator<Pair<V1, V2>> it = iterator(key); it.hasNext();) {
            Pair<V1, V2> current = it.next();
            if (value.equals(current.getSecond())) {
                return current;
            }
        }
        return null;
    }

    private final Converter<Pair<K, Pair<V1, V2>>, Pair<K, V1>> convertEntryToFirst = new Converter<Pair<K, Pair<V1, V2>>, Pair<K, V1>>() {
        @Override
        public Pair<K, V1> convert(Pair<K, Pair<V1, V2>> v) {
            return new Pair<K, V1>(v.getFirst(), v.getSecond().getFirst());
        }
    };

    public Iterator<Pair<K, V1>> firstIterator() {
        return new IteratorWrapper<Pair<K, Pair<V1, V2>>, Pair<K, V1>>(iterator(),
            convertEntryToFirst);
    }

}
