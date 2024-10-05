#include <iostream>

/*
          ------------------------------------------------
          | Move Semantics and Perfect Forwarding in STL |
          ------------------------------------------------
*/

/*
  // how to choose parameter type in functions

  #include <string>
  #include <algorithm>  // std::reverse

  // will use std::string class for read-only operations
  // but inside the function, we need to copy the string
  void foo(const std::string& str)
  {
    auto str_temp = str; // copied the string

    reverse(str_temp.begin(), str_temp.end());
    // not using qualified name(std::) for reverse function
    // because ADL(argument dependent lookup) is being used.
    // "begin" and "end" member function's types are 
    // std::string::iterator  
    // so "reverse" identifier will be searched in std namespace
  }

  // better making parameter variable as non-reference type
  void foo(std::string str)
  {
    reverse(str.begin(), str.end());
  }

  // ADVANTAGE: when PRValue expression is passed to 
  //  non-reference type parameter as an argument, 
  //  mandatory copy ellision will happen.
*/

/*
                      -------------------
                      | move-only types |
                      -------------------
*/

/*
  // move-only types

  class MoveOnly{
  public:
    MoveOnly() = default;

    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;

    MoveOnly(MoveOnly&&) = default;
    MoveOnly& operator=(MoveOnly&&) = default;
  };
  // when one of move members are user declared,
  // copy members will implicitly declared deleted.
  // but it can also be explicitly declared deleted

  // move members can be, 
  //  - user declared defaulted
  //  - user declared to be defined.


  void func(MoveOnly);

  int main()
  {
    MoveOnly mx;

    func(mx); // syntax error
    // "mx" is an LValue expression, so it will be copied

    func(MoveOnly{});
    // MoveOnly{} is a PRValue expression
    // mandatory copy ellision will happen

    func(std::move(mx));
    // std::move(mx) is an XValue expression
    // "mx" has been move to parameter variable 
  }
*/

/*
  // initializer list ctor and move-only types

  #include <vector>

  class MoveOnly{
  public:
    MoveOnly() = default;

    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;

    MoveOnly(MoveOnly&&) = default;
    MoveOnly& operator=(MoveOnly&&) = default;
  };

  int main()
  {
    using namespace std;

    vector<MoveOnly> mvec{ MoveOnly{}, MoveOnly{} };  // syntax error
    // error: use of deleted function 
    // 'MoveOnly::MoveOnly(const MoveOnly&)'

    // when initializer_list ctor is used, temp objects 
    // needs to be copied but MoveOnly classses copy members
    // are deleted.
  }
*/

/*
  // range-based for loop and move-only types

  #include <vector>

  class MoveOnly{
  public:
    MoveOnly() = default;

    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;

    MoveOnly(MoveOnly&&) = default;
    MoveOnly& operator=(MoveOnly&&) = default;
  };

  int main()
  {
    using namespace std;

    vector<MoveOnly> mvec(100);

    for (auto mx : mvec)
    { 
      // syntax error
      // error: use of deleted function 
      // 'MoveOnly::MoveOnly(const MoveOnly&)'
    }
  }
*/

/*
  // function's return type and move-only types

  #include <utility>  // std::move

  class MoveOnly{
  public:
    MoveOnly() = default;

    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;

    MoveOnly(MoveOnly&&) = default;
    MoveOnly& operator=(MoveOnly&&) = default;
  };

  MoveOnly get_moveonly_1()
  {
    MoveOnly mx;
    return mx;  
    // L value to X value(R) conversion
    // move semantics will be applied
  }

  MoveOnly get_moveonly_2()
  {
    return MoveOnly{};  
    // "MoveOnly{}" is a PRValue expression
    // mandatory copy ellision will happen
  }

  MoveOnly global_mx;

  MoveOnly get_moveonly_3()
  {
    return std::move(global_mx);
    // because of "global_mx" is static storage duration object
    // LValue to XValue conversion will NOT be applied
    // for activate move semantics std::move is used
  }
*/

/*
  // putting move-only types into containers

  #include <vector>
  #include <utility>  // std::move

  class MoveOnly{
  public:
    MoveOnly() = default;
    MoveOnly(int, int) {}

    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;

    MoveOnly(MoveOnly&&) = default;
    MoveOnly& operator=(MoveOnly&&) = default;
  };

  int main()
  {
    std::vector<MoveOnly> mvec;

    MoveOnly mx;
    mvec.push_back(mx); // syntax error
    // error: use of deleted function 
    // 'MoveOnly::MoveOnly(const MoveOnly&)'

    mvec.push_back(std::move(mx));  // VALID
    mvec.emplace_back(11, 22);      // VALID
  }
*/

