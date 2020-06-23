#if !defined(UTIL_HASHMAP_HPP)
#define UTIL_HASHMAP_HPP

/*
 * HashMap definitions and correct includes for different versions
 * of compilers
 * 
 * @author Tuukka Haapasalo
 * @created 2004-07-28
 */

#ifdef __GNUC__
#if __GNUC__ < 3
#include <hash_map.h>
namespace sgi {using ::hash_map;}; // inherit globals
#else
#include <ext/hash_map>
#if __GNUC_MINOR__ == 0
namespace sgi = std; // GCC 3.0
#else
namespace sgi = ::__gnu_cxx; // GCC 3.1 and later
#endif
#endif
#else      // ...  there are other compilers, right?
namespace sgi = std;
#endif

/* 
 * To access hash_map, use 
 * sgi::hash_map<KeyType, ValueType>
 */

#endif // !defined(UTIL_HASHMAP_HPP)
