#if !defined(UTIL_RANDOM_HPP)
#define UTIL_RANDOM_HPP

/*
 * Provides a native and a sequential random number generator
 *
 * @author Tuukka Haapasalo
 * @created 2004-07-21
 */

namespace util {

/*
 * Provides the basic interface for the a random number generator
 */
class Random {
protected:
    /* Protected constructor for subclasses */
    Random();

    /* 
     * The static instance of Random. Can be set, or will be created 
     * when called via getInstance().
     */
    static Random * instance;

public:
    /* Virtual destructor */
    virtual ~Random();

    /* Returns the static Random instance */
    static Random * getInstance();

    /* Manually sets the static Random instance */
    static void setInstance(Random * random);

    /* Return a random number between [0..1[ */
    virtual double getRandom() = 0;

    /* Return a random number between [min..max[ */
    virtual double getRandom(double const min, double const max);

    /* Seeds the random number generator with given number */
    virtual void seed(int const number) = 0;

    /* Seeds the random number generator with current time */
    void seed();

private:
    /* Copy constructor and assignment operator not usable 
     * (these are not defined) */
    Random(Random const & other);
    void operator=(Random const & other);

};

/*
 * Native implementation of random number generator
 */
class NativeRandom: public Random {
public:

    /*
     * Creates a native random generator, seeding it with a number fetched
     * from system time
     */
    NativeRandom();
    /* Creates a native random generator, seeding it with given number */
    NativeRandom(int const seed);
    /* Destroys the native random number generator */
    virtual ~NativeRandom();

    /* Returns a random number between [0..1[ */
    virtual double getRandom();

    /* Seeds the random number generator with given number */
    virtual void seed(int const number);

private:
    /* Copy constructor and assignment operator not usable 
     * (these are not defined) */
    NativeRandom(NativeRandom const & other);
    void operator=(NativeRandom const & other);
};

/*
 * Table implementation of random number generator.
 *
 * This random generator implementation is adapted from Appendix B of
 * Rick Parent's Computer Animation - Algorithms and Techniques.
 */
class TableRandom: public Random {
public:

    /*
     * Creates a table-based random generator, seeding it with a number fetched
     * from system time
     */
    TableRandom();
    /* Creates a table-based random generator, seeding it with given number */
    TableRandom(int const seed);
    /* Destroys the table-based random number generator */
    virtual ~TableRandom();

    /* Returns a random number between [0..1[ */
    virtual double getRandom();

    /* Seeds the random number generator with given number */
    virtual void seed(int const number);

private:
    /* Copy constructor and assignment operator not usable 
     * (these are not defined) */
    TableRandom(TableRandom const & other);
    void operator=(TableRandom const & other);

private:
    /* Pseudo-random table */
    mutable int data[101];
};

}

#endif // !defined(UTIL_RANDOM_HPP)
