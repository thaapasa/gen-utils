package fi.tuska.util;

public class AssertionSupport {

    private AssertionSupport() {
        // Private constructor to prevent instantiation
    }

    /**
     * @return true if Java language assertions are enabled, false otherwise
     */
    public static boolean isAssertionsEnabled() {
        boolean assertionsEnabled = false;
        assert assertionsEnabled = true;
        return assertionsEnabled;
    }

}
