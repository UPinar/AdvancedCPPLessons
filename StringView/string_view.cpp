#include <iostream>

/*
              =====================================
              | `std::string_view` class template |
              =====================================
*/

/*
  strings can be hold in 
  - std::string
  - char str[120]
  - std::vector<char>
  - std::array<char, 10>
  - string literals "hello"

  -> those characters are contigious in memory

  - if we have 2 pointers (or 1 pointer and a size)
    - one points to the first character
    - second points to end of the string(last character + 1) 

    because of the characters are contigious in memory
    we can view the string with 2 pointers
    and we can be  independent from the container type 
    that it holds the string
*/

/*
  class StringView{
  public:
    // std::string class's non-const member functions
    // inside StringView class's interface
    // size, length, empty, find, rfind, find_first_of etc.
  private:
    const char* mp_start;
    const char* mp_end;
  };

  class StringView_2{
  private:
    const char* mp_start;
    std::size_t ms_elem;
  };
*/

/*
  #include <string_view>

  int main()
  {
    std::cout << "sizeof(char*) = " << sizeof(char*) << '\n';
    // output -> sizeof(char*) = 8

    std::cout << "sizeof(std::string_view) = " 
              << sizeof(std::string_view) << '\n';
    // output -> sizeof(std::string_view) = 16
  }
*/

/*
  #include <string>
  #include <string_view>

  void func(const std::string& str);
  void foo(const char* p);
  void bar(std::string_view sv);

  int main()
  {
    func("hello world we are live from Istanbul, Turkey");
    // temp std::string object will be created
    // copying characters of string literal to the temp object

    foo("hello world we are live from Istanbul, Turkey");
    // no temp object will be created
    // but foo function can not be called with std::string object
    // and we can not use std::string classes interface

    bar("hello world we are live from Istanbul, Turkey");
    // std::string_view object will be created
    // copying a class object which have 2 pointers
  }
*/

/*
  #include <string>

  int main()
  {
    using namespace std;

    std::string str(100'000, 'a');

    size_t idx{ 4576 };
    size_t n{ 50'000 };

    auto sub_str = str.substr(idx, n);
    // "substr" member function creates a new std::string object
    // in this scenario it copies 
    // 50'000 characters from str to sub_str

    // if sub_str will not be modified(immutable usage of sub_str)
    // better using std::string_view class template
  }
*/

/*
  2 possible problematic scenarios
    - dangling pointer
    - string that string_view points to does not need to be NTBS 
      passing an address generated from string_view 
      to a function that expects a NTBS
*/

/*
              ------------------------------------
              | string_view classes constructors |
              ------------------------------------
*/

/*
  // how to construct a string_view object

  #include <string_view>

  int main()
  {
    using namespace std;

    // --------------------------------------------------------

    string_view sv1;  // default initialize

    cout << boolalpha;
    cout << "sv1.size() = " << sv1.size() << '\n';
    // output -> sv1.size() = 0
    cout << "sv1.length() = " << sv1.length() << '\n';
    // output -> sv1.length() = 0
    cout << "sv1.empty() = " << sv1.empty() << '\n';
    // output -> sv1.empty() = true
    cout << (sv1.data() == nullptr) << '\n';
    // output -> true

    // --------------------------------------------------------

    string_view sv2{};  // value initialize

    cout << boolalpha;
    cout << "sv2.size() = " << sv2.size() << '\n';
    // output -> sv2.size() = 0
    cout << "sv2.length() = " << sv2.length() << '\n';
    // output -> sv2.length() = 0
    cout << "sv2.empty() = " << sv2.empty() << '\n';
    // output -> sv2.empty() = true
    cout << (sv2.data() == nullptr) << '\n';
    // output -> true

    // --------------------------------------------------------
  }
*/

