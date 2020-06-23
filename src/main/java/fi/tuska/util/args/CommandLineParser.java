package fi.tuska.util.args;

import java.util.Properties;

/**
 * Simple command line parser.
 * 
 * @author thaapasa
 */
public class CommandLineParser {

    public static final String DEFAULT_PROP_NAME = "__def";

    /**
     * Reads the command-line arguments into a Properties object.
     * 
     * @param args
     * @return
     */
    public static void readProperties(String[] args, Properties props) {
        if (args == null)
            return;

        String currentKey = null;

        for (String arg : args) {
            if (arg.startsWith("-")) {
                if (currentKey != null) {
                    props.put(currentKey, null);
                }

                if (arg.startsWith("--")) {
                    currentKey = arg.substring(2);
                } else {
                    currentKey = arg.substring(1);
                }
                int eqIndex = currentKey.indexOf("=");
                if (eqIndex >= 0) {
                    arg = currentKey.substring(eqIndex + 1);
                    currentKey = currentKey.substring(0, eqIndex);
                    props.put(currentKey, arg);
                    currentKey = null;
                    continue;
                }
            } else {
                if (currentKey != null) {
                    props.put(currentKey, arg);
                } else {
                    // No key defined
                    props.put(DEFAULT_PROP_NAME, arg);
                }
                currentKey = null;
            }
        }
    }

    public static String getDefaultArgument(Properties props) {
        return props.getProperty(DEFAULT_PROP_NAME);
    }

}
