package fi.tuska.util.iterator;

import java.util.Iterator;
import java.util.NoSuchElementException;

public class EmptyIterator<K> implements Iterator<K>, Iterable<K> {

    public EmptyIterator() {
    }

    @Override
    public boolean hasNext() {
        return false;
    }

    @Override
    public K next() {
        throw new NoSuchElementException("No entries in this iterator");
    }

    @Override
    public void remove() {
        throw new UnsupportedOperationException("remove() not supported");
    }

    @Override
    public Iterator<K> iterator() {
        return this;
    }

}
