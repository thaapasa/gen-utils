package fi.tuska.util.db;

public interface Constants {

    /** Name of the property file. */
    public static final String PROPERTY_FILE = "/db.properties";

    /**
     * Parameter names used for properties. You can overwrite the values in
     * the properties file by giving command-line arguments (for example
     * "java DB --user=me --db=mydb").
     */
    public static final String DB_USER = "user";
    public static final String DB_PASSWORD = "pass";
    public static final String DB_DIRECTORY = "dir";
    public static final String DB_DATABASE = "db";
    public static final String DB_CREATE = "create";

}
