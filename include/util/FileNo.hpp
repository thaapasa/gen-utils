#if !defined(UTIL_FILENO_HPP)
#define UTIL_FILENO_HPP

// This code is copied from
// fileno(3) on C++ Streams: A Hacker's Lament
// by Richard B. Kreckel
// http://www.ginac.de/~kreckel/fileno/

#include <iosfwd>

template <typename charT, typename traits>
int getFileDescriptor(const std::basic_ios<charT, traits>& stream);

#endif // !defined(UTIL_FILENO_HPP)
