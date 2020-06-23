package fi.tuska.util;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.TreeMap;
import java.util.Map.Entry;

import fi.tuska.util.iterator.EmptyIterator;

/**
 * Like a sorted map but allows duplicate entries. Does not allow null
 * keys/values.
 * 
 * Note that this class is not thread-safe!
 * 
 * @author thaapasa
 * 
 * @param <K> the type of keys
 * @param <V> the type or values
 */
public class SortedList<K extends Comparable<K>, V> implements Iterable<Pair<K, V>> {

    private int entryCount = 0;
    private TreeMap<K, LinkedList<V>> map = new TreeMap<K, LinkedList<V>>();

    public SortedList() {
    }

    public void clear() {
        map.clear();
        entryCount = 0;
    }

    /**
     * @return true if this list contains any value with the given key
     */
    public boolean contains(K key) {
        // Null key not permitted
        if (key == null)
            return false;
        return map.containsKey(key);
    }

    /**
     * @return the size of the list, counting the duplicates as separate
     * entries
     */
    public int size() {
        return entryCount;
    }

    public boolean isEmpty() {
        return map.isEmpty();
    }

    /**
     * @return the first key in the list
     */
    public K getFirstKey() {
        if (map.isEmpty())
            return null;
        return map.firstKey();
    }

    /**
     * @return the last key in the list
     */
    public K getLastKey() {
        if (map.isEmpty())
            return null;
        return map.lastKey();
    }

    /**
     * @return the value at the given index
     */
    public Pair<K, V> getAt(int index) {
        if (index < 0 || index >= entryCount)
            throw new IndexOutOfBoundsException("Index " + index + " out of bounds");

        int c = index;
        for (Entry<K, LinkedList<V>> e : map.entrySet()) {
            List<V> list = e.getValue();
            if (c < list.size())
                return new Pair<K, V>(e.getKey(), list.get(c));
            c -= list.size();
        }
        assert false : "Entry at " + index + " not found, this is a bug";
        throw new IllegalStateException();
    }

    /**
     * @return always a non-null list
     */
    public List<Pair<K, V>> getAll() {
        List<Pair<K, V>> values = new ArrayList<Pair<K, V>>(entryCount);
        for (Pair<K, V> entry : this) {
            values.add(entry);
        }
        return values;
    }

    /**
     * @return always a non-null list
     */
    public List<V> getAll(K key) {
        // Null key not permitted
        if (key == null)
            return new ArrayList<V>();
        List<V> values = map.get(key);
        return values != null ? values : new LinkedList<V>();
    }

    /**
     * @return null if not found
     */
    public V getFirst(K key) {
        // Null key not permitted
        if (key == null)
            return null;
        return CollectionUtils.getFirst(getAll(key));
    }

    /**
     * @return null if not found
     */
    public V getLast(K key) {
        // Null key not permitted
        if (key == null)
            return null;
        LinkedList<V> list = (LinkedList<V>) getAll(key);
        return list != null ? list.getLast() : null;
    }

    /**
     * From the list of values such that the key is the greatest key that is
     * less than or equal to the given key; returns the last such value.
     * 
     * Example: If stored keys are 2, 6, etc., then getFirstFloor(4) returns
     * the first value associated with the key 2. Also, getFirstFloor(2) = 2.
     * 
     * @return null if not found
     */
    public Pair<K, V> getLastFloor(K key) {
        Entry<K, LinkedList<V>> entry = map.floorEntry(key);
        if (entry == null)
            return null;
        List<V> list = entry.getValue();
        assert list != null && !list.isEmpty();
        return new Pair<K, V>(entry.getKey(), CollectionUtils.getLast(list));
    }

    /**
     * Returns the list of values with the greatest key less than or equal to
     * the given key.
     * 
     * @return null if not found
     */
    public Pair<K, List<V>> getAllFloor(K key) {
        Entry<K, LinkedList<V>> entry = map.floorEntry(key);
        if (entry == null)
            return null;
        List<V> list = entry.getValue();
        assert list != null && !list.isEmpty();
        return new Pair<K, List<V>>(entry.getKey(), list);
    }

    /**
     * From the list of values such that the key is the least key that is
     * greater than or equal to the given key; returns the first such value.
     * 
     * Example: If stored keys are 2, 6, etc., then getLastCeiling(4) returns
     * the first value associated with the key 6. Also, getLastCeiling(6) = 6.
     * 
     * @return null if not found
     */
    public Pair<K, V> getFirstCeiling(K key) {
        Entry<K, LinkedList<V>> entry = map.ceilingEntry(key);
        if (entry == null)
            return null;
        List<V> list = entry.getValue();
        assert list != null && !list.isEmpty();
        return new Pair<K, V>(entry.getKey(), CollectionUtils.getFirst(list));
    }

