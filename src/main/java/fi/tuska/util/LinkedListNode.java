package fi.tuska.util;

import org.apache.log4j.Logger;

public class LinkedListNode<T> {

    private static final Logger log = Logger.getLogger(LinkedListNode.class);

    protected LinkedList<T> list;
    private final T value;
    protected LinkedListNode<T> next;
    protected LinkedListNode<T> previous;

    protected LinkedListNode(final T value) {
        this.list = null;
        this.value = value;
        next = previous = null;
    }

    protected void clear() {
        next = previous = null;
        list = null;
    }

    /**
     * Returns the value that is stored in this node.
     * 
     * @return the node's value
     */
    public T getValue() {
        return value;
    }

    public boolean isHead() {
        if (list == null)
            return false;
        return this == list.head;
    }

    public boolean isTail() {
        if (list == null)
            return false;
        return this == list.tail;
    }

    /**
     * Returns the next node in the list.
     * 
     * @return the next node
     */
    public LinkedListNode<T> getNext() {
        return next;
    }

    /**
     * Returns the previous node in the list.
     * 
     * @return the previous node
     */
    public LinkedListNode<T> getPrevious() {
        return previous;
    }

    /**
     * Checks whether the node is already in the list.
     * 
     * @return true if the node exists in the list.
     */
    public boolean isInList() {
        return list != null;
    }

    public boolean isInList(LinkedList<T> list) {
        return this.list == list;
    }

    /**
     * Adds the given node in the linked list before this node.
     * 
     * @param node the node to add
     * @return true if the node was added; false if the node already exists in
     * this list
     */
    protected boolean addBefore(final LinkedListNode<T> node) {
        if (node.isInList()) {
            // Already in the list
            return false;
        }
        assert this.isInList();

        node.list = this.list;
        if (isHead()) {
            // Update pointer to first node
            list.head = node;
            node.previous = null;
        } else {
            assert this.previous != null;
            // If there is a node before this node, update pointers to it
            node.previous = this.previous;
            this.previous.next = node;
        }
        // Update pointers from node to this
        node.next = this;
        this.previous = node;
        // Update list size
        list.size++;
        return true;
    }

    /**
     * Adds the given node in the linked list after this node.
     * 
     * @param node the node to add
     * @return true if the node was added; false if the node already exists in
     * this list
     */
    protected boolean addAfter(final LinkedListNode<T> node) {
        if (node.isInList()) {
            // Already in the list
            return false;
        }
        assert this.isInList();

        node.list = this.list;
        if (isTail()) {
            assert this.next == null;
            this.next = node;
            node.previous = this;
            node.next = null;
            // Update pointer to tail node
            list.tail = node;
        } else {
            // If there is a node after this node, update pointers to it
            LinkedListNode<T> nextNode = this.next;
            assert nextNode != null;
            node.next = nextNode;
            nextNode.previous = node;
            this.next = node;
            node.previous = this;
        }
        // Update list size
        list.size++;
        return true;
    }

    /**
     * Removes this node from the linked list.
     * 
     * @return true if the node actually was in the list; false if it was
     * already removed from the list
     */
    public boolean remove() {
        if (!isInList()) {
            // Already removed (not in the list)
            log.warn(this + " not in linked list, can't remove");
            return false;
        }
        if (isHead()) {
            // New head is the next node
            list.head = next;
        }
        if (isTail()) {
            // New tail is the previous node
            list.tail = previous;
        }
        if (previous != null) {
            // Update previous node to skip this node
            previous.next = next;
        }
        if (next != null) {
            // Update next node to skip this node
            next.previous = previous;
        }
        // Clear state info
        previous = null;
        next = null;
        // Update list size
        list.size--;
        // Remove item from list
        list = null;
        return true;
    }

    /**
     * Moves the node to the head of the linked list.
     * 
     * @return true if the operation was successful, false if the node is not
     * in the list
     */
    public boolean moveToHead() {
        if (!isInList()) {
            // Node is not in list
            return false;
        }
        if (isHead()) {
            // Node is already the head node
            return true;
        }
        LinkedList<T> theList = list;
        // Remove entry from list
        boolean result = remove();
        assert result : "Old entry removal failed";
        assert !isInList();

        assert theList != null;
        // Add entry to the head
        result = theList.head.addBefore(this);
        assert result : "Insert to head failed";

        // Check that the entry is in the list
        assert isInList();

        return true;
    }

    @Override
    public String toString() {
        return String.format("Linked list node [value: %s, in list: %s]", value,
            isInList() ? "yes" : "no");
    }

}
