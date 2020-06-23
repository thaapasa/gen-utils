package fi.tuska.util.iterator;

import java.util.Iterator;

import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

/**
 * This class provides a convenience wrapper around the W3C DOM NodeList.
 * 
 * @author Tuukka Haapasalo
 */
public class IterableNodeList implements Iterable<Node> {

    private NodeList nodeList;

    /**
     * Constructs the NodeListIterable.
     */
    public IterableNodeList(NodeList list) {
        this.nodeList = list;
    }

    /**
     * Returns the iterator that iterates over the NodeList.
     */
    public Iterator<Node> iterator() {
        return new Iterator<Node>() {
            private int pos = 0;

            public boolean hasNext() {
                return pos < nodeList.getLength();
            }

            public Node next() {
                return nodeList.item(pos++);
            }

            public void remove() {
                throw new UnsupportedOperationException(
                    "NodeListIterable does not support element deletion");
            }

        };
    }

}
