package fi.tuska.util.iterator;

import java.util.Iterator;

/**
 * Wrapper for making the Iterator class Iterable.
 * 
 * @author Tuukka Haapasalo
 * 
 * @param <E>
 */
public class IterableIterator<E> implements Iterable<E> {

    private final Iterator<E> iterator;

    public IterableIterator(final Iterator<E> iterator) {
        this.iterator = iterator;
    }

    public Iterator<E> iterator() {
        return iterator;
    }

}
