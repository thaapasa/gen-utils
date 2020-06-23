package fi.tuska.util;

/**
 * An interface that defines how to read in strings.
 * 
 * @author thaapasa
 * 
 * @param <T>
 */
public interface StringParser<T> {

    /**
     * Reads a value of this class from the given string.
     * 
     * @param value the string to parse
     * @return the parsed value
     */
    T parse(String value);

    String write(T object);

    /**
     * Checks whether the given string contains a valid representation of this
     * data type.
     * 
     * @param value the string to check
     * @return true if the string can be parsed.
     */
    boolean isValid(String value);

    /**
     * A dummy parser for reading strings.
     */
    static final StringParser<String> STRING_PARSER = new StringParser<String>() {

        @Override
        public boolean isValid(String value) {
            return value != null;
        }

        @Override
        public String parse(String value) {
            return value;
        }

        @Override
        public String write(String object) {
            return object;
        }
    };

    /**
     * A parser for reading long values.
     */
    static final StringParser<Long> LONG_PARSER = new StringParser<Long>() {

        @Override
        public Long parse(String value) {
            if (value == null)
                return null;
            return new Long(value.trim());
        }

        @Override
        public boolean isValid(String value) {
            try {
                Long v = parse(value);
                return v != null;
            } catch (NumberFormatException e) {
                return false;
            }
        }

        @Override
        public String write(Long object) {
            return String.valueOf(object.longValue());
        }

    };

    /**
     * A parser for reading integer values.
     */
    static final StringParser<Integer> INTEGER_PARSER = new StringParser<Integer>() {

        @Override
        public Integer parse(String value) {
            if (value == null)
                return null;
            return new Integer(value.trim());
        }

        @Override
        public boolean isValid(String value) {
            if (value == null) {
                return false;
            }
            try {
                Integer v = parse(value);
                return v != null;
            } catch (NumberFormatException e) {
                return false;
            }
        }

        @Override
        public String write(Integer object) {
            return String.valueOf(object.intValue());
        }

    };

}
