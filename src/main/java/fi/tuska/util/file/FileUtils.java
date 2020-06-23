package fi.tuska.util.file;

import java.io.Closeable;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.channels.FileChannel;
import java.util.List;

import org.apache.log4j.Logger;

import fi.tuska.util.StreamUtils;

public class FileUtils {

    private static final Logger log = Logger.getLogger(FileUtils.class);

    private FileUtils() {
        // Private constructor to prevent instantiation
    }

    /**
     * Fast and simple file copy.
     * 
     * <p>
     * Source:
     * 
     * <a href="http://forums.sun.com/thread.jspa?threadID=644995">http://
     * forums.sun.com/thread.jspa?threadID=644995</a>
     * 
     * 
     */
    public static boolean copy(File source, File dest) {
        FileChannel in = null, out = null;
        FileInputStream fis = null;
        FileOutputStream fos = null;
        try {
            fis = new FileInputStream(source);
            in = fis.getChannel();
            fos = new FileOutputStream(dest);
            out = fos.getChannel();

            in.transferTo(0, in.size(), out);

            out.close();
            fos.close();

            in.close();
            fis.close();
            return true;
        } catch (IOException e) {
            log.warn("Could not copy file " + source + " to " + dest + ": " + e);
            tryToClose(in);
            tryToClose(out);
            tryToClose(fis);
            tryToClose(fos);
            return false;
        }
    }

    /**
     * Writes the given text lines into a file.
     * 
     * @param file the file to write to
     * @param lines the text lines to write
     * @return true on success; false on failure
     */
    public static boolean writeToFile(File file, List<String> lines) {
        FileWriter writer = null;
        PrintWriter pw = null;
        try {
            writer = new FileWriter(file);
            pw = new PrintWriter(writer);

            for (String line : lines) {
                pw.println(line);
            }
            pw.flush();
            writer.flush();

            pw.close();
            writer.close();
            return true;
        } catch (IOException e) {
            log.debug(String.format("Could not save file %s, reason: %s", file, e.getMessage()));
            tryToClose(pw);
            tryToClose(writer);
            return false;
        }
    }

    /**
     * Reads all text lines from a file.
     * 
     * @param file the file to write to
     * @return the text lines; or null if file was not found or could not be
     * accessed
     */
    public static List<String> readFromFile(File file) {
        FileReader reader = null;
        try {
            reader = new FileReader(file);
            return StreamUtils.readFromReader(reader);
        } catch (FileNotFoundException e) {
            log.debug(String.format("File %s not found", file));
            return null;
        } finally {
            tryToClose(reader);
        }
    }

    private static void tryToClose(Closeable c) {
        if (c != null) {
            try {
                c.close();
            } catch (IOException e) {
                // Ignore
            }
        }
    }

}
