package fi.tuska.util.iterator;

import java.util.Enumeration;


/**
 * Wrapper for making the Enumeration class Iterable.
 * 
 * @author Tuukka Haapasalo
 * 
 * @param <E>
 */
public class IterableEnumeration<E> extends IterableIterator<E> {

    public IterableEnumeration(final Enumeration<E> enumeration) {
        super(new EnumerationIterator<E>(enumeration));
    }

}
