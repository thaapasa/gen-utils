package fi.tuska.util;

import java.util.Iterator;

public class IteratorWrapper<S, T> implements Iterator<T>, Iterable<T> {

    private final Converter<S, T> converter;
    private final Iterator<S> srcIterator;

    public IteratorWrapper(Iterator<S> srcIterator, Converter<S, T> converter) {
        this.srcIterator = srcIterator;
        this.converter = converter;
    }

    public IteratorWrapper(Iterable<S> src, Converter<S, T> converter) {
        this.srcIterator = src.iterator();
        this.converter = converter;
    }

    @Override
    public Iterator<T> iterator() {
        return this;
    }

    @Override
    public boolean hasNext() {
        return srcIterator.hasNext();
    }

    @Override
    public T next() {
        S original = srcIterator.next();
        T converted = converter.convert(original);
        return converted;
    }

    @Override
    public void remove() {
        srcIterator.remove();
    }

}
