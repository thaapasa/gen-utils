package fi.tuska.util.iterator;

import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * An abstract class that supports the creation of iterators.
 * 
 * @author thaapasa
 */
public abstract class AbstractIterator<T> implements Iterator<T> {

    private T nextV;
    private boolean removed;

    protected abstract T findNext();

    protected abstract void doRemove();

    @Override
    public boolean hasNext() {
        if (nextV != null)
            return true;
        nextV = findNext();
        return nextV != null;
    }

    @Override
    public T next() {
        if (nextV == null)
            nextV = findNext();
        if (nextV == null)
            throw new NoSuchElementException();
        T r = nextV;
        removed = false;
        nextV = null;
        return r;
    }

    @Override
    public void remove() {
        if (removed)
            throw new IllegalStateException("Already removed");
        if (nextV != null)
            throw new IllegalStateException("hasNext() has been already called");
        doRemove();
        removed = true;
    }
}
