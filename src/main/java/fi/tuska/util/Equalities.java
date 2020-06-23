package fi.tuska.util;

public class Equalities {

    public static <T> boolean equalsOrNull(T a, T b) {
        if (a == null)
            return b == null;
        return a.equals(b);
    }

}
