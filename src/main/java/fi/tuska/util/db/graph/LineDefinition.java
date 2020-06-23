package fi.tuska.util.db.graph;

import java.io.File;

public class LineDefinition {

    private String dataFetchSQL;
    private String title;
    private String linestyle;
    private String definition;
    private int pos;
    private GraphDefinition graph;

    public LineDefinition() {
    }

    protected void setGraph(GraphDefinition graph) {
        this.graph = graph;
    }

    protected void setPos(int pos) {
        this.pos = pos;
    }

    public int getPos() {
        return pos;
    }

    public File getDataFile(File directory) {
        return new File(directory, graph.getName() + "-" + pos + ".data");
    }

    public void setDataFetchSQL(String dataFetchSQL) {
        this.dataFetchSQL = dataFetchSQL;
    }

    public String getDataFetchSQL() {
        return dataFetchSQL;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getLinestyle() {
        return linestyle;
    }

    public void setLinestyle(String linestyle) {
        this.linestyle = linestyle;
    }

    public String getDefinition() {
        return definition;
    }

    public void setDefinition(String def) {
        this.definition = def;
    }

}
