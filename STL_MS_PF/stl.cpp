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

/*
  - STL container's move constructors are constant time O(1)
    complexity, but std::array's move constructor is linear
    time O(N) complexity.
*/

/*
  #include <vector>
  #include <array>

  int main()
  {
    std::array<double, 500> arr1;
    // when 500 element array will be moved, 
    // its elements will be copied one by one

    std::vector<double> vec1(500);
    // when 500 element vector will be moved,
    // only its data member pointers will be copied
    // (pointer that holds the address of the first element)
    // (pointer that holds the address of the end of the array)
  }
*/

/*
  #include <array>

  class Myclass{
  public:
    Myclass() = default;
    Myclass(const Myclass&) = default;
    Myclass& operator=(const Myclass&) = default;

    Myclass(Myclass&&)
    {
      std::cout << "Myclass(Myclass&&)\n";
    }

    Myclass& operator=(Myclass&&) = default;
  };

  int main()
  {
    std::array<Myclass, 5> arr1;

    auto arr2 = std::move(arr1);  
    // linear complexity O(N), elements will be moved one by one

    // output ->
    //  Myclass(Myclass&&)
    //  Myclass(Myclass&&)
    //  Myclass(Myclass&&)
    //  Myclass(Myclass&&)
    //  Myclass(Myclass&&)
  }
*/

/*
  - It is guaranteed that moved-from state std::vector's size is 0. 
*/

/*
              -------------------------------------------
              | std::remove , std::remove_if algorithms |
              -------------------------------------------
  - those algorithms cause move operations
  - those algorithms does not do erase operation, containers 
    member functions do erase operation.
  - those algorithms job is to move elements which will not be deleted
    to the beginning of the container.
*/

/*
  // std::remove and std::remove_if algorithms implementation

  #include <algorithm>  // std::find, std::find_if

  template <typename ForwardIt, class T>
  ForwardIt Remove(ForwardIt first, ForwardIt last, const T& value)
  {
    first = std::find(first, last, value);
    if (first != last)
      for (ForwardIt i = first; ++i != last;)
        if (!(*i == value))
          *first++ = std::move(*i);

    return first; // returns logic end position
  }

  template <typename ForwardIt, class UnaryPred>
  ForwardIt Remove_If(ForwardIt first, ForwardIt last, UnaryPred pred)
  {
    first = std::find_if(first, last, pred);
    if (first != last)
      for (ForwardIt i = first; ++i != last;)
        if (!pred(*i))
          *first++ = std::move(*i);

    return first; // returns logic end position
  }
*/

/*
  #include <algorithm>  // std::remove, std::remove_if
  #include <string>
  #include <vector>

  int main()
  {
    using namespace std;

    vector<string> svec{ "istanbul", "ankara", "izmir", "eskisehir",
                          "bursa", "antalya", "adana"};

    auto iter = std::remove_if(svec.begin(), svec.end(), 
            [](const auto& str){ return str.contains('i');}); // (C++23)
    // [](const auto& str){ return str.find('i') != std::string::npos;});

    // svec.erase(iter, svec.end());
    // elements that we do not want to delete 
    // are moved to the beginning of the vector
    // think about we have 7 elements in vector 
    // and 3 of them are we do not want to delete,
    // so iter will point to the 4th element of the vector
    // and we will start erasing from 4th element 
    // to the end of the vector
    

    int count{};
    while (iter != svec.end())
      cout << ++count << ' ' << *iter++ << '\n';

    // output ->
    //  1
    //  2 
    //  3 

    // because of last 3 elements before remove_if algorithm called
    // did not contain 'i' character
    // they have been moved to the beginning of the container
    // because of there are 3 elements which contains 'i' character
    // iter will point to the 4th(7 - 3) element of the vector
    // and because of the last 3 elements have been moved to the beginning
    // (they do not have 'i' character)
    // output is showing empty strings
  }
*/

