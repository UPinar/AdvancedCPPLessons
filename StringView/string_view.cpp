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

// Lesson_7 - 01:40:30