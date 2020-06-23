package fi.tuska.util.media;

import java.awt.Color;

import junit.framework.TestCase;

public class ColorUtilsTest extends TestCase {

    public void testToMonochrome() {
        assertEquals(new Color(128, 128, 128), ColorUtils.toMonochrome(new Color(128, 128, 128)));
        assertEquals(new Color(128, 128, 128), ColorUtils.toMonochrome(new Color(120, 128, 136)));
        assertEquals(new Color(127, 127, 127), ColorUtils.toMonochrome(new Color(255, 128, 0)));
    }

    public void testLighten() {
        assertEquals(new Color(255, 255, 255), ColorUtils.lighten(new Color(0, 120, 30), 1));
        assertEquals(new Color(255, 255, 255), ColorUtils.lighten(new Color(0, 0, 0), 1));
        assertEquals(new Color(255, 255, 255), ColorUtils.lighten(new Color(255, 255, 255), 1));

        assertEquals(new Color(0, 0, 0), ColorUtils.lighten(new Color(0, 0, 0), 0));
        assertEquals(new Color(120, 200, 0), ColorUtils.lighten(new Color(120, 200, 0), 0));

        assertEquals(new Color(131, 139, 187), ColorUtils.lighten(new Color(100, 110, 170), 0.2));
    }

}
