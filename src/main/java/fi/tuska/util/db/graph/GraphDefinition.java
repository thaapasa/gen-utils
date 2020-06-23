package fi.tuska.util.db.graph;

import java.io.File;
import java.util.List;

import org.springframework.beans.factory.annotation.Required;

public class GraphDefinition {

    private String graphTitle;
    private String name;
    private String xlabel;
    private String ylabel;
    private String xrange;
    private String yrange;
    private String keypos;
    private boolean is3d = false;

    private List<LineDefinition> lines;
    private String extraGNUPlot;

    public GraphDefinition() {
    }

    public String getGraphTitle() {
        return graphTitle;
    }

    public void setGraphTitle(String graphTitle) {
        this.graphTitle = graphTitle;
    }

    @Required
    public void setLines(List<LineDefinition> lines) {
        this.lines = lines;
        int i = 0;
        for (LineDefinition line : this.lines) {
            i++;
            line.setPos(i);
            line.setGraph(this);
        }
    }

    public List<LineDefinition> getLines() {
        return lines;
    }

    public String getName() {
        return name;
    }

    public File getPlotFile(File directory) {
        return new File(directory, name + ".plt");
    }

    public File getImageFile(File directory) {
        return new File(directory, name);
    }

    @Required
    public void setName(String name) {
        this.name = name;
    }

    public String getXlabel() {
        return xlabel;
    }

    public void setXlabel(String xlabel) {
        this.xlabel = xlabel;
    }

    public String getYlabel() {
        return ylabel;
    }

    public void setYlabel(String ylabel) {
        this.ylabel = ylabel;
    }

    public String getXrange() {
        return xrange;
    }

    public void setXrange(String xrange) {
        this.xrange = xrange;
    }

    public String getYrange() {
        return yrange;
    }

    public void setYrange(String yrange) {
        this.yrange = yrange;
    }

    public String getKeypos() {
        return keypos;
    }

    public void setKeypos(String keypos) {
        this.keypos = keypos;
    }

    public String getExtraGNUPlot() {
        return extraGNUPlot;
    }

    public void setExtraGNUPlot(String extraGNUPlot) {
        this.extraGNUPlot = extraGNUPlot;
    }

    public boolean isIs3d() {
        return is3d;
    }

    public void setIs3d(boolean is3d) {
        this.is3d = is3d;
    }

}