/*
  #include <chrono>
  #include <string>

  class PersonClassic{
  public:
    PersonClassic(const std::string& name, const std::string& surname)
      : m_name{ name }, m_surname{ surname } {}
  private:
    std::string m_name;
    std::string m_surname;
  };

  class PersonInitMove{
  public:
    PersonInitMove(std::string name, std::string surname)
      : m_name{ std::move(name) }, m_surname{ std::move(surname) } {}
  private:
    std::string m_name;
    std::string m_surname;
  };

  class PersonInitOverload{
  public:
    PersonInitOverload(const std::string& name, const std::string& surname)
      : first{ name }, last{ surname } {}

    PersonInitOverload(const std::string& name, std::string&& surname)
      : first{ name }, last{ std::move(surname) } {}

    PersonInitOverload(std::string&& name, const std::string& surname)
      : first{ std::move(name) }, last{ surname } {}
    
    PersonInitOverload(std::string&& name, std::string&& surname)
      : first{ std::move(name) }, last{ std::move(surname) } {}

    PersonInitOverload(const char* name, const char* surname)
      : first{ name }, last{ surname } {}

    PersonInitOverload(const char* name, const std::string& surname)
      : first{ name }, last{ surname } {}
    
    PersonInitOverload(const char* name, std::string&& surname)
      : first{ name }, last{ std::move(surname) } {}

    PersonInitOverload(const std::string& name, const char* surname)
      : first{ name }, last{ surname } {}

    PersonInitOverload(std::string&& name, const char* surname)
      : first{ std::move(name) }, last{ surname } {}

  private:
    std::string first;
    std::string last;
  };

  using Person = PersonClassic;
  // using Person = PersonInitMove;
  // using Person = PersonInitOverload;


  const char* pname = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
  const char* psurname = "yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy";

  std::chrono::nanoseconds measure(int num)
  {
    std::chrono::nanoseconds total_duration{};

    for (int i = 0; i < num; ++i){
      std::string name(10000, 'N');
      std::string surname(10000, 'S');

      auto start = std::chrono::steady_clock::now();

      Person p1{ pname, psurname };
      Person p2{ name, surname };
      Person p3{ std::move(name), std::move(surname) };

      auto end = std::chrono::steady_clock::now();
      total_duration += end - start;
    }

    return total_duration;
  }

  constexpr int n = 100'000;

  int main()
  {
    measure(20);  // to not care starting cost 

    std::chrono::nanoseconds nanosecond_duration{ measure(n) };

    std::cout << "test results for " << n << " iterations "
              << nanosecond_duration.count() / n << "ns\n";


    // output PersonClassic ->
    // test results for 100000 iterations 1374ns

    // output PersonInitMove ->
    //  test results for 100000 iterations 1014ns

    // output PersonInitOverload ->
    //  test results for 100000 iterations 740ns
  }
*/

/*
  #include <utility> // std::move, std::forward

  template <typename T, typename Allocator = std::allocator<T>>
  class Vector{
  public:
    // place is a pointer to the next element to be constructed
    void push_back(const T& t) 
    {
      new(place)T(t);
    }
    void push_back(T&& t) 
    {
      new(place)T(std::move(t));
    }

    template <typenmae ...Args>
    constexpr T& emplace_back(Args&&... args)
    {
      new(place)T(std::forward<Args>(args)...);
    }  
    // emplace_back function can be used when a class does not have
    // a copy or move constructor
  };
*/

/*
          -----------------------------------------------
          | std::move and std::move_backward algorithms |
          -----------------------------------------------
*/

/*
  // what std::copy_backward algorithm does 
  // (aynı range içinde kaydırma işlemi)

  #include <vector>
  #include <algorithm>  // std::copy_backward
  #include <numeric>    // std::iota
  #include "../nutility.h"

  int main()
  {
    std::vector<int> ivec(10);

    std::iota(ivec.begin(), ivec.end(), 0);
    print(ivec);  // output -> 0 1 2 3 4 5 6 7 8 9

    // 0 1 2 3 4 5 6 7 8 9
    //     0 1 2 3 4 
    // I want first 5 elements to be moved 
    // better use std::copy_bacward algorithm in this scenario

    std::copy_backward( ivec.begin(), 
                        ivec.begin() + 5, 
                        ivec.begin() + 7);

    print(ivec);  // output -> 0 1 0 1 2 3 4 7 8 9
  }
*/


