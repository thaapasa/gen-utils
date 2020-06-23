package fi.tuska.util.db;

import java.io.File;
import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.Properties;

import org.apache.log4j.Logger;

public class Manager {

    private static final String DERBY_DRIVER = "org.apache.derby.jdbc.EmbeddedDriver";
    private static final Logger log = Logger.getLogger(Manager.class);
    private final boolean initialized;

    public Manager(File directory) {
        log.info("Initializing DB manager");
        boolean dirOK = false;
        if (directory.isDirectory()) {
            try {
                directory = directory.getCanonicalFile();
                dirOK = true;
            } catch (IOException e) {
            }
        }
        if (dirOK) {
            // Set the db system directory.
            String dir = directory.toURI().getPath();
            log.info("Setting the DB path to " + dir);
            System.setProperty("derby.system.home", dir);
        } else {
            log.error("DB directory does not exist: " + directory);
        }

        boolean driverInitSuccess = initDriver();
        if (!driverInitSuccess) {
            log.error("JavaDB driver not found. Perhaps derby.jar is not in the class path? "
                + "Looking for: " + DERBY_DRIVER);
        }

        initialized = driverInitSuccess && dirOK;
        log.info(initialized ? "DB manager initialized successfully"
            : "DB manager initialization failed");
    }

    private boolean initDriver() {
        try {
            Class.forName(DERBY_DRIVER);
            return true;
        } catch (ClassNotFoundException e) {
            return false;
        }
    }

    public Connection createConnection(String database, Properties properties) {
        Connection dbConnection = null;
        String strUrl = "jdbc:derby:" + database;
        try {
            dbConnection = DriverManager.getConnection(strUrl, properties);
        } catch (SQLException sqle) {
            log.error("SQL exception: " + sqle, sqle);
        }
        return dbConnection;
    }

    public Connection createConnection(String database, String user, String password,
        boolean createIfNotExists) {
        if (!initialized) {
            log.warn("DB manager did not initialize correctly");
            return null;
        }
        Properties props = new Properties();
        props.put("user", user);
        props.put("password", password);
        props.put("create", createIfNotExists ? "true" : "false");
        return createConnection(database, props);
    }

}