/*
  // initializing a string_view object with C-style string(NTBS)
  
  #include <string_view>

  int main()
  {
    using namespace std;

    // --------------------------------------------------------

    string_view sv1{ "hello world" };

    cout << "|" << sv1 << "|\n";
    // output -> |hello world|

    // --------------------------------------------------------

    char str[] = "hello galaxy";
    string_view sv2{ str };

    cout << "|" << sv2 << "|\n";
    // output -> |hello galaxy|

    // --------------------------------------------------------

    // C-style string parameter constructor is not explicit
    // copy initialization is VALID

    string_view sv3 = "hello universe"; // copy initialization
    cout << "|" << sv3 << "|\n";
    // output -> |hello universe|

    // --------------------------------------------------------
  }
*/

/*
  #include <string_view>

  int main()
  {
    using namespace std;

    char str[] = "Quantum Computing";
    string_view sv1 = str;
    string_view sv2(str, 7);  // data constructor

    cout << "sv1.length() = " << sv1.length() << '\n';
    // output -> sv1.length() = 17
    cout << "sv2.length() = " << sv2.length() << '\n';
    // output -> sv2.length() = 7 

    cout << sv1 << '\n';
    // output -> Quantum Computing
    cout << sv2 << '\n';
    // output -> Quantum
  }
*/

/*
    // using "data" member function 

    #include <string_view>

    int main()
    {
      using namespace std;

      char str[] = "Quantum Computing";
      string_view sv(str, 7);

      cout << "|" << sv << "|\n";         
      // output -> |Quantum|
      // string_view parameter inserter overload of string_view

      cout << "|" << sv.data() << "|\n"; 
      // output -> |Quantum Computing|
      // C-style string parameter inserter overload of string_view

      // "data" member function will return a pointer 
      // to the first character of the string_view object
      // after the last character that string_view object points to
      // it will continue printing characters until it reaches to '\0'
      // not in this scenario but this might cause undefined behavior(ub)
    }
*/

/*
  #include <array>
  #include <string_view>
  #include <cstdio> // std::puts, std::printf

  int main()
  {
    using namespace std;

    array<char, 5> arr{ 'h', 'e', 'l', 'l', 'o' };
    string_view sv{ arr.data(), arr.size() };

    // --------------------------------------------------------

    cout << "sv.length() = " << sv.length() << '\n';  
    // output -> sv.length() = 5

    cout << "|" << sv << "|\n"; 
    // output -> |hello|

    cout << "|" << sv.data() << "|\n";  // undefined behaviour(UB)
    // C-style string parameter inserter overload of string_view
    // wants a NTBS but arr does not have a '\0' character

    // --------------------------------------------------------

    std::puts(sv.data());  // undefined behaviour(UB)
    // std::puts expects a NTBS

    std::printf("%s\n", sv.data());  // undefined behaviour(UB)
    // std::printf expects a NTBS

    // --------------------------------------------------------
  }
*/

/*
  #include <array>
  #include <string_view>
  #include <cstdio> // std::puts, std::printf

  int main()
  {
    using namespace std;

    array<char, 6> arr{ 'h', 'e', 'l', 'l', 'o' };
    // arr have '\0' character at the end

    string_view sv{ arr.data(), arr.size() };
  
    // --------------------------------------------------------

    cout << "sv.length() = " << sv.length() << '\n';  
    // output -> sv.length() = 6

    cout << "|" << sv << "|\n"; 
    // output -> |hello|

    cout << "|" << sv.data() << "|\n";  // VALID
    // output -> |hello|

    // --------------------------------------------------------

    std::puts(sv.data());             // VALID
    // output -> hello
    std::printf("%s\n", sv.data());   // VALID
    // output -> hello

    // --------------------------------------------------------
  }
*/

/*
  #include <string_view>

  int main()
  {
    // --------------------------------------------------------

    std::string_view sv = "hello\0\0\0world";

    std::cout << "sv.length() = " << sv.length() << '\n';
    // output -> sv.length() = 5

    // --------------------------------------------------------

    const char* p = "hello\0\0\0world";
    std::string_view sv2(p, 13);  // data constructor

    std::cout << "sv2.length() = " << sv2.length() << '\n';
    // output -> sv2.length() = 13
    
    std::cout << sv2 << '\n';   // output -> helloworld
    // string_view parameter inserter overload of string_view

    std::cout << sv2.data() << '\n';  // output -> hello
    // C-style string parameter inserter overload of string_view

    // --------------------------------------------------------
  }
*/

