package fi.tuska.util.media;

import java.awt.Color;

public class ColorUtils {

    private ColorUtils() {
        // Private constructor to prevent instantiation
    }

    public static Color toMonochrome(Color color) {
        int r = color.getRed();
        int g = color.getGreen();
        int b = color.getBlue();
        if (r == g && g == b) {
            return color;
        }
        int mid = (r + g + b) / 3;
        return new Color(mid, mid, mid);
    }

    public static Color lighten(Color color, double amount) {
        return new Color(lighten(color.getRed(), amount), lighten(color.getGreen(), amount),
            lighten(color.getBlue(), amount));
    }

    protected static int lighten(int component, double amount) {
        return 255 - (int) ((1 - amount) * (255 - component));
    }

    public static Color darken(Color color, double amount) {
        return new Color(darken(color.getRed(), amount), darken(color.getGreen(), amount),
            darken(color.getBlue(), amount));
    }

    protected static int darken(int component, double amount) {
        return (int) ((1 - amount) * component);
    }

}
