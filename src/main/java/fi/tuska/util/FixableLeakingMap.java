package fi.tuska.util;

import java.io.PrintStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.log4j.Logger;

/**
 * Implements a map with a maximum size. If the size is exceeded, the oldest
 * value in the map is removed from the map. This map further enables the user
 * to fix items in place so that they cannot be purged. Only unfixed entries
 * will be leaked.
 * 
 * @author thaapasa
 */
public class FixableLeakingMap<K, V> {

    private static final Logger log = Logger.getLogger(FixableLeakingMap.class);

    private Map<K, MapEntry> map = new HashMap<K, MapEntry>();
    private LinkedList<K> fixedList = new LinkedList<K>();
    private LinkedList<K> freeList = new LinkedList<K>();
    /** Maximum amount of pages (fixed + unfixed pages <= maxSize) */
    private int maxSize;
    private int freeSize;
    private final List<LeakEventListener<K, V>> leakListeners = new ArrayList<LeakEventListener<K, V>>();

    public FixableLeakingMap(int size) {
        this.maxSize = size;
        this.freeSize = size;
    }

    /**
     * Removes all items from this map.
     */
    public synchronized void clear() {
        map.clear();
        fixedList.clear();
        freeList.clear();
        freeSize = maxSize;
    }

    public synchronized Set<K> keySet() {
        return map.keySet();
    }

    public void addLeakEventListener(LeakEventListener<K, V> listener) {
        leakListeners.add(listener);
    }

    public synchronized void setMaximumSize(int size) {
        this.maxSize = size;
        ensureCapacity();
    }

    /** @return the total size of the map (amount of pages) */
    public int getMaximumSize() {
        return maxSize;
    }

    /** @return the current amount of fixed and unfixed pages */
    public int getSize() {
        assert freeSize <= (maxSize - map.size()) : map.size() + "/" + maxSize + " = " + freeSize
            + " free";
        return map.size();
    }

    public int getFreeSlots() {
        return freeSize;
    }

    /**
     * For debugging; prints the fix state of the map.
     */
    public synchronized void printFixes(PrintStream str) {
        str.println("Fixes:");
        List<K> zeroFixes = new java.util.LinkedList<K>();
        for (java.util.Map.Entry<K, MapEntry> e : map.entrySet()) {
            if (e.getValue().fixcount == 0) {
                zeroFixes.add(e.getKey());
            } else {
                str.println(e.getKey().toString() + ": " + e.getValue().fixcount + " fix(es)");
            }
        }
        if (!zeroFixes.isEmpty())
            str.println("Zero fixes: " + zeroFixes);
        str.println("Total: " + getTotalFixCount());
    }

    public synchronized String getEntryFixSummary() {
        StringBuilder b = new StringBuilder();
        boolean first = true;
        for (java.util.Map.Entry<K, MapEntry> e : map.entrySet()) {
            if (e.getValue().fixcount > 0) {
                if (!first)
                    b.append(", ");
                b.append(e.getKey().toString()).append(":");
                b.append(e.getValue().fixcount);
                first = false;
            }
        }
        return b.toString();
    }

    /**
     * This can be used to check that all the items have been released.
     * 
     * @return the total amount of fixed items (if an item has two fixes, that
     * counts as two fixes)
     */
    public synchronized int getTotalFixCount() {
        int fixes = 0;
        for (java.util.Map.Entry<K, MapEntry> e : map.entrySet()) {
            fixes += e.getValue().fixcount;
        }
        return fixes;
    }

    /**
     * @return the space that can be used (possibly by purging unfixed
     * entries)
     */
    public synchronized int getUnfixedSpace() {
        int bufferFree = getMaximumSize() - getSize();
        return bufferFree + (int) freeList.getSize();
    }

    public synchronized boolean contains(K key) {
        return map.get(key) != null;
    }

    /**
     * Returns the value with the given key. The returned value is fixed and
     * will not be removed from buffer until it has been unfixed.
     * 
     * @param key the key
     * @return the value with the given key, or null, if no such value is
     * found.
     */
    public synchronized V get(K key) {
        MapEntry entry = map.get(key);
        if (entry == null)
            return null;

        fixEntry(entry);
        return entry.value;
    }

    public synchronized boolean isFixed(K key) {
        MapEntry entry = map.get(key);
        if (entry == null)
            return false;
        return entry.fixcount > 0;
    }

