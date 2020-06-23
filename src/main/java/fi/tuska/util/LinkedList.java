package fi.tuska.util;

import java.util.Collection;
import java.util.Deque;
import java.util.Iterator;
import java.util.List;
import java.util.ListIterator;
import java.util.NoSuchElementException;

/**
 * A simple linked list that allows outsiders to access the list items
 * themselves (to make references to them, for example).
 * 
 * @author thaapasa
 */
public class LinkedList<T> implements List<T>, Deque<T> {

    protected LinkedListNode<T> head;
    protected LinkedListNode<T> tail;
    protected long size;

    /**
     * Constructs an empty linked list.
     */
    public LinkedList() {
        head = tail = null;
        size = 0;
    }

    /**
     * Clears the list.
     */
    public void clear() {
        head = null;
        tail = null;
        size = 0;

        // This should be done before setting head to null...
        // No matter, hopefully GC will clear the list

        // LinkedListNode<T> node = head;
        // while (node != null) {
        // LinkedListNode<T> next = node.next;
        // node.clear();
        // node = next;
        // }
    }

    /**
     * Adds an object as the first entry of the list.
     * 
     * @param value the object to add
     */
    @Override
    public void addFirst(T value) {
        final LinkedListNode<T> node = new LinkedListNode<T>(value);
        addFirst(node);
    }

    /**
     * Adds an object as the first entry of the list.
     * 
     * @param value the object to add
     * @return the node that was created
     */
    public LinkedListNode<T> addFirstNode(T value) {
        final LinkedListNode<T> node = new LinkedListNode<T>(value);
        addFirst(node);
        return node;
    }

    /** Makes the given node a head of this list. */
    public void addFirst(LinkedListNode<T> node) {
        if (head == null) {
            assert size == 0;
            // This is the first node in the list, so make head & tail point
            // to it
            node.list = this;
            head = node;
            tail = node;
            size++;
        } else {
            // Add the new node before the first node
            boolean result = head.addBefore(node);
            assert result : "head.addBefore() returned false";
        }
        assert node.isInList();
    }

    /**
     * Adds an object as the last entry of the list.
     * 
     * @param value the object to add
     */
    public void addLast(T value) {
        final LinkedListNode<T> node = new LinkedListNode<T>(value);
        addLast(node);
    }

    /**
     * Adds an object as the last entry of the list.
     * 
     * @param value the object to add
     * @return the node that was created
     */
    public LinkedListNode<T> addLastNode(T value) {
        final LinkedListNode<T> node = new LinkedListNode<T>(value);
        addLast(node);
        return node;
    }

    /**
     * Makes the given node the tail of this list.
     */
    public void addLast(LinkedListNode<T> node) {
        if (tail == null) {
            assert size == 0;
            // This is the first node in the list, so make head & tail point
            // to it
            node.list = this;
            head = node;
            tail = node;
            size++;
        } else {
            // Add the new node after the last node
            boolean result = tail.addAfter(node);
            assert result : "tail.addAfter() returned false";
        }
        assert node.isInList();
    }

    /**
     * Removes the last node of the list and returns it.
     * 
     * @return the removed node
     */
    public LinkedListNode<T> removeLastNode() {
        if (tail == null) {
            throw new NoSuchElementException("The list is empty");
        }
        final LinkedListNode<T> removed = tail;
        // remove() should always return true
        boolean result = removed.remove();
        assert result : "tail.remove() failed";

        return removed;
    }

    /**
     * Removes the last node of the list and returns the item.
     * 
     * @return the removed item
     */
    @Override
    public T removeLast() {
        if (tail == null) {
            throw new NoSuchElementException("The list is empty");
        }
        final LinkedListNode<T> removed = tail;
        // remove() should always return true
        boolean result = removed.remove();
        assert result : "tail.remove() failed";

        return removed.getValue();
    }

    /**
     * Removes the first node of the list and returns the item.
     * 
     * @return the removed item
     */
    @Override
    public T removeFirst() {
        if (head == null) {
            throw new NoSuchElementException("The list is empty");
        }
        final LinkedListNode<T> removed = head;
        // remove() should always return true
        if (!removed.remove()) {
            throw new RuntimeException(
                "Bug in LinkedList.removeFirst(): tail.remove() returned false");
        }
        return removed.getValue();
    }

    /**
     * Removes the first node of the list and returns it.
     * 
     * @return the removed node
     */
    public LinkedListNode<T> removeFirstNode() {
        if (head == null) {
            throw new NoSuchElementException("The list is empty");
        }
        final LinkedListNode<T> removed = head;
        // remove() should always return true
        if (!removed.remove()) {
            throw new RuntimeException(
                "Bug in LinkedList.removeFirst(): tail.remove() returned false");
        }
        return removed;
    }

    /**
     * Returns the size of the list.
     * 
     * @return the amount of nodes in the list
     */
    public long getSize() {
        return size;
    }

    /**
     * Returns the first item in the list, or null, if the list is empty.
     * 
     * @return the first item in the list
     */
    @Override
    public T getFirst() {
        if (head == null)
            throw new NoSuchElementException("The list is empty");
        return head.getValue();
    }

    /**
     * Returns the first node in the list, or null, if the list is empty.
     * 
     * @return the first node in the list
     */
    public LinkedListNode<T> getFirstNode() {
        return head;
    }

