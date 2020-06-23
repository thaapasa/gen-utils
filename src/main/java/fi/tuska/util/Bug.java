package fi.tuska.util;

public class Bug extends RuntimeException {

    private static final long serialVersionUID = -5577769853295172697L;

    private String srcClass;
    private String srcMethod;

    public Bug() {
        this("not specified");
    }

    public Bug(String message) {
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
        return "Bug: " + super.getMessage() + " in " + getStackTrace()[0];
    }

}
