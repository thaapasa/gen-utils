package fi.tuska.util.iterator;

import java.util.Iterator;
import java.util.List;
import java.util.NoSuchElementException;

public class ReverseIterator<V> implements Iterator<V> {

    private List<V> list;
    // A natural position of the next element in list. At first, it is
    // list.size(). At the end, it is 0. If the list is invalid or empty, it
    // is -1. If the last element is removed with remove(), it will be -1
    // after that operation.
    private int pos;
    private boolean canRemove;

    public ReverseIterator(List<V> list) {
        this.list = list;
        this.pos = list != null ? list.size() : -1;
        this.canRemove = false;
    }

    public static <T> Iterable<T> iterable(final List<T> list) {
        return new Iterable<T>() {
            @Override
            public Iterator<T> iterator() {
                return new ReverseIterator<T>(list);
            }
        };
    }

    @Override
    public boolean hasNext() {
        // At zero we have already went through the list. At -1 the list was
        // empty or the last element was removed with remove().
        return pos > 0;
    }

    @Override
    public V next() {
        if (!hasNext())
            throw new NoSuchElementException("List contains no more elements");
        // Pos is the natural index of the next element, so it is one more
        // than the zero-based index
        V value = list.get(pos - 1);
        pos--;
        canRemove = true;
        return value;
    }

    @Override
    public void remove() {
        if (pos < 0)
            throw new IllegalStateException("List was empty");
        if (!canRemove)
            throw new IllegalStateException("Element already deleted");
        list.remove(pos);
        canRemove = false;
    }

}
