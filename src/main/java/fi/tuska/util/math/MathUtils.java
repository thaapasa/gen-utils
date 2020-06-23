package fi.tuska.util.math;

import org.nfunk.jep.JEP;

/**
 * Collection of mathematical general purpose utilities.
 * 
 * @author Tuukka Haapasalo
 */
public class MathUtils {

    /** The JEP mathematical expression parser. */
    private static JEP jepParser = new JEP();

    private MathUtils() {
        // Private constructor to prevent instantiation
    }

    /**
     * Evaluates a mathematical expression (that does not contain variables).
     * 
     * @param expression the expression to evaluate
     * @return the value of the expression
     */
    public static double evaluate(final String expression) {
        jepParser.parseExpression(expression);
        if (jepParser.hasError()) {
            throw new IllegalArgumentException("The expression is not valid: "
                + jepParser.getErrorInfo());
        }
        return jepParser.getValue();
    }

}
