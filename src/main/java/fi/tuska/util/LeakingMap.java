package fi.tuska.util;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

/**
 * Implements a map with a maximum size. If the size is exceeded, the oldest
 * value in the map is removed from the map.
 * 
 * @author thaapasa
 * 
 * @param <T>
 */
public class LeakingMap<K, V> {

    private Map<K, MapEntry> map = new HashMap<K, MapEntry>();
    private LinkedList<K> usageList = new LinkedList<K>();
    private int maxSize;
    private final List<LeakEventListener<V>> leakListeners = new ArrayList<LeakEventListener<V>>();

    public LeakingMap(int size) {
        this.maxSize = size;
    }

    public synchronized Set<K> keySet() {
        return map.keySet();
    }

    public void addLeakEventListener(LeakEventListener<V> listener) {
        leakListeners.add(listener);
    }

    public synchronized void setMaximumSize(int size) {
        this.maxSize = size;
        ensureCapacity();
    }

    public int getMaximumSize() {
        return maxSize;
    }

    public int getSize() {
        return map.size();
    }

    private synchronized void ensureCapacity() {
        while (map.size() > maxSize) {
            // Take the least recently used key from the usage list
            LinkedListNode<K> last = usageList.removeLastNode();
            K removeKey = last.getValue();
            // Remove the key from the backing map
            MapEntry entry = map.remove(removeKey);
            for (LeakEventListener<V> listener : leakListeners) {
                listener.itemLeaked(entry.value);
            }
            entry.clear();
        }
    }

    /**
     * Sets the object with the given key. Any existing value with same key
     * will be overridden. This method will remove the oldest value if the
     * size of the backing map exceeds the capacity. The value is marked as
     * being the newest value.
     * 
     * @param key the key
     * @param value the object
     */
    public synchronized void put(K key, V value) {
        MapEntry entry = map.get(key);
        if (entry == null) {
            LinkedListNode<K> node = usageList.addFirstNode(key);
            entry = new MapEntry(value, node);
            map.put(key, entry);
            ensureCapacity();
        } else {
            entry.node.moveToHead();
            entry.value = value;
        }
    }

    /**
     * Returns the value with the given key. The returned value is marked as
     * being the newest value.
     * 
     * @param key the key
     * @return the value with the given key, or null, if no such value is
     * found.
     */
    public synchronized V get(K key) {
        MapEntry entry = map.get(key);
        if (entry == null) {
            return null;
        }
        // Move the entry to the front of the queue
        entry.node.moveToHead();
        return entry.value;
    }

    public interface LeakEventListener<V> {
        public void itemLeaked(V item);
    }

    private class MapEntry {
        public V value;
        public LinkedListNode<K> node;

        public MapEntry(V value, LinkedListNode<K> node) {
            this.value = value;
            this.node = node;
        }

        public void clear() {
            this.value = null;
            this.node = null;
        }
    }

}
