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
    -> char (character constant)

  - literal operator functions needs to be started with "_"(underscore)

  - literal operator functions should be defined inside a namespace
    for avoiding ambiguity

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
  #include <string>

  namespace Std{
    inline namespace Literals{
      inline namespace StringLiterals{
        std::string operator""s(const char*, std::size_t);
      }
    }
  }

  int main()
  {
    "hello"s; // syntax error
    // error: unable to find string literal operator 'operator""s' 
    // with 'const char [6]', 'long long unsigned int' arguments

    // because of operator""s is inside a nested namespace

    using namespace std::literals::string_literals;
    // std::literals namespace is an inline namespace
    // std::string_literals is an inline namespace and
    // nested inside std::literals namespace
    auto str = "hello"s;  // VALID
  }
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
  // what if we want to use a string literal as an argument
  // in cooked literal operator function

  void operator""_dt(const char* p, std::size_t n)
  {
    std::puts(p);
    std::cout << "n = " << n << '\n';
  }

  int main()
  {
    "24-06-1999"_dt;
    // output ->
    //  24-06-1999
    //  n = 10
  }
*/

/*
  constexpr int operator""_i(char c)
  {
    return static_cast<int>(c);
  }

  int main()
  {
    using namespace std;

    cout << 'A' << '\n';      // output -> A
    cout << 'A'_i << '\n';    // output -> 65
  }
*/

/*
  #include <string>

  std::string operator""_ds(const char* p, std::size_t)
  {
    return std::string{ p } + p;
  }

  int main()
  {
    using namespace std;

    auto str = "Istanbul"_ds;
    cout << "(" << str << ")\n";
    // output -> (IstanbulIstanbul)
  }
*/

/*
  #include <string>
  #include <vector>

  std::vector<char> operator""_v(const char* p, std::size_t n)
  {
    std::vector<char> ret;

    while(*p)
      ret.push_back(*p++);

    return ret;
  }

  int main()
  {
    auto cvec = "istanbul"_v;

    std::cout << "cvec.size() = " << cvec.size() << '\n';
    // output -> cvec.size() = 8

    for(const auto& c : cvec)
      std::cout << c << ' ';
    // output -> i s t a n b u l
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

/*
  void operator""_pr(const char* p)
  {
    std::cout << p << '\n';
  }

  int main()
  {
    using namespace std;

    34.56_pr;   // output -> 34.56
    123496_pr;  // output -> 123496
  }
*/

/*
          -----------------------------------------------
          | literal operator functions and strong types |
          -----------------------------------------------
*/

/*
  class Meter{
  public:
    struct PreventUsage{};
    Meter(PreventUsage, double dval);
  private:
    double m_val;
  };

  Meter operator ""_m(long double val)
  {
    return Meter{ Meter::PreventUsage{}, static_cast<double>(val) };
  }

  int main()
  {
    Meter m = 4.67_m;
  }
*/

/*
              ---------------------------------
              | raw string literal (REMINDER) |
              ---------------------------------
*/

/*
  #include <cstdio> // std::puts

  int main()
  {
    // hard to read and write
    // ----------------------------------------------------
    
    const char* p = "\"hello world\"";
    std::puts(p);  // output -> "hello world"

    const char* p2 = "\"hello \\ galaxy\"";
    std::puts(p2); // output -> "hello \ galaxy"
    
    const char* p3 =  "\"day " \
                      "night\"";
    std::puts(p3); // output -> "day night"
  }
*/

/*
  #include <string>
  #include <type_traits>  // std::is_same_v

  int main()
  {
    // ----------------------------------------------------

    std::cout << sizeof(R"(hello world)") << '\n';
    // output -> 12
    std::cout << sizeof("hello world") << '\n';
    // output -> 12

    // ----------------------------------------------------

    std::cout << std::boolalpha;

    auto b1 = std::is_same_v<decltype("hey"), const char(&)[4]>;
    std::cout << b1 << '\n';  // output -> true

    auto b2 = std::is_same_v<decltype(R"(hey)"), const char(&)[4]>;
    std::cout << b2 << '\n';  // output -> true

    // ----------------------------------------------------
  }
*/

