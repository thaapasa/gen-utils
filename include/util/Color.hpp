#if !defined(UTIL_COLOR_HPP)
#define UTIL_COLOR_HPP

/*
 * A basic three or four component color template
 * 
 * @author Tuukka Haapasalo
 * @created 2004-10-21
 */

namespace util {

template<typename T>
class Color {
public:
    /* Creates a color with red, green, blue and alpha values set */
    Color(T red, T green, T blue, T alpha = 0);
    /* Copies the color */
    Color(Color const & color);
    /* Assigns the color */
    Color const & operator=(Color const & color);
    /* Destroys the color */
    ~Color();

    /* Compares two colors for equality */
    bool operator==(Color const & color) const;
    /* Compares two colors for inequality */
    bool operator!=(Color const & color) const;

    /* Returns the red component */
    inline T red() const;
    /* Returns the green component */
    inline T green() const;
    /* Returns the blue component */
    inline T blue() const;
    /* Returns the alpha component */
    inline T alpha() const;

    /* Sets the red component */
    inline void red(T value);
    /* Sets the green component */
    inline void green(T value);
    /* Sets the blue component */
    inline void blue(T value);
    /* Sets the alpha component */
    inline void alpha(T value);

    /* Sets the color components */
    inline void set(T red, T green, T blue);
    /* Sets the color components and the alpha value */
    inline void set(T red, T green, T blue, T alpha);

private:
    T values[4];
};

}

/*
 * Template method definitions
 */

/* 
 * Creates a color with red, green, blue and alpha values set 
 */
template<typename T>
util::Color<T>::Color(T red, T green, T blue, T alpha) {
    values[0] = red;
    values[1] = green;
    values[2] = blue;
    values[3] = alpha;
}

/* 
 * Copies the color 
 */
template<typename T>
util::Color<T>::Color(Color const & color) {
    values[0] = color.values[0];
    values[1] = color.values[1];
    values[2] = color.values[2];
    values[3] = color.values[3];
}

/* 
 * Assigns the color 
 */
template<typename T>
util::Color<T>::Color<T> const & util::Color<T>::operator=(Color const & color) {
    values[0] = color.values[0];
    values[1] = color.values[1];
    values[2] = color.values[2];
    values[3] = color.values[3];
}

/* 
 * Destroys the color 
 */
template<typename T>
util::Color<T>::~Color() {
}

/* 
 * Compares two colors for equality 
 */
template<typename T>
bool util::Color<T>::operator==(Color const & color) const {
    return values[0] == color.values[0] && values[1] == color.values[1]
            && values[2] == color.values[2] && values[3] == color.values[3];
}

/* 
 * Compares two colors for inequality 
 */
template<typename T>
bool util::Color<T>::operator!=(Color const & color) const {
    return !(*this == color);
}

/* 
 * Returns the red component 
 */
template<typename T>
inline T util::Color<T>::red() const {
    return values[0];
}

/* 
 * Returns the green component 
 */
template<typename T>
inline T util::Color<T>::green() const {
    return values[1];
}

/* 
 * Returns the blue component 
 */
template<typename T>
inline T util::Color<T>::blue() const {
    return values[2];
}

/* 
 * Returns the alpha component 
 */
template<typename T>
inline T util::Color<T>::alpha() const {
    return values[3];
}

/* 
 * Sets the red component 
 */
template<typename T>
inline void util::Color<T>::red(T value) {
    values[0] = value;
}

/* 
 * Sets the green component 
 */
template<typename T>
inline void util::Color<T>::green(T value) {
    values[1] = value;
}

/* 
 * Sets the blue component 
 */
template<typename T>
inline void util::Color<T>::blue(T red) {
    values[2] = value;
}

/* 
 * Sets the alpha component 
 */
template<typename T>
inline void util::Color<T>::alpha(T red) {
    values[3] = value;
}

/* 
 * Sets the color components 
 */
template<typename T>
inline void util::Color<T>::set(T red, T green, T blue) {
    values[0] = red;
    values[1] = green;
    values[2] = blue;
}

/* 
 * Sets the color components and the alpha value 
 */
template<typename T>
inline void util::Color<T>::set(T red, T green, T blue, T alpha) {
    values[0] = red;
    values[1] = green;
    values[2] = blue;
    values[3] = alpha;
}

#endif // !defined(UTIL_COLOR_HPP)