    public synchronized int getFixCount(K key) {
        MapEntry entry = map.get(key);
        if (entry == null)
            return 0;
        return entry.fixcount;
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
            ensureCapacity();
            freeSize--;
            LinkedListNode<K> node = fixedList.addFirstNode(key);
            entry = new MapEntry(value, node);
            entry.fixcount = 1;
            map.put(key, entry);
            if (log.isDebugEnabled())
                log.debug("Added entry " + key + "; free: " + freeSize);
        } else {
            throw new IllegalStateException("Cannot overwrite values, remove old values first: "
                + key + ", " + value + "; existing: " + entry);
        }
    }

    /**
     * Ensures that there is space to do (at least) one insert. This is
     * accomplished by possibly purging unfixed entries from the map. If there
     * is no unfixed entries and no free space, throws an
     * IndexOutOfBoundsException.
     */
    public synchronized void ensureCapacity() {
        if (log.isDebugEnabled())
            log.debug("Ensuring capacity: " + map.size() + "/" + maxSize + ", free: " + freeSize);
        while (true) {
            if (getUnfixedSpace() < 1) {
                throw new IndexOutOfBoundsException(
                    "Buffer is full of fixed entries, cannot drop old entries from buffer");
            }
            assert freeSize >= 0;
            assert freeSize <= (maxSize - map.size());
            assert map.size() <= maxSize;
            if (freeSize > 0) {
                // We're done
                if (log.isDebugEnabled())
                    log.debug("Capacity ensured: " + map.size() + "/" + maxSize + ", free: "
                        + freeSize);

                return;
            }
            // freeSize == 0, that is, no free slots currently

            if (freeList.getSize() < 1) {
                assert getSize() == getMaximumSize();
                log.warn("Freelist is empty");
                throw new RuntimeException("Freelist is empty, cannot release any pages");
            }

            removeAnItem();
            // Item leaking might have caused other pages to be loaded so
            // ensure capacity again
        }
    }

    public synchronized V remove(K key) {
        MapEntry entry = map.get(key);
        if (entry == null)
            return null;

        assert entry.node != null;
        if (entry.fixcount != 0) {
            throw new IllegalStateException("Trying to remove item that is fixed");
        }
        V value = entry.value;

        assert entry.node.isInList();
        assert entry.node.isInList(freeList);
        long fSize = freeList.getSize();
        entry.node.remove();
        assert !entry.node.isInList();
        assert freeList.getSize() == fSize - 1 : "freeList size is " + freeList.getSize()
            + " and not " + (fSize - 1);

        // Notify listeners that page has been leaked (to store it to
        // disk, for example)
        for (LeakEventListener<K, V> listener : leakListeners) {
            listener.itemLeaked(key, value);
        }

        // Delete the entry
        MapEntry e2 = map.remove(key);
        assert e2 != null;
        assert e2 == entry;
        assert e2.fixcount == 0;
        e2.clear();

        freeSize++;
        return value;
    }

    private void removeAnItem() {
        // Take the least recently used key from the free item list
        LinkedListNode<K> last = freeList.getLastNode();
        remove(last.getValue());
    }

    private synchronized void fixEntry(MapEntry entry) {
        // fixed + free size can be occasionally smaller than map size if we
        // are in the process of removing entries (which has caused other
        // entries to be loaded)
        assert fixedList.getSize() + freeList.getSize() <= map.size() : fixedList.getSize()
            + " + " + freeList.getSize() + " != " + map.size();
        if (entry.fixcount == 0) {
            // First fix, move entry from freeList to fixedList
            boolean success = entry.node.remove();
            assert success;
            fixedList.addFirst(entry.node);
        } else {
            // Move entry to the top of the fixedList
            boolean res = entry.node.moveToHead();
            assert res;
        }
        entry.fixcount++;
    }

    public synchronized void unfix(K key) {
        // fixed + free size can be occasionally smaller than map size if we
        // are in the process of removing entries (which has caused other
        // entries to be loaded)
        assert fixedList.getSize() + freeList.getSize() <= map.size() : fixedList.getSize()
            + " + " + freeList.getSize() + " != " + map.size();
        MapEntry entry = map.get(key);
        if (entry == null) {
            throw new IllegalArgumentException("Key " + key + " not found");
        }
        if (entry.fixcount < 1) {
            throw new IllegalStateException("Entry with key " + key + " is not fixed");
        }
        entry.fixcount--;
        if (entry.fixcount == 0) {
            // Move entry from fixedList to the beginning of freeList
            boolean success = entry.node.remove();
            assert success;
            freeList.addFirst(entry.node);
        }
    }

    /**
     * Notified when an (unfixed) item is leaked from the map. The item
     * reference will stay valid throughout the itemLeaked() function call,
     * but the item must be persisted during that time.
     */
    public interface LeakEventListener<K, V> {

        /**
         * Notifies the listener that an item has been leaked from the map.
         * During this function call the item should be persisted. After this
         * function call, the item will be actually removed.
         */
        public void itemLeaked(K key, V item);

    }

    private class MapEntry {
        public V value;
        public LinkedListNode<K> node;
        public int fixcount;

        public MapEntry(V value, LinkedListNode<K> node) {
            this.value = value;
            this.node = node;
            this.fixcount = 0;
        }

        public void clear() {
            this.value = null;
            this.node = null;
            this.fixcount = 0;
        }

        public String toString() {
            return value + " [" + fixcount + "]";
        }

    }

    @Override
    public String toString() {
        StringBuilder b = new StringBuilder();
        b.append("[");
        boolean first = true;
        for (Map.Entry<K, MapEntry> e : map.entrySet()) {
            if (!first)
                b.append(", ");
            b.append(e.getKey()).append(":");
            b.append(e.getValue().value);
            b.append("[").append(e.getValue().fixcount).append("]");
            first = false;
        }

        b.append("]");
        return b.toString();
    }

}
