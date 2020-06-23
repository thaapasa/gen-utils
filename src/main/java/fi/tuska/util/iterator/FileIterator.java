package fi.tuska.util.iterator;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Iterator;
import java.util.NoSuchElementException;

public class FileIterator implements Iterable<String>, Iterator<String> {

    private final FileReader rd;
    private final BufferedReader bfr;
    private boolean eof = false;

    private String nextLine = null;

    public FileIterator(File file) throws FileNotFoundException {
        rd = new FileReader(file);
        bfr = new BufferedReader(rd);
    }

    @Override
    public Iterator<String> iterator() {
        return this;
    }

    private void seekNext() {
        if (nextLine != null)
            return;
        if (eof)
            return;
        try {
            nextLine = bfr.readLine();
            if (nextLine != null)
                return;
        } catch (IOException e) {
        }

        // If we are here, either an exception occurred or readLine() returned
        // null
        eof = true;
        try {
            bfr.close();
            rd.close();
        } catch (IOException e) {
            // Ignore
        }
    }

    @Override
    public boolean hasNext() {
        seekNext();
        return !eof;
    }

    @Override
    public String next() {
        seekNext();
        if (eof)
            throw new NoSuchElementException();
        String n = nextLine;
        nextLine = null;
        return n;
    }

    @Override
    public void remove() {
        throw new UnsupportedOperationException();
    }

}
