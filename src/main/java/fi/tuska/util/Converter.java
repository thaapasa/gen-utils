package fi.tuska.util;

/**
 * Simple interface that defines a converter object that can convert objects
 * of a given source type into the target type.
 * 
 * @author thaapasa
 * 
 * @param <S> the source type
 * @param <T> the target type
 */
public interface Converter<S, T> {

    /**
     * Converts the given source object into the target type.
     * 
     * @param source the source object
     * @return the converted object
     */
    T convert(S source);

}
