#if !defined(UTIL_CLONEABLE_HPP)
#define UTIL_CLONEABLE_HPP

/*
 * Base class for cloneable objects.
 * 
 * @author Tuukka Haapasalo
 * @created 2004-06-22
 */

#define CLONEMETHOD(type) virtual util::Cloneable * clone() const { return new type(*this); }

namespace util {

class Cloneable {
public:
    virtual ~Cloneable() {
    }

    /*
     * Clones the object
     */
    virtual Cloneable * clone() const = 0;

};

}

#endif // !defined(UTIL_CLONEABLE_HPP)
