package fi.tuska.util.math;

import fi.tuska.util.math.MathUtils;
import junit.framework.TestCase;

/**
 * Tests for MathUtils
 * 
 * @author Tuukka Haapasalo
 */
public class MathUtilTest extends TestCase {

    /**
     * Tests expression evaluation and exception throwing.
     */
    public void testEvaluate() {
        assertEquals(8.0, MathUtils.evaluate("3+5"));
        assertEquals(8.0, MathUtils.evaluate("3 + 5"));
        assertEquals(15.0, MathUtils.evaluate("3 * 5"));
        assertEquals(14.0, MathUtils.evaluate("2 + 3 * 4"));
        assertEquals(20.0, MathUtils.evaluate("(2 + 3) * 4"));
        try {
            MathUtils.evaluate("3+");
            fail("MathUtils.evaluate did not throw exception");
        } catch (IllegalArgumentException e) {
            // OK, invalid expression
        }
    }

}
