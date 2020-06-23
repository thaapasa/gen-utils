package fi.tuska.util;

import junit.framework.TestCase;

public class AssertionTest extends TestCase {

    public void testAssertions() {
        assertTrue(AssertionSupport.isAssertionsEnabled());
    }

}