/*
  // there is no std::string parameter constructor of string_view
  // but std::string class have an operator type conversion function 
  // to string_view class

  #include <string>
  #include <string_view>

  int main()
  {
    std::string str{ "hello world" };

    auto sv = str.operator std::basic_string_view<
                                  char, std::char_traits<char>>();

    std::string_view sv2;
    sv2 = str;  
    // operator= type conversion function will be called for "str"

    std::string_view sv3{ str };
  }
*/

/*
  #include <string_view>

  int main()
  {
    char cities[] = "Istanbul Ankara";

    std::string_view sv{ cities + 9, cities + 15 };

    std::cout << "sv.length() = " << sv.length() << '\n';
    // output -> sv.length() = 7
    std::cout << "|" << sv << "|\n";
    // output -> |Ankara|
  }
*/

/*
  #include <string>
  #include <string_view>
  #include <vector>

  int main()
  {
    using namespace std;

    // --------------------------------------------------------

    string str{ "hello world" };

    string_view sv(str.begin(), str.end()); 
    // range parameter ctor(C++20)

    cout << "|" << sv << "|\n";
    // output -> |hello world|

    // --------------------------------------------------------

    vector<char> cvec{ 'h', 'e', 'l', 'l', 'o' };

    string_view sv2(cvec.begin(), cvec.end());

    cout << "|" << sv2 << "|\n";
    // output -> |hello|

    // --------------------------------------------------------
  }
*/

/*
  // nullptr parameter constructor is deleted

  #include <string_view>

  int main()
  {
    std::string_view sv = nullptr;  // syntax error
    // error: use of deleted function 
    // 'std::basic_string_view<_CharT, _Traits>::basic_string_view(
    //                                                std::nullptr_t)
  }
*/

/*
  #include <string_view>

  int main()
  {
    using namespace std;

    char city[] = "ISTANBUL";

    // --------------------------------------------------------

    string_view sv(city);

    cout << "|" << sv << "|\n";   
    // output -> |ISTANBUL|

    cout << "sv.front() = " << sv.front() << '\n';  
    // output -> sv.front() = I

    cout << "sv.back() = " << sv.back() << '\n';
    // output -> sv.back() = L

    // --------------------------------------------------------

    city[0] = '_';
    city[1] = 'A';
    city[2] = 'N';
    city[3] = 'K';
    city[4] = 'A';
    city[5] = 'R';
    city[6] = 'A';
    city[7] = '_';

    cout << "|" << sv << "|\n";
    // output -> |_ANKARA_|

    cout << "sv.front() = " << sv.front() << '\n';  
    // output -> sv.front() = _

    cout << "sv.back() = " << sv.back() << '\n';
    // output -> sv.back() = _
    
    // --------------------------------------------------------
  }
*/

/*
  // std::string class has an explicit 
  // std::string_view parameter constructor

  #include <string>
  #include <string_view>

  void func(std::string);

  std::string foo()
  {
    return std::string_view{ "hello galaxy" };  // syntax error
    // error: could not convert 
    // 'std::basic_string_view<char>(((const char*)"hello galaxy"))'  
    // from 'std::string_view' to 'std::string'
  }

  int main()
  {
    using namespace std;

    string_view sv{ "hello world" };

    string str{ sv };  
    // explicit std::string_view parameter constructor

    // --------------------------------------------------------

    string str2 = sv; // syntax error
    // error: conversion from 'std::string_view' 
    // to non-scalar type 'std::string' requested

    func(sv);  // syntax error
    // error: could not convert 'sv' 
    // from 'std::string_view' to 'std::string' 

    // --------------------------------------------------------
  }
*/

/*
  #include <string>
  #include <string_view>

  std::string foo()
  {
    return "Lorem ipsum dolor sit amet";
  }

  int main()
  {
    // --------------------------------------------------------

    auto str = foo();

    const std::string& clref_str = foo();   // life extension
    std::string&& rref_str = foo();         // life extension

    auto p = foo().c_str();         // undefined behaviour(UB)
    // pointer that c_str member function returns will become
    // dangling pointer after "foo" function scope ends

    // --------------------------------------------------------

    std::string_view sv = foo();    // undefined behaviour(UB)
    // pointers that string_view object holds will become 
    // dangling pointers after "foo" function scope ends

    // --------------------------------------------------------
  }
*/

