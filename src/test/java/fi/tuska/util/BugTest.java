package fi.tuska.util;

import junit.framework.TestCase;

public class BugTest extends TestCase {

    public void doFail() {
        throw new Bug("General Epic Failure");
    }

    public void testThrow() {
        try {
            doFail();
            fail("Bug not thrown");
        } catch (RuntimeException b) {
            System.out.println(b);
        }
    }

}