    /**
     * Returns the last item in the list, or null, if the list is empty.
     * 
     * @return the last item in the list
     */
    @Override
    public T getLast() {
        if (head == null)
            throw new NoSuchElementException("The list is empty");
        return tail.getValue();
    }

    /**
     * Returns the last node in the list, or null, if the list is empty.
     * 
     * @return the last node in the list
     */
    public LinkedListNode<T> getLastNode() {
        return tail;
    }

    @Override
    public Iterator<T> iterator() {
        return new Iterator<T>() {

            private LinkedListNode<T> curNode = null;
            private LinkedListNode<T> toRemove = null;

            {
                curNode = head;
            }

            private void seekNext() {
                if (curNode == null)
                    return;
                curNode = curNode.next;
            }

            @Override
            public boolean hasNext() {
                return curNode != null;
            }

            @Override
            public T next() {
                toRemove = curNode;
                seekNext();
                return toRemove != null ? toRemove.getValue() : null;
            }

            @Override
            public void remove() {
                if (toRemove == null)
                    throw new IllegalStateException();
                toRemove.remove();
                toRemove = null;
            }
        };
    }

    @Override
    public int size() {
        return (int) size;
    }

    @Override
    public boolean isEmpty() {
        return size == 0;
    }

    @Override
    public boolean contains(Object o) {
        LinkedListNode<T> n = head;
        while (n != null) {
            if (o == null) {
                if (n.getValue() == null)
                    return true;
            } else {
                if (n.getValue().equals(o))
                    return true;
            }
            n = n.next;
        }
        return false;
    }

    @Override
    public Object[] toArray() {
        throw new NotImplementedException();
    }

    @Override
    public <E> E[] toArray(E[] a) {
        throw new NotImplementedException();
    }

    @Override
    public boolean add(T e) {
        addLast(e);
        return true;
    }

    @Override
    public boolean remove(Object o) {
        throw new NotImplementedException();
    }

    @Override
    public boolean containsAll(Collection<?> c) {
        throw new NotImplementedException();
    }

    @Override
    public boolean addAll(Collection<? extends T> c) {
        throw new NotImplementedException();
    }

    @Override
    public boolean addAll(int index, Collection<? extends T> c) {
        throw new NotImplementedException();
    }

    @Override
    public boolean removeAll(Collection<?> c) {
        throw new NotImplementedException();
    }

    @Override
    public boolean retainAll(Collection<?> c) {
        throw new NotImplementedException();
    }

    @Override
    public T get(int index) {
        throw new NotImplementedException();
    }

    @Override
    public T set(int index, T element) {
        throw new NotImplementedException();
    }

    @Override
    public void add(int index, T element) {
        throw new NotImplementedException();
    }

    @Override
    public T remove(int index) {
        if (index < 0)
            throw new IndexOutOfBoundsException("List index < 0");
        if (index >= size)
            throw new IndexOutOfBoundsException("List index > list size");
        int i = 0;
        LinkedListNode<T> cur = head;
        while (i++ < index) {
            cur = cur.next;
        }
        T val = cur.getValue();
        cur.remove();
        return val;
    }

    @Override
    public int indexOf(Object o) {
        throw new NotImplementedException();
    }

    @Override
    public int lastIndexOf(Object o) {
        throw new NotImplementedException();
    }

    @Override
    public ListIterator<T> listIterator() {
        throw new NotImplementedException();
    }

    @Override
    public ListIterator<T> listIterator(int index) {
        throw new NotImplementedException();
    }

    @Override
    public List<T> subList(int fromIndex, int toIndex) {
        throw new NotImplementedException();
    }

    protected void checkConsistency() {
        if (head == null || tail == null) {
            assert head == null;
            assert tail == null;
            assert size == 0;
            return;
        }
        if (size == 1)
            assert head == tail;
        assert head.previous == null;
        assert tail.next == null;
        LinkedListNode<T> cur = head;
        int checkSize = 0;
        while (cur != null) {
            checkSize++;
            LinkedListNode<T> next = cur.next;
            if (next != null) {
                assert next.previous == cur;
            }
            cur = next;
        }
        assert size == checkSize;
    }

    @Override
    public boolean offerFirst(T e) {
        throw new NotImplementedException();
    }

    @Override
    public boolean offerLast(T e) {
        throw new NotImplementedException();
    }

    @Override
    public T pollFirst() {
        if (head == null)
            return null;
        T item = head.getValue();
        head.remove();
        return item;
    }

    @Override
    public T pollLast() {
        if (tail == null)
            return null;
        T item = tail.getValue();
        tail.remove();
        return item;
    }

    @Override
    public T peekFirst() {
        return head != null ? head.getValue() : null;
    }

    @Override
    public T peekLast() {
        return tail != null ? tail.getValue() : null;
    }

    @Override
    public boolean removeFirstOccurrence(Object o) {
        throw new NotImplementedException();
    }

    @Override
    public boolean removeLastOccurrence(Object o) {
        throw new NotImplementedException();
    }

    @Override
    public boolean offer(T e) {
        throw new NotImplementedException();
    }

    @Override
    public T remove() {
        return removeFirst();
    }

    @Override
    public T poll() {
        return pollFirst();
    }

    @Override
    public T element() {
        return getFirst();
    }

    @Override
    public T peek() {
        return peekFirst();
    }

    @Override
    public void push(T e) {
        addFirst(e);
    }

    @Override
    public T pop() {
        return removeFirst();
    }

    @Override
    public Iterator<T> descendingIterator() {
        throw new NotImplementedException();
    }

}
