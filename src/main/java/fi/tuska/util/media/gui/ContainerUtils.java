package fi.tuska.util.media.gui;

import java.awt.Component;
import java.awt.FlowLayout;

import javax.swing.JPanel;

public class ContainerUtils {

    private ContainerUtils() {
        // Private constructor to prevent instantiation
    }

    public static Component getPaddedComponent(Component component, int hPad, int vPad) {
        FlowLayout layout = new FlowLayout(FlowLayout.CENTER, hPad, vPad);
        JPanel panel = new JPanel(layout);
        panel.add(component);
        return panel;
    }

}
