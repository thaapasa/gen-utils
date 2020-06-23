package fi.tuska.util.iterator;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.util.Iterator;

public class ReaderIterator implements Iterator<String>, Iterable<String> {

    private Reader reader;
    private BufferedReader buffer;
    private String nextLine;
    private boolean eof;

    public ReaderIterator(Reader reader) {
        this.reader = reader;
        buffer = new BufferedReader(reader);
        eof = false;
        readNext();
    }

    public ReaderIterator(InputStream stream) {
        this(new InputStreamReader(stream));
    }

    protected void readNext() {
        if (eof)
            return;
        try {
            nextLine = buffer.readLine();
            if (nextLine == null) {
                // End-of-file reached, mark it and return null
                eof = true;
                buffer.close();
                reader.close();
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    public boolean hasNext() {
        return nextLine != null;
    }

    @Override
    public String next() {
        String result = nextLine;
        readNext();
        return result;
    }

    @Override
    public void remove() {
        throw new UnsupportedOperationException("remove() not supported");
    }

    @Override
    public Iterator<String> iterator() {
        return this;
    }

}
