package fi.tuska.util.time;

import java.io.PrintStream;
import java.util.Date;

public class Timer {

    private enum State {
        UNINITIALIZED, RUNNING, PAUSED, STOPPED
    };

    private State state;
    private long elapsed;
    private long startMillis;

    private Date startTime;
    private Date endTime;

    public Timer() {
        reset();
    }

    /**
     * Clears the timer, resetting all calculated values.
     */
    public void reset() {
        state = State.UNINITIALIZED;
        elapsed = 0;
        startTime = null;
        endTime = null;
    }

    /**
     * @return true if the timer has been started (and not yet stopped); the
     * timer can be either running or paused.
     */
    public boolean isStarted() {
        return state == State.RUNNING || state == State.PAUSED;
    }

    /**
     * @return true if the timer is currently paused
     */
    public boolean isPaused() {
        return state == State.PAUSED;
    }

    /**
     * @return true if the timer is currently running (and not paused)
     */
    public boolean isRunning() {
        return state == State.RUNNING;
    }

    /**
     * @return true if the timer is currently stopped
     */
    public boolean isStopped() {
        return state == State.STOPPED;
    }

    /**
     * Starts the timer, or resumes a paused timer.
     * 
     * @return true if the timer was started; false if the timer was already
     * running.
     * @throws IllegalStateException if the timer has already been stopped
     * (and not reset)
     */
    public boolean start() {
        if (state == State.STOPPED)
            throw new IllegalStateException("Timer already stopped");
        if (state == State.RUNNING)
            return false;

        if (state == State.UNINITIALIZED)
            startTime = new Date();
        // state might also be PAUSED, that is ok - resume timer
        assert startTime != null;
        startMillis = System.currentTimeMillis();
        state = State.RUNNING;
        return true;
    }

    /**
     * Stops the timer. After calling this, you can only query for the
     * recorded times or reset the timer.
     * 
     * @return true if the timer was stopped, false if it was already stopped.
     * @throws IllegalStateException if the timer has not been started
     */
    public boolean stop() {
        if (state == State.UNINITIALIZED)
            throw new IllegalStateException("Timer not started");
        if (state == State.STOPPED)
            return false;
        if (state == State.RUNNING) {
            elapsed += System.currentTimeMillis() - startMillis;
        }
        // state might also be PAUSED, in which case we only record the end
        // time (no change to elapsed), and set new state
        endTime = new Date();
        startMillis = 0;
        state = State.STOPPED;
        return true;
    }

    /**
     * Pauses the timer.
     * 
     * @return true if the timer was paused, false if the timer already was
     * paused.
     */
    public boolean pause() {
        if (state == State.UNINITIALIZED)
            throw new IllegalStateException("Timer not started");
        if (state == State.STOPPED)
            throw new IllegalStateException("Timer already stopped");
        if (state == State.PAUSED)
            return false;
        assert state == State.RUNNING;

        // Add elapsed time
        elapsed += System.currentTimeMillis() - startMillis;
        startMillis = 0;

        state = State.PAUSED;
        return true;
    }

    /**
     * @return the time when the timer was first started
     */
    public Date getStartTime() {
        if (state == State.UNINITIALIZED)
            throw new IllegalStateException("Timer not started");
        return startTime;
    }

    /**
     * @return the time when the timer was stopped
     */
    public Date getEndTime() {
        if (state != State.STOPPED)
            throw new IllegalStateException("Timer not stopped");
        return endTime;
    }

    /**
     * @return the amount of time elapsed (in milliseconds), not including the
     * times when the timer was paused.
     */
    public long getElapsedMillis() {
        if (state == State.UNINITIALIZED)
            throw new IllegalStateException("Timer not started");
        if (state == State.RUNNING) {
            // Calculate new elapsed time, do not affect counter values
            return elapsed + (System.currentTimeMillis() - startMillis);
        }
        return elapsed;
    }

    /**
     * @return the amount of time elapsed (in seconds), not including the
     * times when the timer was paused.
     */
    public double getElapsedSeconds() {
        return (double) getElapsedMillis() / (double) 1000.0;
    }

    /**
     * Reports the time to the given output stream.
     * 
     * @param out the output stream where the time is reported to
     */
    public void reportTime(PrintStream out) {
        double t = getElapsedSeconds();
        out.println(String.format("Time elapsed: %.3f", t));
    }

}
