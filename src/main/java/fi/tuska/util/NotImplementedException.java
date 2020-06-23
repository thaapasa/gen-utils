package fi.tuska.util;

/**
 * You can throw this runtime exception to indicate that something is not yet
 * implemented. For convenience, this exception automatically tracks the
 * source method and class, and prints them in the exception message.
 * 
 * @author thaapasa
 */
public class NotImplementedException extends RuntimeException {

    private static final long serialVersionUID = -1949451200539795389L;

    private String srcClass;
    private String srcMethod;

    public NotImplementedException() {
        this("Not implemented");
    }

    public NotImplementedException(String message) {
        super(message);
        StackTraceElement el = getStackTrace()[0];
        this.srcClass = el.getClassName();
        this.srcMethod = el.getMethodName();
    }

    public String getSrcClass() {
        return srcClass;
    }

    public String getSrcMethod() {
        return srcMethod;
    }

    @Override
    public String getMessage() {
        return super.getMessage() + ": " + getStackTrace()[0];
    }

}
