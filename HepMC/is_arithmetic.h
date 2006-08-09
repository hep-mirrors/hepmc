#ifndef IS_ARITHMETIC
#define IS_ARITHMETIC
// author: Walter Brown

// ----------------------------------------------------------------------
// prolog

namespace HepMC  {
namespace detail  {


// ----------------------------------------------------------------------
// is_arithmetic<>

template< class T >
  struct is_arithmetic
{
  static  bool const  value = false;
};

template<>
  struct is_arithmetic<char>
{ static  bool const  value = true; };

template<>
  struct is_arithmetic<unsigned char>
{ static  bool const  value = true; };

template<>
  struct is_arithmetic<signed char>
{ static  bool const  value = true; };

template<>
  struct is_arithmetic<short>
{ static  bool const  value = true; };

template<>
  struct is_arithmetic<unsigned short>
{ static  bool const  value = true; };

template<>
  struct is_arithmetic<int>
{ static  bool const  value = true; };

template<>
  struct is_arithmetic<unsigned int>
{ static  bool const  value = true; };

template<>
  struct is_arithmetic<long>
{ static  bool const  value = true; };

template<>
  struct is_arithmetic<unsigned long>
{ static  bool const  value = true; };

template<>
  struct is_arithmetic<float>
{ static  bool const  value = true; };

template<>
  struct is_arithmetic<double>
{ static  bool const  value = true; };

template<>
  struct is_arithmetic<long double>
{ static  bool const  value = true; };


// ----------------------------------------------------------------------
// epilog

}  // namespace detail
}  // namespace HepMC

#endif  // IS_ARITHMETIC
