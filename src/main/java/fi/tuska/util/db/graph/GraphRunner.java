package fi.tuska.util.db.graph;

import org.apache.log4j.Logger;
import org.springframework.context.support.ClassPathXmlApplicationContext;

public class GraphRunner {

    private static final Logger log = Logger.getLogger(GraphRunner.class);

    public static final String[] CONFIG_PATHS = new String[] { "classpath:graphs.xml" };

    public static void main(String[] args) {
        log.info("Initializing application context");
        new ClassPathXmlApplicationContext(CONFIG_PATHS);
    }

}