    /**
     * Returns the list of values with the least key greater than or equal to
     * the given key.
     * 
     * @return null if not found
     */
    public Pair<K, List<V>> getAllCeiling(K key) {
        Entry<K, LinkedList<V>> entry = map.ceilingEntry(key);
        if (entry == null)
            return null;
        List<V> list = entry.getValue();
        assert list != null && !list.isEmpty();
        return new Pair<K, List<V>>(entry.getKey(), list);
    }

    /**
     * @throw IllegalArgumentException if key or value is null
     */
    public void add(K key, V value) {
        if (key == null || value == null)
            throw new IllegalArgumentException("Key or value null: " + key + ": " + value);
        LinkedList<V> list = map.get(key);
        if (list == null) {
            list = new LinkedList<V>();
            map.put(key, list);
        }

        // Will add to the beginning of the list if the items are not
        // comparable
        int pos = 0;
        // Try to get the comparable list and search for the insertion
        // position
        List<? extends Comparable<V>> cList = getComparableList(list);
        if (cList != null) {
            pos = Collections.binarySearch(cList, value);
            if (pos < 0) {
                pos = -pos - 1;
            }
        }
        list.add(pos, value);
        entryCount++;
    }

    @SuppressWarnings("unchecked")
    private List<? extends Comparable<V>> getComparableList(List<V> list) {
        if (list == null || list.isEmpty())
            return null;
        if (list.get(0) instanceof Comparable)
            return (List<? extends Comparable<V>>) list;
        else
            return null;
    }

    /**
     * @return the value removed; or null if not found in list
     */
    public V removeFirst(K key) {
        // Null key not permitted
        if (key == null)
            return null;
        LinkedList<V> list = map.get(key);
        if (list == null)
            return null;
        assert !list.isEmpty();
        // Remove the first entry from the list
        V removed = list.removeFirst();
        // Decrease entry count
        entryCount--;
        // Check if list became empty
        if (list.isEmpty())
            map.remove(key);
        return removed;
    }

    public boolean remove(K key, V value) {
        // Null key not permitted
        if (key == null)
            return false;

        LinkedList<V> list = map.get(key);
        if (list == null)
            return false;
        assert !list.isEmpty();
        // Try to find and remove the value (returns false if value not found)
        if (!list.remove(value))
            return false;
        // Value found and removed
        // Decrease entry count
        entryCount--;
        // Check if list became empty
        if (list.isEmpty())
            map.remove(key);

        return true;
    }

    @Override
    public String toString() {
        StringBuilder b = new StringBuilder();
        b.append("[");
        boolean first = true;
        for (Entry<K, LinkedList<V>> entry : map.entrySet()) {
            if (!first) {
                b.append(",");
            }
            b.append(entry.getKey());
            b.append(":{");
            boolean firstVal = true;
            for (V val : entry.getValue()) {
                if (!firstVal) {
                    b.append(",");
                }
                b.append(val);
                firstVal = false;
            }
            b.append("}");
            first = false;
        }
        b.append("]");
        return b.toString();
    }

    /**
     * @return always a non-null iterator over the values with the given key
     */
    public Iterator<V> iterator(final K key) {
        if (key == null)
            return new EmptyIterator<V>();
        final List<V> list = map.get(key);
        if (list == null)
            return new EmptyIterator<V>();
        // Return the list iterator, note that the remove() method must be
        // overridden to also decrease the entry count
        return new Iterator<V>() {
            private Iterator<V> it = list.iterator();

            @Override
            public boolean hasNext() {
                return it.hasNext();
            }

            @Override
            public V next() {
                return it.next();
            }

            @Override
            public void remove() {
                it.remove();
                entryCount--;
                // Need to check if this list has been totally emptied
                if (list.isEmpty()) {
                    map.remove(key);
                }
            }
        };
    }

    /**
     * @return the first entry in this sorted list
     */
    public Pair<K, V> firstEntry() {
        if (map.isEmpty())
            return null;
        return getFirstFromEntry(map.firstEntry());
    }

    /**
     * @return the last entry in this sorted list
     */
    public Pair<K, V> lastEntry() {
        if (map.isEmpty())
            return null;
        return getLastFromEntry(map.lastEntry());
    }

    private Pair<K, V> getFirstFromEntry(Entry<K, LinkedList<V>> entry) {
        if (entry == null)
            return null;
        assert !entry.getValue().isEmpty() : "Empty list for key " + entry.getKey();
        return new Pair<K, V>(entry.getKey(), CollectionUtils.getFirst(entry.getValue()));
    }

