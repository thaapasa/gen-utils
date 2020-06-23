package fi.tuska.util;

/**
 * A simple interface that can be used to define callback parameters to
 * methods, and so on.
 * 
 * @author thaapasa
 * 
 * @param <T> the parameter type of the callback method
 */
public interface Callback<T> {

    /**
     * The callback method.
     * 
     * @param object the parameter
     * @return for example: return true to contine, false to stop processing.
     */
    boolean callback(T objects);

}