/*
  // std::copy, std::copy_backward, std::move, std::move_backward 
  // algorithms implementations

  #include <utility>    // std::move

  template <typename InIt, typename OutIt>
  OutIt Copy(InIt first, InIt last, OutIt dest_first)
  {
    while (first != last)
      *dest_first++ = *first++;

    return dest_first;
  }

  template <typename BidIt1, typename BidIt2>
  BidIt2 CopyBackward(BidIt1 first, BidIt1 last, BidIt2 dest_last)
  {
    while (first != last)
      *--dest_last = *--last;

    return dest_last;
  }

  template <typename InIt, typename OutIt>
  OutIt Move(InIt first, InIt last, OutIt dest_first)
  {
    while (first != last)
      *dest_first++ = std::move(*first++);

    return dest_first;
  }

  template <typename BidIt1, typename BidIt2>
  BidIt2 MoveBackward(BidIt1 first, BidIt1 last, BidIt2 dest_last)
  {
    while (first != last)
      *--dest_last = std::move(*--last);

    return dest_last;
  }
*/

/*
  #include <vector>
  #include <string>
  #include <algorithm>  // std::copy, std::move

  int main()
  {
    using namespace std;

    vector<string> svec{ "istanbul", "ankara", "izmir", "eskisehir" };
    vector<string> destvec(svec.size());
    vector<string> destvec2(svec.size());

    // -------------------------------------------------------

    std::copy(svec.begin(), svec.end(), destvec.begin());  // std::copy

    cout << "svec.size() = " << svec.size() << '\n';
    // output -> svec.size() = 4

    for(int cnt{}; const auto& str : svec)
      cout << ++cnt << ' ' << str << '\n';  
    
    // output ->
    //  1 istanbul
    //  2 ankara
    //  3 izmir
    //  4 eskisehir

    // -------------------------------------------------------

    std::move(svec.begin(), svec.end(), destvec2.begin());  // std::move

    cout << "svec.size() = " << svec.size() << '\n';
    // output -> svec.size() = 4

    for(int cnt{}; const auto& str : svec)
      cout << ++cnt << ' ' << str << '\n';

    // output ->
    //  1
    //  2
    //  3
    //  4
    // elements become moved-from state

    // -------------------------------------------------------
  }
*/

/*
            -----------------------------------------
            | `std::move_iterator` iterator adaptor |
            -----------------------------------------
*/

/*
  // ways to create an iterator adaptor

  // -------------------- SCENARIO_1 --------------------

  template <typename Iter>
  class AdaptorIterator{
  private:
    Iter m_iter;
  };

  // -------------------- SCENARIO_2 --------------------

  template <typename Iter>
  class AdaptorIterator_2 : public Iter{ 

  };
*/

/*
  // std::move_iterator is a class template (iterator adaptor)

  #include <iterator> // std::move_iterator
  #include <vector>
  #include <string>


  template <typename Iter>
  typename std::move_iterator<Iter> MakeMoveIterator(Iter it)
  {
    return std::move_iterator<Iter>(it);
  }

  int main()
  {
    std::move_iterator<std::vector<int>::iterator> iter;
    // iter is adapting `std::vector<int>::iterator` type


    using namespace std;
    vector<string> svec{ "istanbul", "ankara", "izmir", "eskisehir" };

    // -------------------------------------------------------

    auto regular_iter = svec.begin();
    // "regular_iter" type is `std::vector<string>::iterator`

    // -------------------------------------------------------

    move_iterator<vector<string>::iterator> iter_1{ svec.begin() };
    move_iterator iter_2{ svec.begin() }; // CTAD(C++17)
    auto iter_3 = make_move_iterator(svec.begin()); 

    // iter_1, iter_2 and iter_3 types are same 
    // `std::move_iterator<std::vector<string>::iterator>`
    
    // -------------------------------------------------------

    auto iter_x = svec.begin();
    auto city = *iter_x; 

    cout << svec[0] << " ==> size = " <<svec[0].size() << '\n';
    // output -> istanbul ==> size = 8

    auto miter_x = make_move_iterator(svec.begin());
    city = *miter_x; 
    // "*miter_x" is an RValue expression
    // dereferencing a move iterator generates an RValue expression

    cout << svec[0] << " ==> size = " <<svec[0].size() << '\n';
    // output -> "" ==> size = 0

    // -------------------------------------------------------
  }
*/