/*
  #include <string>
  #include <string_view>

  class Person{
  public:
    Person(const std::string& name) : m_name{ name } {}
    void print() const { std::cout << m_name << '\n'; }

    std::string_view get_name() const { return m_name; }
    // get_name member function returns a string_view object
  private:
    std::string m_name{};
  };

  Person create_person()
  {
    return Person{ "Lorem Ipsum" };
  }

  int main()
  {
    auto sv = create_person().get_name();
    // string_view objects pointers will become a dangling pointers
    // when create_person function scope ends
    // temporary Person object's destructor will be called

    std::cout << sv << '\n';  // undefined behaviour(UB)
  }
*/

/*
  #include <string>
  #include <string_view>

  int main()
  {
    using namespace std;

    string str(10, 'A');
    string_view sv{ str };

    str.append(20, 'B');   // reallocation has been done.
    cout << str << '\n';    // output -> AAAAAAAAAABBBBBBBBBBBBBBBBBBBB

    cout << sv << '\n';   // undefined behaviour(UB)
    // string_view object's pointers will still point to the old string
    // but the string has been reallocated
    // string_view object's pointers will become dangling pointers
  }
*/

/*
  #include <string>
  #include <string_view>

  int main()
  {
    using namespace std;

    auto p = new string{ "istanbul ankara" };
    string_view sv{ *p };

    cout << sv << '\n'; // output -> istanbul ankara

    delete p;
    cout << sv << '\n'; // undefined behaviour(UB)
    // when p has been deleted(its memory has been deallocated)
    // string_view object's pointer will still points to the
    // address that p points to
    // string_view object's pointers will become dangling pointers
  }
*/

/*
  #include <string_view>
  #include <string>

  std::string operator+(std::string_view sv1, std::string_view sv2)
  {
    return std::string(sv1) + std::string(sv1);
  }

  template <typename T>
  T concat(const T& x, const T& y)
  {
    return x + y;
  }

  int main()
  {
    std::string_view sv = "hello";
    auto val = concat(sv, sv);
    // "concat" functions return type is string_view
    // "concat" function will return temp string_view object

    std::cout << val << '\n';   // undefined behaviour(UB)
    // val's pointers will point to the same addresses
    // that temp string_view objects pointers points to
    // but temp object will be destroyed after the function call
    // and "val" will become a dangling pointer
  }
*/

/*
    #include <string_view>
    #include <string>

    std::string operator+(std::string_view sv1, std::string_view sv2)
    {
      return std::string(sv1) + std::string(sv1);
    }

    template <typename T>
    auto concat(const T& x, const T& y)
    {
      return x + y;
    }

    int main()
    {
      std::string_view sv = "hello";
      auto val = concat(sv, sv);
      // concat function returns type is std::string

      std::cout << val << '\n';   // VALID
      // output -> hellohello
    }
*/

/*
  #include <string>
  #include <string_view>

  std::string_view foo(std::string str)
  {
    return str; 
    // automatic storage duration parameter variable
    // its life time will end when the function scope ends
  }
*/

/*
  // most of string_view classes member functions are constexpr
  // copy, hash and inserter functions are not constexpr 

  #include <string_view>

  int main()
  {
    constexpr std::string_view sv{ "hello world" };
    constexpr auto len = sv.length();
    constexpr auto c_start = sv.front();
    constexpr auto c_end = sv.back();

    constexpr auto iter_beg = sv.begin();
    constexpr auto iter_end = sv.end();

    constexpr auto idx = sv.find('w');
    constexpr auto idx_2 = sv.find_first_of("klmnp");
  }
*/

