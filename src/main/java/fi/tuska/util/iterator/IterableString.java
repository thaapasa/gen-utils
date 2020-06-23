package fi.tuska.util.iterator;

import java.util.Iterator;

/**
 * Provides a Java 5 compatible character Iterator for standard Strings.
 * 
 * @author Tuukka Haapasalo
 */
public class IterableString implements Iterable<Character> {

    private String string;
    private int startPos;

    /**
     * Creates the iterator for the given string.
     * 
     * @param string the string to iterate over
     */
    public IterableString(final String string) {
        this.string = string;
        this.startPos = 0;
    }

    /**
     * Creates the iterator for the given string starting at given position.
     * 
     * @param string the string to iterate over
     * @param position the position to start iterating from
     */
    public IterableString(final String string, final int position) {
        this.string = string;
        this.startPos = position;
    }

    /**
     * Returns the iterator for the given string.
     */
    public Iterator<Character> iterator() {
        return new Iterator<Character>() {

            private int position = startPos;

            public boolean hasNext() {
                return position < string.length();
            }

            public Character next() {
                return string.charAt(position++);
            }

            public void remove() {
                throw new UnsupportedOperationException("Remove not supported by IterableString");
            }

        };
    }

}
