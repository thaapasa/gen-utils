package fi.tuska.util.iterator;

import java.util.Iterator;

import fi.tuska.util.Filter;

/**
 * A wrapper class for providing a filter for an iterator. Only items that
 * pass the filter are returned.
 * 
 * @author tuska
 */
public class FilteringIterator<T> implements Iterator<T>, Iterable<T> {

    private final Iterator<T> iterator;
    private final Filter<T> filter;
    private T next;
    private boolean available;

    public FilteringIterator(Iterator<T> iterator, Filter<T> filter) {
        this.iterator = iterator;
        this.filter = filter;
        this.next = null;
    }

    @Override
    public boolean hasNext() {
        if (available) {
            return true;
        }
        // Seek next
        while (!available) {
            if (!iterator.hasNext()) {
                return false;
            }
            next = iterator.next();
            if (filter.isValid(next)) {
                available = true;
            }
        }
        return available;
    }

    @Override
    public T next() {
        if (available) {
            T result = next;
            available = false;
            return result;
        }
        throw new IllegalStateException("hasNext() not called before next()");
    }

    @Override
    public void remove() {
        iterator.remove();
    }

    @Override
    public Iterator<T> iterator() {
        return this;
    }

}