/*
  #include <string_view>

  int main()
  {
    using namespace std;

    string_view sv{ "hello world" };

    // --------------------------------------------------------

    cout << "|" << sv << "|\n";  
    // output -> |hello world|
    cout << "sv.size() = " << sv.size() << '\n';  
    // output -> sv.size() = 11
    cout << "sv.data() = " << sv.data() << '\n';
    // output -> sv.data() = hello world

    // --------------------------------------------------------

    sv.remove_prefix(3);

    cout << "|" << sv << "|\n";  
    // output -> |lo world|
    cout << "sv.size() = " << sv.size() << '\n';  
    // output -> sv.size() = 8
    cout << "sv.data() = " << sv.data() << '\n';
    // output -> sv.data() = lo world

    // --------------------------------------------------------

    sv.remove_suffix(4);

    cout << "|" << sv << "|\n";  
    // output -> |lo w|
    cout << "sv.size() = " << sv.size() << '\n';  
    // output -> sv.size() = 4
    cout << "sv.data() = " << sv.data() << '\n';
    // output -> sv.data() = lo world

    // sv.data() = lo world because of the NTBS

    // --------------------------------------------------------
  }
*/

/*
  // remove_prefix and remove_suffix are non-const member functions

  #include <string_view>
  #include <string>
  #include <algorithm>  // std::min

  int main()
  {
    std::string str{ "______________hello world" };
    std::string_view sv{ str };

    // --------------------------------------------------------

    std::cout << "(" << str << ")\n";
    // output -> (______________hello world)
    std::cout << "(" << sv << ")\n";
    // output -> (______________hello world)

    // --------------------------------------------------------

    sv.remove_prefix(std::min(sv.find_first_not_of('_'), sv.size()));

    // if the '_' character could not be found, find_first_not_of
    // will return string::npos(max value of size_t), 
    // and std::min will return sv.size()

    std::cout << "(" << str << ")\n";
    // output -> (______________hello world)
    std::cout << "(" << sv << ")\n";
    // output -> (hello world)

    // --------------------------------------------------------
  }
*/

/*
  #include <string>
  #include <string_view>

  // foo overloads
  void foo(std::string)
  {
    std::cout << "foo(std::string)\n";
  }

  void foo(std::string_view)
  {
    std::cout << "foo(std::string_view)\n";
  }

  // bar overloads
  void bar(const char*)
  {
    std::cout << "bar(const char*)\n";
  }

  void bar(std::string)
  {
    std::cout << "bar(std::string)\n";
  }

  void bar(std::string_view)
  {
    std::cout << "bar(std::string_view)\n";
  }

  int main()
  {
    // --------------------------------------------------------

    foo("hello world");   // syntax error
    // error: call of overloaded 'foo(const char [12])' is ambiguous
    // user-defined conversion : const char[12] ==> std::string 
    // user-defined conversion : const char[12] ==> std::string_view 

    using namespace std::literals;

    foo("hello world"s);  // VALID
    // output -> foo(std::string)

    foo("hello world"sv); // VALID
    // output -> foo(std::string_view)

    // --------------------------------------------------------

    bar("hello world");   // VALID
    // output -> bar(const char*)

    bar("hello world"s);  // VALID
    // output -> bar(std::string)

    bar("hello world"sv); // VALID
    // output -> bar(std::string_view)

    // --------------------------------------------------------
  }
*/

/*
  // <---- check class_ctor_overloads.png ---->

  class Myclass{
  public:
    Myclass(const std::string& str) : m_str{ str } {}
    Myclass(std::string_view sv) : m_str{ sv } {}
    Myclass(std::string str) : m_str{ std::move(str) } {}
  private:
    std::string m_str{};
  };
*/

/*
  // "start_with" and "end_with" constexpr member functions (C++20)
  // "contains" constexpr member function (C++23)

  #include <string_view>

  int main()
  {
    std::string_view sv{ "hello world" };

    bool b1 = sv.starts_with("hello");
    bool b2 = sv.ends_with("ld");     
    bool b3 = sv.contains("wor");

    std::cout << std::boolalpha;
    std::cout << b1 << '\n';  // output -> true
    std::cout << b2 << '\n';  // output -> true
    // --------------------------------------------------------
    std::cout << b3 << '\n';  // output -> true
  }
*/