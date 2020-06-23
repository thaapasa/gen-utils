package fi.tuska.util.db.graph;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintStream;
import java.util.List;
import java.util.Properties;

import org.apache.log4j.Logger;

import fi.tuska.util.db.DB;
import fi.tuska.util.db.Executor;

public class GraphCreator {

    private static final Logger log = Logger.getLogger(GraphCreator.class);

    private List<GraphDefinition> graphs;
    private Executor executor;

    private File directory;
    private String term;
    private String gnuplot;
    private String gnuplotDefinitions;
    private String epstopdf;

    public GraphCreator() {
        this.executor = initDBExecutor();
        this.directory = new File(".");
        this.term = "postscript enhanced color eps";
    }

    /**
     * Set the directory where the graph data is stored and where the
     * generated images are placed.
     */
    public void setDirectory(File directory) {
        if (!directory.isDirectory()) {
            log.warn("Not a directory: " + directory);
        }
        this.directory = directory;
    }

    public void setGnuplotDefinitions(String gnuplotDefinitions) {
        this.gnuplotDefinitions = gnuplotDefinitions;
    }

    public void setTerm(String term) {
        this.term = term;
    }

    public String getTerm() {
        return term;
    }

    /**
     * Set the graphs to create
     */
    public void setGraphs(List<GraphDefinition> graphs) {
        this.graphs = graphs;
    }

    /**
     * Set the GNUPlot executable
     */
    public void setGnuplot(String gnuplot) {
        this.gnuplot = gnuplot;
    }

    /**
     * Set the epstopdf executable
     */
    public void setEpstopdf(String epstopdf) {
        this.epstopdf = epstopdf;
    }

    /**
     * Call to create the defined graphs
     */
    public void createGraphs() {
        for (GraphDefinition def : graphs) {
            try {
                createGraph(def);
            } catch (IOException e) {
                log.error(e, e);
            }
        }
    }

    private void createGraph(GraphDefinition def) throws IOException {
        log.info("Processing graph " + def.getName() + ": " + def.getGraphTitle());
        // Load data from DB to data file
        for (LineDefinition line : def.getLines()) {
            String sql = line.getDataFetchSQL();
            if (sql != null) {
                log.debug("Loading SQL data for " + def.getName() + " into data file");
                loadDataFromDB(line, def);
            }

        }

        // Create GNUPlot data file
        createPlotFile(def);

        // Run GNUPlot
        if (gnuplot != null) {
            log.debug("Running GNUPlot for " + def.getName());
            runGNUPlot(def);
        }

        // Convert to PDF
        if (epstopdf != null) {
            log.debug("Converting EPS to PDF for " + def.getName());
            runEPStoPDF(def);
        }
    }

    private boolean runEPStoPDF(GraphDefinition def) {
        try {
            Process process = Runtime.getRuntime().exec(
                new String[] { epstopdf, def.getImageFile(directory).getPath() });
            return process.waitFor() == 0;
        } catch (Exception e) {
            log.warn(e);
            return false;
        }
    }

    private boolean runGNUPlot(GraphDefinition def) {
        try {
            Process process = Runtime.getRuntime().exec(
                new String[] { gnuplot, def.getPlotFile(directory).getPath() });
            return process.waitFor() == 0;
        } catch (Exception e) {
            log.warn(e);
            return false;
        }
    }

    public static boolean isEmpty(String src) {
        return src == null || src.trim().equals("");
    }

    private void createPlotFile(GraphDefinition def) throws IOException {
        File datafile = def.getPlotFile(directory);
        FileOutputStream fos = new FileOutputStream(datafile);
        PrintStream ps = new PrintStream(fos);

        ps.println("set term " + term);
        ps.println("set output '" + def.getImageFile(directory).getPath() + "'");
        ps.println();

        ps.println(gnuplotDefinitions);
        ps.println();

        // Title
        if (def.getGraphTitle() != null) {
            ps.println("set title \"" + def.getGraphTitle() + "\"");
        }
        // Axis labels
        if (def.getXlabel() != null) {
            ps.println("set xlabel \"" + def.getXlabel() + "\"");
        }
        if (def.getYlabel() != null) {
            ps.println("set ylabel \"" + def.getYlabel() + "\"");
        }

        // Ranges
        if (def.getXrange() != null) {
            ps.println("set xrange " + def.getXrange());
        }
        if (def.getYrange() != null) {
            ps.println("set yrange " + def.getYrange());
        }

        // Key (legend) position
        if (def.getKeypos() != null) {
            ps.println("set key " + def.getKeypos());
        }

        if (def.getExtraGNUPlot() != null) {
            ps.println(def.getExtraGNUPlot());
            ps.println();
        }

        boolean first = true;
        // Print line plots
        for (LineDefinition line : def.getLines()) {
            if (first) {
                ps.print(def.isIs3d() ? "splot \\\n" : "plot \\\n");
            } else {
                ps.print(", \\\n");
            }
            // Plot file
            ps.print("  '" + line.getDataFile(directory).getPath() + "'");

            // Defs
            if (!isEmpty(line.getDefinition())) {
                ps.print(" " + line.getDefinition());
            }

            // Title
            if (!isEmpty(line.getTitle())) {
                ps.print(" title '" + line.getTitle() + "'");
            } else {
                ps.print(" notitle");
            }

            // Linestyle
            String linestyle = line.getLinestyle();
            if (linestyle == null)
                linestyle = "linespoints ls " + line.getPos();
            linestyle = linestyle.trim();
            if (!isEmpty(linestyle)) {
                ps.print(" with " + linestyle);
            }

            first = false;
        }
        ps.println();

        ps.close();
        fos.close();
    }

    private void loadDataFromDB(LineDefinition line, GraphDefinition graph) throws IOException {
        File datafile = line.getDataFile(directory);
        FileOutputStream fos = new FileOutputStream(datafile);
        PrintStream ps = new PrintStream(fos);
        ps.println(String.format("# Graph %s: %s", graph.getName(), graph.getGraphTitle()));
        ps.println(String.format("# Line %d: %s", line.getPos(), line.getTitle()));
        ps.println(String.format("# SQL: %s", line.getDataFetchSQL()));
        executor.execute(line.getDataFetchSQL(), ps, null);
        ps.close();
        fos.close();
    }

    private Executor initDBExecutor() {
        // Read properties (database directory, credentials, database name,
        // etc.)
        Properties props = DB.getProperties();

        // Create executor object
        Executor executor = new Executor(props);
        executor.connect();

        return executor;
    }

}
