package fi.tuska.util;

import java.io.BufferedReader;
import java.io.Closeable;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.util.ArrayList;
import java.util.List;

import org.apache.log4j.Logger;

public class StreamUtils {

    private static final Logger log = Logger.getLogger(StreamUtils.class);

    public static List<String> readFromReader(Reader reader) {
        BufferedReader pr = null;
        try {
            pr = new BufferedReader(reader);

            List<String> result = new ArrayList<String>();
            String line = null;
            while ((line = pr.readLine()) != null) {
                result.add(line);
            }
            pr.close();
            return result;
        } catch (IOException e) {
            log
                .debug(String
                    .format("Could not read from %s, reason: %s", reader, e.getMessage()));
            tryToClose(pr);
            return null;
        }
    }

    public static List<String> readFromStream(InputStream stream) {
        InputStreamReader reader = new InputStreamReader(stream);
        List<String> lines = readFromReader(reader);
        tryToClose(reader);
        return lines;
    }

    public static void tryToClose(Closeable c) {
        if (c != null) {
            try {
                c.close();
            } catch (IOException e) {
                // Ignore
            }
        }
    }

}