/*
  #include <utility>  // std::move
  #include <type_traits>
  // is_default_constructible_v, is_destructible_v
  // is_copy_constructible_v, is_copy_assignable_v
  // is_move_constructible_v, is_move_assignable_v

  template <typename T>
  void print_traits()
  {
    if constexpr(std::is_default_constructible_v<T>)
      std::cout << "T is default constructible\n";
    else
      std::cout << "T is NOT default constructible\n";

    if constexpr(std::is_destructible_v<T>)
      std::cout << "T is destructible\n";
    else
      std::cout << "T is NOT destructible\n";

    if constexpr(std::is_copy_constructible_v<T>)
      std::cout << "T is copy constructible\n";
    else
      std::cout << "T is NOT copy constructible\n";

    if constexpr(std::is_copy_assignable_v<T>)
      std::cout << "T is copy assignable\n";
    else
      std::cout << "T is NOT copy assignable\n";

    if constexpr(std::is_move_constructible_v<T>)
      std::cout << "T is move constructible\n";
    else
      std::cout << "T is NOT move constructible\n";

    if constexpr(std::is_move_assignable_v<T>)
      std::cout << "T is move assignable\n";
    else
      std::cout << "T is NOT move assignable\n";

    std::cout << '\n';
  }

  class MoveOnly{
  public:
    MoveOnly() = default;
    MoveOnly(int, int) {}

    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;

    MoveOnly(MoveOnly&&) = default;
    MoveOnly& operator=(MoveOnly&&) = default;
  };

  class Myclass{
  private:
    MoveOnly m_x;
  };
  // Myclass's move members become implicitly declared deleted

  int main()
  {
    print_traits<Myclass>();
    // output ->
    //  T is default constructible
    //  T is destructible
    //  T is NOT copy constructible
    //  T is NOT copy assignable
    //  T is move constructible
    //  T is move assignable
  }
*/

/*
  // std::ofstream class is a move-only type

  #include <string>
  #include <fstream>

  std::ofstream create_file(const std::string& filename)
  {
    std::ofstream ofs{ filename };

    if (!ofs)
      throw std::runtime_error{ filename + "cannot created!"};

    return ofs;
    // returning automatic storage duration object
    // LValue to XValue conversion
    // move semantics will be applied
  }

  void write_to_file(std::ofstream ofs)
  {
    ofs << "Hello, World!\n";
  }

  int main()
  {
    // ----------------------------------------------

    auto ofs = create_file("notes.txt");

    // ----------------------------------------------

    write_to_file(ofs); // syntax error
    // error: use of deleted function 
    // 'std::basic_ofstream<_CharT, _Traits>::basic_ofstream(
    //          const std::basic_ofstream<_CharT, _Traits>&)

    write_to_file(std::move(ofs));  // VALID
    // move semantics will be applied

    // ----------------------------------------------
  }
*/

/*
                ------------------------------
                | std::initilizer_list class |
                ------------------------------
*/

/*
  // std::initializer_list class reminder

  #include <initializer_list>

  void func(std::initializer_list<int> param)
  {
    std::cout << "&param           = " << &param << '\n';
    std::cout << "address of array = " << param.begin() << '\n';
  }

  void foo(int a, int b, int c)
  {
    for (auto val : { a, b, c })
      std::cout << val << ' ';
    std::cout << '\n';
  }

  void bar(std::initializer_list<int> param)
  {
    for (auto val : param)
      std::cout << val << ' ';
    std::cout << '\n';
  }

  int main()
  {
    // -------------------------------------------------------

    std::initializer_list<int> my_list = { 1, 2, 3, 4, 5 };
    // compiler will generate an array of integers
    // integers inside initializer_list will be COPIED to array

    // -------------------------------------------------------

    // initializer_list object holds only 2 pointers
    // first pointer holds the address of the first element
    // second pointer holds the address of the end of the array

    std::cout << "sizeof(my_list) = " << sizeof(my_list) << '\n';
    // output -> sizeof(my_list) = 16(2 * 8byte)

    // -------------------------------------------------------

    std::cout << "&my_list         = " << &my_list << '\n';
    std::cout << "address of array = " << my_list.begin() << '\n';
    // output ->
    //  &my_list         = 0xfa64dff940
    //  address of array = 0x7ff6532250b0

    func(my_list);  
    // initializer_list object will be copied to parameter variable
    // output ->
    //  &param           = 0xfa64dff8e0
    //  address of array = 0x7ff6532250b0


    // &my_list and &param will be different 
    // but param.begin() and my_list.begin() will be same
    // (copied data member pointers)

    // -------------------------------------------------------

    int x = 10, y = 20, z = 30;

    foo(x, y, z);       // output -> 10 20 30
    // parameter variables used as an initializer_list

    bar( { x, y, z} );  // output -> 10 20 30
    // parameter variables sent as an initializer_list

    // -------------------------------------------------------
  }
*/

