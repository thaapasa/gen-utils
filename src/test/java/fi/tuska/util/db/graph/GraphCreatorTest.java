package fi.tuska.util.db.graph;

import java.io.File;

import fi.tuska.util.db.DB;
import fi.tuska.util.db.Executor;
import junit.framework.TestCase;

public class GraphCreatorTest extends TestCase {

    public void testGraphCreation() {
        File dir = new File("db");
        if (!dir.exists())
            dir.mkdir();

        Executor executor = DB.getDefaultExecutor(new String[] { "--create=true" });

        executor.execute("drop table graphs", System.out, System.err);
        executor.execute("create table graphs (id INTEGER NOT NULL "
            + "PRIMARY KEY GENERATED ALWAYS AS IDENTITY " + "(START WITH 1, INCREMENT BY 1), "
            + "xval INTEGER, yval INTEGER, type INTEGER, run INTEGER)", System.out, System.err);

        executor.execute("insert into graphs (xval, yval, type, run) values (1, 1, 1, 20)",
            System.out, System.err);
        executor.execute("insert into graphs (xval, yval, type, run) values (2, 10, 1, 20)",
            System.out, System.err);
        executor.execute("insert into graphs (xval, yval, type, run) values (3, 15, 1, 20)",
            System.out, System.err);

        executor.execute("insert into graphs (xval, yval, type, run) values (1, 10, 2, 20)",
            System.out, System.err);
        executor.execute("insert into graphs (xval, yval, type, run) values (2, 7, 2, 20)",
            System.out, System.err);
        executor.execute("insert into graphs (xval, yval, type, run) values (3, 6, 2, 20)",
            System.out, System.err);
    }

}
