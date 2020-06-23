package fi.tuska.util.db;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintStream;
import java.util.Properties;

import org.apache.log4j.Logger;

import fi.tuska.util.CollectionUtils;
import fi.tuska.util.args.CommandLineParser;

/**
 * Embedded JavaDB executor. This runnable class can be used to work with a
 * JavaDB database.
 * 
 * @author thaapasa
 */
public class DB {

    private static final Logger log = Logger.getLogger(DB.class);

    public static final Properties getProperties() {
        return getProperties(null);
    }

    /**
     * Reads DB properties from the properties file (in classpath); and reads
     * arguments from command line. The arguments given in command line
     * overwrite default values in the properties file.
     * 
     * @param args the command-line arguments
     * @return the DB properties read from file and cmd line
     */
    public static final Properties getProperties(String[] args) {
        Properties props = new Properties();

        InputStream propStream = DB.class.getResourceAsStream(Constants.PROPERTY_FILE);
        if (propStream != null) {
            try {
                props.load(propStream);
            } catch (IOException e) {
                log.warn("Could not load DB properties: " + e);
            }
        }

        CommandLineParser.readProperties(args, props);
        return props;
    }

    /**
     * Reads input (SQL clauses) from the given input stream, and executes the
     * SQL clauses with the given executor. Used for reading SQL input from
     * System.in, for example.
     * 
     * @param stream the input stream (for example, System.in)
     * @param executor the SQL executor object
     */
    public static void executeSQL(Executor executor, InputStream stream, PrintStream target,
        PrintStream messageTarget) {
        BufferedReader rd = new BufferedReader(new InputStreamReader(System.in));
        String line = null;
        StringBuilder curSQL = new StringBuilder();
        try {
            while ((line = rd.readLine()) != null) {
                line = line.trim();

                if (line.equalsIgnoreCase("exit") || line.equalsIgnoreCase("quit")
                    || line.equalsIgnoreCase("q")) {
                    target.flush();
                    messageTarget.println("Bye!");
                    break;
                }

                if (curSQL.length() > 0) {
                    curSQL.append(" ");
                }
                if (line.startsWith("--")) {
                    // Comment line, skip it
                    continue;
                }
                curSQL.append(line);
                if (line.endsWith(";")) {
                    executor.execute(curSQL.toString(), target, messageTarget);
                    curSQL.setLength(0);
                }
            }
            if (curSQL.length() > 0) {
                executor.execute(curSQL.toString(), target, messageTarget);
            }
        } catch (IOException e) {
            // Not likely
            log.error("IO exception: " + e, e);
        }
    }

    public static Executor getDefaultExecutor(String[] args) {
        // Read properties (database directory, credentials, database name,
        // etc.)
        Properties props = getProperties(args);

        // Create executor object
        Executor executor = new Executor(props);
        return executor;
    }

    public static void main(String[] args) {
        Executor executor = getDefaultExecutor(args);
        executor.connect();

        PrintStream msgTarget = System.err;

        if (CollectionUtils.contains(args, "-s")) {
            msgTarget = nullStream;
        }

        // Read SQL input from System.in, execute on the database
        executeSQL(executor, System.in, System.out, msgTarget);
        executor.close();
    }

    /**
     * A stream that does not print anywhere.
     */
    private static final PrintStream nullStream = new PrintStream(new OutputStream() {
        @Override
        public void write(int b) throws IOException {
        }
    });

}
