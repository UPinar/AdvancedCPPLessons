#include <iostream>

/*
                ==============================
                | Literal Operator Functions |
                ==============================
*/

/*
  int main()
  {
    875'483;
    0xFF'00'1A'BC;
    0b11111111'00001111'10101010'11001100;
    // digit separator can be used (C++14)
  }
*/

/*
  - parameter variables of literal operator functions are:
    -> unsigned long long (integral constant)
    -> long double (floating point constant)

  - literal operator functions needs to be started with "_"(underscore)

  - cooked literal operator functions
  -----------------------------------
    -> 123_m 
    (compiler will sent "123" expression literal operator _m function
    as an integral constant)
      operator""_m(unsigned long long val);
      compiler will call -> operator""_m(123);
     
  - raw(uncooked) literal operator functions
  ------------------------------------------
    -> "123"_m
    (compiler will sent "123" expression literal operator _m function
    as string literal)
      operator""m(const char* p);
      compiler will call -> operator""m("123");

*/

/*
            -------------------------------------
            | cooked literal operator functions |
            -------------------------------------
*/

/*
  #include <cmath>  // std::sqrt

  double operator""_sr(unsigned long long val)
  {
    std::cout << "operator""_k is called, val =  " << val << '\n';
    return std::sqrt(val);
  }

  int main()
  {
    // ----------------------------------------------------
    double x = 823423_sr;
    std::cout << "x = " << x << '\n'; 
    
    // output ->
    //  operator_k is called, val =  823423
    //  x = 907.427

    // ----------------------------------------------------

    double y = operator""_sr(823423);
    std::cout << "y = " << y << '\n';

    // output ->
    //  operator_k is called, val =  823423
    //  x = 907.427

    // ----------------------------------------------------
  }
*/

/*
  constexpr double operator""_m(long double val) 
  {
    return static_cast<double>(val);
  }

  constexpr double operator""_dm(long double val) 
  {
    return static_cast<double>(val * 10);
  }

  constexpr double operator""_cm(long double val) 
  {
    return static_cast<double>(val * 100);
  }

  constexpr double operator""_mm(long double val) 
  {
    return static_cast<double>(val * 1000);
  }

  constexpr double operator""_km(long double val) 
  {
    return static_cast<double>(val / 1000);
  }

  int main()
  {
    constexpr auto val = 1.23_km + 230.67_m + 
                          45.78_dm + 56.89_cm + 78.90_mm;

    // val is a compile time constant (constexpr variable)
  }
*/

/*
            --------------------------------------------
            | uncooked(raw) literal operator functions |
            --------------------------------------------
*/

/*
  #include <cstring>  // std::strlen

  void operator""_sr(const char* p)
  {
    std::cout << "operator""_k is called. " 
              << std::strlen(p) << '\n';

    while (*p){
      std::cout << *p << ' ' << (int)*p << '\n';
      ++p;
    }
  }

  int main()
  {
    // ----------------------------------------------------
    823423_sr;
    // output ->
    //  operator_k is called. 6
    //  8 56
    //  2 50
    //  3 51
    //  4 52
    //  2 50
    //  3 51

    // ----------------------------------------------------

    89.345_sr;
    // output ->
    //  operator_k is called. 6
    //  8 56
    //  9 57
    //  . 46
    //  3 51
    //  4 52
    //  5 53

    // ----------------------------------------------------
  }
*/

/*
  int operator""_bin(const char* p)
  {
    int ret{};

    while(*p){
      if(*p != '0' || *p != '1')
        throw std::runtime_error("bad binary constant");

      ret = ret * 2 + (*p - '0');
      ++p;
    }

    return ret;
  }

  int main()
  {
    auto val = 10001010_bin;
    std::cout << "val = " << val << '\n'; // output -> val = 138
  }
*/

