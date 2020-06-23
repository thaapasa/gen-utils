package fi.tuska.util;

public class TypeConvertingCallback<S, T> implements Callback<S> {

    private final Converter<S, T> converter;
    private final Callback<T> callback;

    public TypeConvertingCallback(Converter<S, T> converter, Callback<T> callback) {
        this.converter = converter;
        this.callback = callback;
    }

    @Override
    public boolean callback(S object) {
        return callback.callback(converter.convert(object));
    }

}
