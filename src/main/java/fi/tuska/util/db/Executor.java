package fi.tuska.util.db;

import java.io.File;
import java.io.PrintStream;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Properties;

import org.apache.log4j.Logger;

public class Executor {

    private static final Logger log = Logger.getLogger(Executor.class);

    private Connection dbConnection;

    private File dbDirectory;
    private String dbName;
    private String user;
    private String password;
    private boolean create;

    public Executor(Properties props) {
        this.dbDirectory = new File("db");
        this.dbName = "default";
        this.user = "dbuser";
        this.password = "1234";
        setProperties(props);
    }

    public Executor() {
        this(null);
    }

    public void setProperties(Properties props) {
        if (props.containsKey(Constants.DB_USER))
            user = props.getProperty(Constants.DB_USER);
        if (props.containsKey(Constants.DB_PASSWORD))
            password = props.getProperty(Constants.DB_PASSWORD);
        if (props.containsKey(Constants.DB_CREATE))
            create = props.getProperty(Constants.DB_CREATE).equalsIgnoreCase("true");
        if (props.containsKey(Constants.DB_DIRECTORY))
            dbDirectory = new File(props.getProperty(Constants.DB_DIRECTORY));
        if (props.containsKey(Constants.DB_DATABASE))
            dbName = props.getProperty(Constants.DB_DATABASE);
    }

    public void connect() {
        if (dbConnection == null) {
            Manager manager = new Manager(dbDirectory);
            dbConnection = manager.createConnection(dbName, user, password, create);
        }
    }

    public void close() {
        if (dbConnection != null) {
            try {
                dbConnection.close();
            } catch (SQLException e) {
                // Ignore
            }
            dbConnection = null;
        }
    }

    public static String trimSQL(String sql) {
        sql = sql.trim();
        if (sql.endsWith(";"))
            sql = sql.substring(0, sql.length() - 1);
        return sql;
    }

    public void execute(String sql, PrintStream target, PrintStream messageTarget) {
        sql = trimSQL(sql);
        log.debug("Executing SQL: " + sql);
        connect();
        try {
            Statement statement = dbConnection.createStatement();
            if (statement.execute(sql)) {
                // There are results
                ResultSet res = statement.getResultSet();
                int rows = showResults(res, target);
                res.close();
                if (messageTarget != null) {
                    target.flush();
                    messageTarget.println(String.format("Query returned %d line%s", rows,
                        rows != 1 ? "s" : ""));
                    messageTarget.flush();
                }
            }
            statement.close();
        } catch (SQLException ex) {
            log.error("Exception [" + ex.getMessage() + "] when executing " + sql, ex);
        }
    }

    private int showResults(ResultSet res, PrintStream target) throws SQLException {
        int cols = res.getMetaData().getColumnCount();
        int rows = 0;
        while (res.next()) {
            for (int i = 1; i <= cols; i++) {
                String r = res.getString(i);
                if (i != 1) {
                    target.print("\t");
                }
                target.print(r);
            }
            target.println();
            rows++;
        }
        return rows;
    }

}
