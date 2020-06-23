package fi.tuska.util;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import java.util.SortedSet;
import java.util.Map.Entry;

import fi.tuska.util.iterator.IterableIterator;

public class CollectionUtils {

    private CollectionUtils() {
        // Private constructor to prevent instantiation
    }

    /**
     * Returns the first item from the given collection (as given by the
     * collection's iterator).
     * 
     * @return the first item; or null, if the collection is null or empty.
     */
    public static <T> T getFirst(Collection<T> collection) {
        if (collection == null || collection.size() < 1)
            return null;
        if (collection instanceof List<?>)
            return ((List<T>) collection).get(0);

        return collection.iterator().next();
    }

    /**
     * Returns the last item from the given list.
     * 
     * @return the last item; or null, if the list is null or empty.
     */
    public static <T> T getLast(List<T> list) {
        if (list == null || list.size() < 1)
            return null;
        return list.get(list.size() - 1);
    }

    /**
     * Wraps the given array in a list.
     * 
     * @return the created list.
     */
    public static <T> List<T> toList(T[] array) {
        List<T> list = new ArrayList<T>(array.length);
        for (T item : array) {
            list.add(item);
        }
        return list;
    }

    /**
     * Convenience method for creating arrays with varargs syntax.
     * 
     * @param <T> the array type
     * @param elements the elements of the array
     * @return the elements in an array
     */
    public static <T> T[] array(T... elements) {
        return elements;
    }

    /**
     * Checks whether the array contains an element that equals to the given
     * element. Checks for nulls also.
     * 
     * @return true if the given array contains an element that equals() to
     * the given element.
     */
    public static <T> boolean contains(T[] array, T element) {
        for (T el : array) {
            if (el == null) {
                if (element == null)
                    return true;
            } else {
                if (el.equals(element))
                    return true;
            }
        }
        return false;
    }

    /**
     * Converts a collection of objects into a string array by calling the
     * toString() method for each object.
     * 
     * @return the string array
     */
    public static <T> String[] toStringArray(Collection<T> list) {
        String[] res = new String[list.size()];
        int i = 0;
        for (T item : list) {
            res[i++] = item.toString();
        }
        return res;
    }

    /**
     * Joins the given values in a single string, separated by the given
     * separator.
     * 
     * @return the created string.
     */
    public static <T> String join(Collection<T> list, String separator) {
        boolean first = true;
        StringBuilder buf = new StringBuilder();
        for (T item : list) {
            if (!first) {
                buf.append(separator);
            }
            buf.append(item.toString());
            first = false;
        }
        return buf.toString();
    }

    /**
     * Finds the last entry in the set that is smaller or equal to the given
     * key, in the order given by set's iterator.
     * 
     * @param set the set
     * @param key the key to look for
     * @return an entry such that the entry's key is the greatest key that is
     * smaller or equal to the given key
     */
    public static <T extends Comparable<T>> T floorEntry(SortedSet<T> set, T key) {
        T lastFound = null;
        for (T actual : set) {
            // if (key < actual)
            if (key.compareTo(actual) < 0) {
                break;
            }
            lastFound = actual;
        }
        return lastFound != null ? lastFound : null;
    }

    /**
     * @return the given values in a collection.
     */
    public static <T> List<T> getList(T... values) {
        List<T> list = new ArrayList<T>(values.length);
        for (T value : values) {
            list.add(value);
        }
        return list;
    }

    /**
     * Creates a list from a given iterator.
     * 
     * @return the given values in a collection.
     */
    public static <T> List<T> getList(Iterator<T> values) {
        return getList(new IterableIterator<T>(values));
    }

    /**
     * Creates a list from a given iterable source.
     * 
     * @return the given values in a collection.
     */
    public static <T> List<T> getList(Iterable<T> values) {
        List<T> list = new ArrayList<T>();
        for (T value : values) {
            list.add(value);
        }
        return list;
    }

    /**
     * @return a list of all the values that are in the first component of the
     * stored pairs
     */
    public static <A, B> List<A> getPairFirstList(final Collection<Pair<A, B>> list) {
        List<A> result = new ArrayList<A>(list.size());
        for (Pair<A, B> entry : list) {
            result.add(entry.getFirst());
        }
        return result;
    }

    /**
     * @return a list of all the values that are in the second component of
     * the stored pairs
     */
    public static <A, B> List<B> getPairSecondList(final Collection<Pair<A, B>> list) {
        List<B> result = new ArrayList<B>(list.size());
        for (Pair<A, B> entry : list) {
            result.add(entry.getSecond());
        }
        return result;
    }

    /**
     * @param entrySet a set of Map.Entries; such that you can get from
     * Map.entrySet()
     * @return a list of all the values in the entries
     */
    public static <K, V> List<V> entrySetToValueList(final Set<Entry<K, V>> entrySet) {
        List<V> result = new ArrayList<V>(entrySet.size());
        for (Entry<K, V> entry : entrySet) {
            result.add(entry.getValue());
        }
        return result;
    }

    @SuppressWarnings("unchecked")
    public static <T> T[] createArray(T proto, int length) {
        return (T[]) Array.newInstance(proto.getClass(), length);
    }

    /**
     * Create a "tail" array from the given array, that is, an array
     * containing all the entries of the source array beginning from the given
     * index.
     * 
     * @return the new array with some items at the beginning skipped
     */
    public static <T> T[] tailArray(T[] array, int fromIndex) {
        if (array == null)
            return null;
        if (fromIndex < 1)
            return array;

        int size = array.length - fromIndex;
        if (size < 0)
            return null;
        T proto = array[0];
        if (size == 0)
            return createArray(proto, 0);
        T[] newAr = createArray(proto, size);
        System.arraycopy(array, fromIndex, newAr, 0, size);
        return newAr;
    }

    /**
     * Adds all entries from the iterator to the given target collection.
     */
    public static <T> void addAll(Collection<T> target, Iterator<T> iterator) {
        while (iterator.hasNext()) {
            target.add(iterator.next());
        }
    }

    /**
     * Adds all entries from the iterator to the given target collection.
     */
    public static <T> void addAll(Collection<T> target, Iterable<T> iterable) {
        for (T val : iterable) {
            target.add(val);
        }
    }

    /**
     * Convenience method for iterating over the values of a map (without the
     * keys).
     */
    public static <K, V> Iterable<V> values(final Iterable<Entry<K, V>> map) {
        return new Iterable<V>() {
            @Override
            public Iterator<V> iterator() {
                final Iterator<Entry<K, V>> backEnd = map.iterator();
                return new Iterator<V>() {
                    @Override
                    public boolean hasNext() {
                        return backEnd.hasNext();
                    }

                    @Override
                    public V next() {
                        return backEnd.next().getValue();
                    }

                    @Override
                    public void remove() {
                        backEnd.remove();
                    }
                };
            }
        };
    }

    /**
     * Converts all entries in the list to the new format.
     */
    public static <S, T> List<T> convertList(List<S> source, Converter<S, T> converter) {
        List<T> result = new ArrayList<T>(source.size());
        for (S src : source) {
            result.add(converter.convert(src));
        }
        return result;
    }

    public static <T> String toString(T[] array) {
        if (array == null)
            return "null";
        StringBuilder b = new StringBuilder();
        b.append("[");
        boolean first = true;
        for (T t : array) {
            if (!first) {
                b.append(", ");
            }
            b.append(t != null ? t.toString() : "null");
            first = false;
        }
        b.append("]");
        return b.toString();
    }

}