/*
  // auto type deduction and template argument deduction 
  // aynı kurallara tabi  
  // Aradaki uyumu bozan initializer_list.

  template <typename T>
  void func(T);

  int main()
  {
    auto x = { 1, 3, 5, 7, 9 };
    // x's type is std::initializer_list<int>

    func({ 1, 3, 5, 7, 9 });  // syntax error
    // error: no matching function for call to 
    // 'func(<brace-enclosed initializer list>)'
  }
*/

/*
  #include <vector>

  #define asize(x)  (sizeof(x) / sizeof(x[0]))

  int main()
  {
    using namespace std;
    // --------------------------------------------------------

    const int a[] = { 1, 2, 3, 4, 5 };
    vector<int> ivec(a, a + asize(a));
    // -- before initializer_list
    // std::vector's range constructor is being used.

    vector<int> ivec2 = { 1, 2, 3, 4, 5 };
    // -- after initializer_list
    // std::vector's initializer_list constructor is being used.

    // --------------------------------------------------------

    // container's insert functions also have initializer_list 
    // parameter overloads

    // --------------------------------------------------------

    // container's operator= function's also have initializer_list
    // parameter overloads

    ivec = { 11, 12, 13, 14, 15 };

    // --------------------------------------------------------
  }
*/

/*
  #include <initializer_list>
  #include <string>
  #include <vector>

  class Myclass{
  public:
    Myclass(int)
    {
      std::cout << "Myclass(int)\n";
    }

    Myclass(std::initializer_list<int>)
    {
      std::cout << "Myclass(std::initializer_list<int>)\n";
    }
  };

  // default member initializer
  class ClassA{
  private:
    std::vector<int> ivec(10, 5); // syntax error
    // error: expected identifier before numeric constant

    std::vector<int> ivec{10, 5}; // VALID 
    //  initializer_list ctor will be called
  }

  int main()
  {
    // -------------------------------------------------------

    Myclass mx_1{ 10 };   // direct list initialization
    // output -> Myclass(std::initializer_list<int>)

    Myclass mx_2(10);    // direct initialization
    // output -> Myclass(int)

    // -------------------------------------------------------

    std::string str_1(48, 'X'); 
    // (size_t, char) ctor
    std::cout << str_1 << '\n'; 
    // output -> XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    std::string str_2{48, 'X'}; 
    // (std::initializer_list<char>) ctor
    std::cout << str_2 << '\n'; 
    // output -> 0X

    // -------------------------------------------------------

    std::vector<int> vec_1(10, 5);  
    // (size_t, int) ctor
    std::cout << vec_1.size() << '\n';  
    // output -> 10

    std::vector<int> vec_2{10, 5};  
    // (std::initializer_list<int>) ctor
    std::cout << vec_2.size() << '\n';  
    // output -> 2

    // -------------------------------------------------------
  }
*/

/*
  #include <string>
  #include <sstream>  // std::istringstream, std::ostringstream
  #include <fstream>  // std::ofstream

  int main()
  {
    using namespace std;

    string str{ "Istanbul\nAnkara\nIzmir\nEskisehir\nBursa\n" };

    // -------------------------------------------------------

    istringstream iss{ str };
    string city;

    getline(iss, city);
    cout << "|" << city << "|\n";   // output -> |Istanbul|

    iss >> city;
    cout << "|" << city << "|\n";   // output -> |Ankara|

    // -------------------------------------------------------

    getline(istringstream{ str }, city);
    cout << "|" << city << "|\n";   // output -> |Istanbul|

    // -------------------------------------------------------

    int x = 345;
    string planet{ "Mars" };
    double dval = 3.14;

    auto os = (ostringstream{} << x << planet << dval).str();
    cout << "os = " << os << '\n';
    // output -> os = 345Mars3.14

    // -------------------------------------------------------

    ofstream{ "notes.txt" } << "hello world";
    // notes.txt output -> hello world

    // -------------------------------------------------------
  }
*/

// Lesson-6 01:17:00