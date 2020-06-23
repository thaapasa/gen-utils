package fi.tuska.util.time;

import java.util.Date;

import org.apache.log4j.Logger;

import junit.framework.TestCase;

public class TimerTest extends TestCase {

    private static final Logger log = Logger.getLogger(TimerTest.class);

    private void smallPause() {
        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
        }
    }

    public void testRunning() {
        Timer timer = new Timer();
        assertTrue(timer.start());
        assertFalse(timer.start());

        for (int i = 0; i < 5; i++) {
            smallPause();
        }

        assertTrue(timer.getElapsedMillis() > 0);

        assertTrue(timer.pause());
        assertFalse(timer.pause());
        long elapsed = timer.getElapsedMillis();

        for (int i = 0; i < 10; i++) {
            smallPause();
        }
        assertEquals(elapsed, timer.getElapsedMillis());

        assertTrue(timer.start());
        assertFalse(timer.start());

        for (int i = 0; i < 2; i++) {
            smallPause();
        }

        assertTrue(timer.stop());
        assertFalse(timer.stop());

        long elapsed2 = timer.getElapsedMillis();
        assertTrue(elapsed2 > elapsed);
        assertTrue(elapsed2 < elapsed * 2);
    }

    public void testShowElapsed() {
        Timer t = new Timer();
        log.info("Starting timer test");
        t.start();
        log.info("Start-time: " + new Date());
        log.info("Start-time: " + t.getElapsedMillis());
        smallPause();
        log.info("Running: " + t.getElapsedMillis());
        smallPause();
        log.info("Mid-time: " + t.getElapsedMillis());
        t.pause();
        log.info("Paused, 1: " + t.getElapsedMillis());
        smallPause();
        smallPause();
        smallPause();
        smallPause();
        log.info("After some time in pause, 1: " + t.getElapsedMillis());
        t.start();
        log.info("Resumed: " + t.getElapsedMillis());
        smallPause();
        log.info("Resumed: " + t.getElapsedMillis());
        smallPause();
        log.info("After running: " + t.getElapsedMillis());
        t.stop();
        log.info("Stopped: " + t.getElapsedMillis());
        log.info("Start time: " + t.getStartTime());
        log.info("End time: " + t.getEndTime());
        log.info("Elapsed by datetimes: "
            + (t.getEndTime().getTime() - t.getStartTime().getTime()));
    }

}
