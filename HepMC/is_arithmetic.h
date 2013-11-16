#ifndef IS_ARITHMETIC
#define IS_ARITHMETIC

namespace HepMC  {

  /// Internal namespace for utility functions
  ///
  /// @author Walter Brown
  /// @todo Can be replaced by standard C++11 type traits features
  namespace detail  {

    /// @name Arithmetic type testing
    //@{

    /// Generic template form is non-arithmetic
    template< class T >
    struct is_arithmetic
    {
      static  bool const  value = false;
    };

    /// Specialise character as arithmetic
    template<>
    struct is_arithmetic<char>
    { static  bool const  value = true; };

    /// Specialise unsigned character as arithmetic
    template<>
    struct is_arithmetic<unsigned char>
    { static  bool const  value = true; };

    /// Specialise signed character as arithmetic
    template<>
    struct is_arithmetic<signed char>
    { static  bool const  value = true; };

    /// Specialise short as arithmetic
    template<>
    struct is_arithmetic<short>
    { static  bool const  value = true; };

    /// Specialise unsigned short as arithmetic
    template<>
    struct is_arithmetic<unsigned short>
    { static  bool const  value = true; };

    /// Specialise int as arithmetic
    template<>
    struct is_arithmetic<int>
    { static  bool const  value = true; };

    /// Specialise unsigned int as arithmetic
    template<>
    struct is_arithmetic<unsigned int>
    { static  bool const  value = true; };

    /// Specialise long as arithmetic
    template<>
    struct is_arithmetic<long>
    { static  bool const  value = true; };

    /// Specialise unsigned long as arithmetic
    template<>
    struct is_arithmetic<unsigned long>
    { static  bool const  value = true; };

    /// Specialise float as arithmetic
    template<>
    struct is_arithmetic<float>
    { static  bool const  value = true; };

    /// Specialise double as arithmetic
    template<>
    struct is_arithmetic<double>
    { static  bool const  value = true; };

    /// Specialise long double as arithmetic
    template<>
    struct is_arithmetic<long double>
    { static  bool const  value = true; };

    //@}

  }  // namespace detail
}  // namespace HepMC

#endif  // IS_ARITHMETIC
