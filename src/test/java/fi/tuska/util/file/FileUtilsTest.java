package fi.tuska.util.file;

import java.io.File;

import fi.tuska.util.CollectionUtils;

import junit.framework.TestCase;

public class FileUtilsTest extends TestCase {

    public void testFileCopyAndDelete() {
        File file1 = new File("test1.txt");
        File file2 = new File("test2.txt");

        if (file1.exists()) {
            assertTrue(file1.delete());
        }
        if (file2.exists()) {
            assertTrue(file2.delete());
        }

        FileUtils.writeToFile(file1, CollectionUtils.getList("just", "testing"));
        assertTrue(file1.exists());

        assertTrue(FileUtils.copy(file1, file2));
        assertTrue(file1.exists());
        assertTrue(file2.exists());

        assertTrue(file1.delete());
        assertTrue(file2.delete());

        assertFalse(file1.exists());
        assertFalse(file2.exists());
    }

}
