package fi.tuska.util.file;

import java.beans.PropertyEditorSupport;
import java.io.File;

public class CustomFileEditor extends PropertyEditorSupport {

    private File file;

    public CustomFileEditor() {
        // Creates the editor
    }

    @Override
    public String getAsText() {
        return file.toString();
    }

    @Override
    public Object getValue() {
        return file;
    }

    @Override
    public void setAsText(String text) throws IllegalArgumentException {
        this.file = new File(text);
    }

    @Override
    public void setValue(Object value) {
        this.file = (File) value;
    }

}
