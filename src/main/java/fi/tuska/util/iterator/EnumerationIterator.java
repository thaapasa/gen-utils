package fi.tuska.util.iterator;

import java.util.Enumeration;
import java.util.Iterator;

/**
 * Wrapper for making Iterators out of Enumerations.
 * 
 * @author Tuukka Haapasalo
 * 
 * @param <E>
 */
public class EnumerationIterator<E> implements Iterator<E> {

    private final Enumeration<E> enumeration;

    public EnumerationIterator(final Enumeration<E> enumeration) {
        this.enumeration = enumeration;
    }

    public boolean hasNext() {
        return enumeration.hasMoreElements();
    }

    public E next() {
        return enumeration.nextElement();
    }

    public void remove() {
        throw new UnsupportedOperationException("remove() not supported by java.util.Enumeration");
    }

}
