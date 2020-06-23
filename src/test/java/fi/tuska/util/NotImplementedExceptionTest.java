package fi.tuska.util;

import junit.framework.TestCase;

public class NotImplementedExceptionTest extends TestCase {

    public void testThrowException() {
        try {
            throw new NotImplementedException();
        } catch (NotImplementedException e) {
            assertEquals("testThrowException", e.getSrcMethod());
            assertEquals("fi.tuska.util.NotImplementedExceptionTest", e.getSrcClass());
            System.err.println(e);
        }
    }

}