/*
  - dereferencing move iterator for the second time means,
    dereferencing a moved-from state object
*/

/*
  #include <vector>
  #include <string>

  int main()
  {
    using namespace std;

    vector<string> svec{ "istanbul", "ankara", "izmir", "eskisehir" };

    // moving elements of svec for constructing a new vector
    // svec's elements will become moved-from state objects

    // -------------------------------------------------------

    vector<string> copy_vec{ svec.begin(), svec.end() };

    for (int cnt{}; const auto& str : svec)
      cout << ++cnt << ' ' << str << " ==> " << str.length() <<'\n';
    // output ->
    //  1 istanbul ==> 8
    //  2 ankara ==> 6
    //  3 izmir ==> 5
    //  4 eskisehir ==> 9

    // -------------------------------------------------------

    vector<string> move_vec{ move_iterator(svec.begin()), 
                            move_iterator(svec.end()) };

    for (int cnt{}; const auto& str : svec)
      cout << ++cnt << ' ' << str << " ==> " << str.length() <<'\n';
    // output ->
    //  1  ==> 0
    //  2  ==> 0
    //  3  ==> 0
    //  4  ==> 0

    // -------------------------------------------------------
  }
*/

/*
  // ------------------------- PROBLEM -------------------------
  // moving names contains 'i' character to the callable function
  // ------------------------- PROBLEM -------------------------

  #include <vector>
  #include <string>
  #include <algorithm>  // std::for_each
  #include "../nutility.h"

  void callable_function(std::string name)
  {
    std::cout << "name = " << name << '\n';
  }
  
  int main()
  {
    using namespace std;

    vector<string> svec;
    rfill(svec, 5, rname);
    print(svec);
    // output -> ceyda alparslan sezen hasan niyazi


    // we are moving names(strings) to the parameter variable
    // of the lambda function (*move_iterator)
    std::for_each(make_move_iterator(svec.begin()), 
                  make_move_iterator(svec.end()), 
                  [](string str){   // -------> moving
                    if (str.contains('i'))
                      callable_function(str); 
                  });
    // output -> name = niyazi

    for (int cnt{}; const auto& name : svec)
      cout << cnt++ << ' ' << name << '\n';
    // output ->
    //  0
    //  1
    //  2
    //  3
    //  4

    // all names are in moved-from state
  }
*/

/*
  // ------------------------- SOLUTION -------------------------
  // moving names contains 'i' character to the callable function
  // ------------------------- SOLUTION -------------------------

  #include <vector>
  #include <string>
  #include <algorithm>  // std::for_each
  #include "../nutility.h"

  void callable_function(std::string name)
  {
    std::cout << "name = " << name << '\n';
  }

  // moving names contains 'i' character to the callable function

  int main()
  {
    using namespace std;

    vector<string> svec;
    rfill(svec, 5, rname);
    print(svec);
    // output -> atakan pakize pelinsu emrecan hilal


    // because of "str" inside lambda function is an LValue expression
    // move semantics needs to be applied.
    // "std::move(str)" expression is an XValue(R) expression

    std::for_each(make_move_iterator(svec.begin()), 
                  make_move_iterator(svec.end()), 
                  [](string&& str){
                    if (str.contains('i'))
                      callable_function(std::move(str)); 
                  });
    // output ->
    //  name = pakize
    //  name = pelinsu
    //  name = hilal

    for (int cnt{}; const auto& name : svec)
      cout << cnt++ << ' ' << name << '\n';
    // output ->
    //  0 atakan
    //  1
    //  2
    //  3 emrecan
    //  4

    // names which contains 'i' character have been moved
    // to the callable function
  }
*/