package fi.tuska.util.args;

import java.util.Properties;

import junit.framework.TestCase;

public class CommandLineParserTest extends TestCase {

    public void testParse() {
        Properties props = new Properties();
        Properties empty = new Properties();

        assertEquals(props, empty);
        props.put("test", "moi");
        assertFalse(props.equals(empty));

        props.clear();
        CommandLineParser.readProperties(new String[] { "--a", "b" }, props);
        assertEquals("b", props.get("a"));

        props.clear();
        CommandLineParser.readProperties(new String[] { "--a=b" }, props);
        assertEquals("b", props.get("a"));

        props.clear();
        CommandLineParser.readProperties(new String[] { "--a=b", "c" }, props);
        assertEquals("b", props.get("a"));
        assertEquals("c", CommandLineParser.getDefaultArgument(props));

        props.clear();
        CommandLineParser.readProperties(new String[] { "-a", "b", "c" }, props);
        assertEquals("b", props.get("a"));
        assertEquals("c", CommandLineParser.getDefaultArgument(props));
    }
}
