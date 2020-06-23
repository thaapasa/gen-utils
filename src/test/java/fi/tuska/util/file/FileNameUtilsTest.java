package fi.tuska.util.file;

import java.io.File;

import junit.framework.TestCase;

public class FileNameUtilsTest extends TestCase {

    public void testGetExtension() {
        assertNull(FileNameUtils.getExtension((File) null));
        assertNull(FileNameUtils.getExtension((String) null));

        assertEquals("", FileNameUtils.getExtension(""));
        assertEquals("", FileNameUtils.getExtension("kala"));
        assertEquals(".txt", FileNameUtils.getExtension(".txt"));
        assertEquals("", FileNameUtils.getExtension("aa/"));
        assertEquals("", FileNameUtils.getExtension("aa/bb"));
        assertEquals("", FileNameUtils.getExtension(".aa/bb"));
        assertEquals(".bb", FileNameUtils.getExtension(".aa/.bb"));
        assertEquals(".cc", FileNameUtils.getExtension(".aa/bb.cc"));
        assertEquals("", FileNameUtils.getExtension("aa.bb/cc"));
        assertEquals(".ee", FileNameUtils.getExtension("aa.bb/cc.dd.ee"));
        assertEquals("", FileNameUtils.getExtension("aa.bb/cc.dd.ee/ff"));
    }

    public void testForceExtension() {
        assertEquals("aa.bb", FileNameUtils.forceExtension(new File("aa"), ".bb").getName());
        assertEquals("aa.bb", FileNameUtils.forceExtension(new File("aa"), "bb").getName());

        assertEquals("aa.bb", FileNameUtils.forceExtension(new File("aa.bb"), ".bb").getName());
        assertEquals("aa.bb", FileNameUtils.forceExtension(new File("aa.bb"), "bb").getName());
        assertEquals("bb.cc", FileNameUtils.forceExtension(new File("aa/bb"), ".cc").getName());
        assertEquals("bb.cc", FileNameUtils.forceExtension(new File("aa/bb"), "cc").getName());
        assertEquals("bb.cc.dd", FileNameUtils.forceExtension(new File("aa/bb.cc"), ".dd")
            .getName());
        assertEquals("bb.cc.dd", FileNameUtils.forceExtension(new File("aa/bb.cc"), "dd")
            .getName());
        assertEquals("bb.cc.dd", FileNameUtils.forceExtension(new File("aa/bb.cc.dd"), ".dd")
            .getName());
        assertEquals("bb.cc.dd", FileNameUtils.forceExtension(new File("aa/bb.cc.dd"), "dd")
            .getName());
    }
}