    private Pair<K, V> getLastFromEntry(Entry<K, LinkedList<V>> entry) {
        if (entry == null)
            return null;
        assert !entry.getValue().isEmpty() : "Empty list for key " + entry.getKey();
        return new Pair<K, V>(entry.getKey(), CollectionUtils.getLast(entry.getValue()));
    }

    /**
     * Returns the next key-value pair from the list given an existing
     * key-value pair.
     * 
     * Will fail (and return null) if the given key-value pair is not present.
     */
    public Pair<K, V> nextEntry(K key, V value) {
        List<V> list = map.get(key);
        if (list == null)
            return null;
        // Find correct position at list
        boolean found = false;
        for (V cur : map.get(key)) {
            if (found) {
                // Found during previous iteration
                return new Pair<K, V>(key, cur);
            }
            if (cur.equals(value)) {
                // Mark found, continue to next
                found = true;
            }
        }
        if (!found)
            return null;
        // At this point, the item was found but it was the last item in the
        // list, so we must get the list with the next key
        return getFirstFromEntry(map.higherEntry(key));
    }

    /**
     * Returns the previous key-value pair from the list given an existing
     * key-value pair.
     * 
     * Will fail (and return null) if the given key-value pair is not present.
     */
    public Pair<K, V> previousEntry(K key, V value) {
        List<V> list = map.get(key);
        if (list == null)
            return null;
        // Find correct position at list
        V prevValue = null;
        for (V cur : map.get(key)) {
            if (cur.equals(value)) {
                if (prevValue != null) {
                    // Return the previous key
                    return new Pair<K, V>(key, prevValue);
                } else {
                    // The first value is the current, so break out
                    break;
                }
            }
            prevValue = cur;
        }
        // At this point, the item was found but it was the first item in the
        // list, so we must get the list with the previous key
        return getLastFromEntry(map.lowerEntry(key));
    }

    private final Converter<Pair<K, V>, V> valueConverter = new Converter<Pair<K, V>, V>() {
        @Override
        public V convert(Pair<K, V> src) {
            return src.getSecond();
        }
    };

    public Iterator<V> contentIterator() {
        return new IteratorWrapper<Pair<K, V>, V>(iterator(), valueConverter);
    }

    /**
     * @return an iterator over the unique keys in this list
     */
    public Iterator<K> keyIterator() {
        return map.keySet().iterator();
    }

    /**
     * @return an iterator over the contents of this list (K, V -pairs)
     */
    @Override
    public Iterator<Pair<K, V>> iterator() {
        return new ListIterator(true);
    }

    /**
     * @return an iterator over the contents of this list (K, V -pairs), in
     * reverse order
     */
    public Iterator<Pair<K, V>> descendingIterator() {
        return new ListIterator(false);
    }

    private class ListIterator implements Iterator<Pair<K, V>> {

        private final Iterator<Entry<K, LinkedList<V>>> backingIterator;
        private Iterator<V> currentListIterator;
        private LinkedList<V> currentList;
        private K currentKey;
        private boolean nextSearched;
        private final boolean forward;

        private ListIterator(boolean forward) {
            this.forward = forward;
            this.backingIterator = (forward ? map.entrySet().iterator() : map.descendingMap()
                .entrySet().iterator());
            findNext();
        }

        private boolean findNext() {
            nextSearched = true;
            while (true) {
                if (currentListIterator != null && currentListIterator.hasNext())
                    return true;
                if (!backingIterator.hasNext()) {
                    currentListIterator = null;
                    currentKey = null;
                    return false;
                }
                Entry<K, LinkedList<V>> cur = backingIterator.next();
                currentKey = cur.getKey();
                currentList = cur.getValue();
                currentListIterator = forward ? currentList.listIterator() : currentList
                    .descendingIterator();
            }
        }

        @Override
        public boolean hasNext() {
            if (!nextSearched)
                findNext();
            return currentListIterator != null && currentListIterator.hasNext();
        }

        @Override
        public Pair<K, V> next() {
            if (!hasNext())
                throw new IllegalStateException("No more entries available");
            assert currentListIterator.hasNext();
            Pair<K, V> result = new Pair<K, V>(currentKey, currentListIterator.next());
            nextSearched = false;
            return result;
        }

        @Override
        public void remove() {
            if (nextSearched)
                throw new IllegalStateException(
                    "hasNext() called before remove(), state has been invalidated");
            currentListIterator.remove();
            entryCount--;
            if (currentList.isEmpty()) {
                // Remove the entire list from the backing iterator
                backingIterator.remove();
            }
        }
    }

}
