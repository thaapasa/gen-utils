package fi.tuska.util.iterator;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Enumeration;
import java.util.Iterator;
import java.util.List;

import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

/**
 * This class provides static methods for quickly fetching Iterable wrappers
 * for various standard types.
 * 
 * @author Tuukka Haapasalo
 */
public class Iterables {

    /**
     * Private constructor to prevent accidental instantiation of this class.
     */
    private Iterables() {
    }

    /**
     * @return an Iterable wrapper for an Iterator.
     */
    public static <T> Iterable<T> get(final Iterator<T> iterator) {
        return new IterableIterator<T>(iterator);
    }

    /**
     * @return an Iterable wrapper for an Enumeration.
     */
    public static <T> Iterable<T> get(final Enumeration<T> enumeration) {
        return new IterableEnumeration<T>(enumeration);
    }

    /**
     * @return an Iterable&lt;Character&gt; wrapper for an String.
     */
    public static Iterable<Character> get(final String string) {
        return new IterableString(string);
    }

    /**
     * @return an Iterable&lt;Node&gt; wrapper for a NodeList.
     */
    public static Iterable<Node> get(final NodeList nodeList) {
        return new IterableNodeList(nodeList);
    }

    /**
     * @return a reverse Iterable for the given list
     */
    public static <T> Iterable<T> reverse(final List<T> list) {
        return ReverseIterator.iterable(list);
    }

    /**
     * @return an iterator that iterates through the lines of the file
     */
    public static Iterable<String> get(File file) {
        try {
            return new FileIterator(file);
        } catch (FileNotFoundException e) {
            return new EmptyIterator<String>();
        }
    }
}
