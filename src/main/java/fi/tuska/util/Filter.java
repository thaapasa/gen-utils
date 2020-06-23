package fi.tuska.util;

/**
 * A generic filter interface for testing if certain objects are valid.
 * 
 * @author tuska
 */
public interface Filter<T> {

    boolean isValid(T object);

}
