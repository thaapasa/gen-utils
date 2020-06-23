package fi.tuska.util.file;

import java.io.File;

public class FileNameUtils {

    private FileNameUtils() {
        // Private constructor to prevent instantiation
    }

    /**
     * Returns the extension of the file, with the dot (".png"), or an empty
     * string, if the file contains no extension.
     * 
     * @param filename the filename
     * @return the extension of the file with the dot (".png"), or empty
     * string. Returns null if the filename parameter is null.
     */
    public static String getExtension(String filename) {
        if (filename == null) {
            return null;
        }
        File file = new File(filename);
        return getExtension(file);
    }

    /**
     * Returns the extension of the file, with the dot (".png"), or an empty
     * string, if the file contains no extension.
     * 
     * @param file the file
     * @return the extension of the file, with the dot (".png") or empty
     * string. Returns null if the file parameter is null.
     */
    public static String getExtension(File file) {
        if (file == null) {
            return null;
        }
        String name = file.getName();
        int lastDotPos = name.lastIndexOf('.');
        return lastDotPos >= 0 ? name.substring(lastDotPos) : "";
    }

    /**
     * Checks that the file has the given extension, adding it if necessary.
     * 
     * @param file the file
     * @param extension the required extension
     * @return the original file + the extension, if it was not already
     * present
     */
    public static File forceExtension(File file, String extension) {
        if (!extension.startsWith(".")) {
            extension = "." + extension;
        }
        String ext = getExtension(file);
        if (ext == null) {
            return null;
        }
        if (ext.equalsIgnoreCase(extension)) {
            // Correct extension already
            return file;
        }
        return new File(file.getParent(), file.getName() + extension);
    }
}