/*
  #include <string>

  int main()
  {
    // ----------------------------------------------------

    const char* p = (R"(hello world
  we are live
  from Istanbul)");

    std::puts(p);

    // output ->
    //  hello world
    //  we are live
    //  from Istanbul

    // ----------------------------------------------------

    const char* p2 = R"(hello world)";
    // delimiter beginning ==> "(
    // delimiter ending    ==> )"

    std::puts(p2);  // output -> hello world

    const char* p3 = R"del("(hello world)")del";
    // delimiter beginning ==> "del(
    // delimiter ending    ==> )del"

    std::puts(p3);  // output -> "(hello world)"

    // ----------------------------------------------------
  }
*/

/*
                    --------------------------
                    | std::quoted (REMINDER) |
                    --------------------------
*/

/*
  #include <iomanip>      // std::quoted
  #include <type_traits>  // std::is_same_v

  int main()
  {
    auto str = std::quoted("hello"); 
    // std::quoted function's return type is unspecified

    constexpr auto b = std::is_same_v<decltype(str), std::string>;
    std::cout << std::boolalpha << b << '\n'; // output -> false
  }
*/

/*
  #include <iomanip>  // std::quoted

  int main()
  {
    using namespace std;

    cout << quoted("hello") << '\n';  
    // output -> "hello"

    cout << quoted("\"hello\"") << '\n';  
    // output -> "\"hello\""

    cout << quoted(R"("istanbul" "ankara" "izmir")") << '\n';
    // output -> "\"istanbul\" \"ankara\" \"izmir\""
  }
*/

/*
  #include <iomanip>  // std::quoted
  #include <sstream>  // std::ostringstream

  int main()
  {
    using namespace std;

    // ----------------------------------------------------

    ostringstream oss_1;
    oss_1 << "\"Istanbul\"";

    cout << "|" << oss_1.str() << "|\n";  
    // output -> |"Istanbul"|

    // ----------------------------------------------------

    ostringstream oss_2;
    oss_2 << quoted("\"Istanbul\"");

    cout << "|" << oss_2.str() << "|\n";
    // output -> |"\"Istanbul\""|

    // ----------------------------------------------------
  }
*/

/*
  #include <iomanip>  // std::quoted
  #include <sstream>  // std::ostringstream

  int main()
  {
    using namespace std;

    // "*" character is being used as delimiter
    // ----------------------------------------------------

    ostringstream oss;
    oss << quoted("istanbul", '*');
    cout << oss.str() << '\n';  
    // output -> "*istanbul*"

    // ----------------------------------------------------

    ostringstream oss2;
    oss2 << quoted("*istanbul* *ankara*", '*');
    cout << oss2.str() << '\n';  
    // output -> *\*istanbul\* \*ankara\**

    // ----------------------------------------------------
  }
*/

/*
  #include <iomanip>  // std::quoted
  #include <sstream>  // std::ostringstream

  int main()
  {
    using namespace std;

    // "*" character is being used as delimiter
    // "_" character is being used as escape character

    ostringstream oss;
    oss << quoted("*istanbul* *ankara*", '*', '_');

    cout << oss.str() << '\n';  
    // output -> *_*istanbul_* _*ankara_**
  }
*/

/*
  #include <sstream>  // std::istringstream
  #include <iomanip>  // std::quoted

  int main()
  {
    using namespace std;

    // ----------------------------------------------------

    istringstream iss{"\"istanbul\""};
    string city;

    iss >> city;
    cout << city << '\n';  
    // output -> "istanbul"

    // ----------------------------------------------------

    istringstream iss2{"\"istanbul\""};
    string city2;

    iss2 >> quoted(city2);
    cout << city2 << '\n';  
    // output -> istanbul

    // ----------------------------------------------------
  }
*/

/*
  #include <sstream>  // std::istringstream
  #include <iomanip>  // std::quoted

  int main()
  {
    using namespace std;

    istringstream iss{"*istanbul*"};
    string city;

    iss >> quoted(city, '*');
    cout << city << '\n'; 
    // output -> istanbul
  }
*/