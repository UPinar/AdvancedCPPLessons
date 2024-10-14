#include <iostream>

/*
                    ======================
                    | Structured Binding |
                    ======================
*/

/*
  structured binding can be used 
    - for an array
    - for a type that data members are public
    - for a tuple like class (gave tuple interface)
      -> supporting tuple_size, get, tuple_element functions.
        - std::array
        - std::tuple
        - std::pair
*/

/*
  #include <utility>
  #include <string>

  struct Myclass{
    int m_ival;
    double m_dval;
    std::string m_str{ "hello" };
  };

  std::pair<int, double> foo();

  int main()
  {
    // ------------------------------------------------

    auto [a, b] = foo();

    // ------------------------------------------------

    int arr[3]{ 1, 2, 3 };
    auto [x, y, z] = arr;

    // ------------------------------------------------

    Myclass mx;
    auto [ival, dval, str] = mx;

    // ------------------------------------------------
  }
*/

/*
  auto[a, b] = var;
  auto[a, b](var);
  auto[a, b]{var};

  auto&[a, b] = var;
  auto&[a, b](var);
  auto&[a, b]{var};

  auto&&[a, b] = var;
  auto&&[a, b](var);
  auto&&[a, b]{var};

  -> can be `const` or `volatile` qualified
*/

/*
  #include <type_traits>

  struct Myclass{
    double m_dval{};
    int m_arr[5]{};
  };

  int main()
  {
    Myclass mx;

    // -----------------------------------------------------
    
    auto[d, x] = mx;
    // "d"'s type is double
    // "x"'s type is int[5]

    std::is_same_v<decltype(x), int*>;    // false
    std::is_same_v<decltype(x), int[5]>;  // true

    // -----------------------------------------------------

    // - çıkarım eşitliğin sağ tarafındaki nesne için yapılır.
    // - deduction is done for the object 
    //  on the right side of the equality.

    // -----------------------------------------------------

    // Myclass __abc = mx;
    // d ==> __abc.m_dval
    //    d is a type alias for __abc.m_dval
    // x ==> __abc.m_arr
    //    x is a type alias for __abc.m_arr

    // -----------------------------------------------------
  }
*/

/*
  int main()
  {
    int arr[3]{ 10, 20, 30 };

    // -----------------------------------------------------
    //            <---- check sb_for_arrays1.png ---->
    // -----------------------------------------------------

    auto [e1, e2, e3]{ arr };

    std::cout << "e1 = " << e1 << "\n";   // output -> e1 = 10
    std::cout << "e2 = " << e2 << "\n";   // output -> e2 = 20
    std::cout << "e3 = " << e3 << "\n";   // output -> e3 = 30

    // -----------------------------------------------------
    //            <---- check sb_for_arrays2.png ---->
    // -----------------------------------------------------

    auto& [r1, r2, r3]{ arr };

    ++r1;
    ++r2;
    ++r3;

    std::cout << "arr[0] = " << arr[0] << "\n";   
    // output -> arr[0] = 11
    std::cout << "arr[1] = " << arr[1] << "\n";   
    // output -> arr[1] = 21
    std::cout << "arr[2] = " << arr[2] << "\n";   
    // output -> arr[2] = 31

    arr[0] = 3;
    arr[1] = 4;
    arr[2] = 5;

    std::cout << "r1 = " << r1 << "\n";   // output -> r1 = 3
    std::cout << "r2 = " << r2 << "\n";   // output -> r2 = 4
    std::cout << "r3 = " << r3 << "\n";   // output -> r3 = 5

    // -----------------------------------------------------
    //          <---- check sb_for_arrays3.png ---->
    // -----------------------------------------------------
  }
*/

/*
  auto get_array_1() -> int(&)[3]
  {
    static int arr[3]{ 1, 2, 3 };
    return arr;
  }

  auto& get_array_2() 
  {
    static int arr_2[3]{ 1, 2, 3 };
    return arr_2;
  }

  int(&get_array_3())[3]
  {
    static int arr_3[3]{ 1, 2, 3 };
    return arr_3;
  }

  using arr3_t = int[3];

  arr3_t& get_array_4()
  {
    static int arr_4[3]{ 1, 2, 3 };
    return arr_4;
  }

  // C syntax
  int(*get_array_5())[3]
  {
    static int arr_5[3]{ 1, 2, 3 };
    return &arr_5;
  }

  int main()
  {
    auto [a, b, c] = get_array_1();
    auto [x, y, z] = get_array_2();
    auto [m, n, o] = get_array_3();
    auto [p, q, r] = get_array_4();
    auto [s, t, u] = *get_array_5();
  }
*/

/*
  class AClass{
    int m_a{ 11 };
    int m_b{ 22 };
    int m_c{ 33 };
    friend void foo();
  };

  void foo()
  {
    auto [x, y, z] = AClass{}; // VALID
  }

  int main()
  {
    auto [x, y, z] = AClass{}; // syntax error
    // error: cannot decompose inaccessible member 
    // 'AClass::m_a' of 'AClass'
  }
*/

/*
  #include <string>

  class Myclass{
  public:
    std::string m_str1{ "hello" };
    std::string m_str2{ "world" };
  };

  int main()
  {
    Myclass mx;

    // -----------------------------------------------------

    auto [str1, str2] = mx;             // copy semantics

    std::cout << mx.m_str1.length() << "\n";  // output -> 5
    std::cout << mx.m_str2.length() << "\n";  // output -> 5

    // -----------------------------------------------------

    auto [str3, str4] = std::move(mx);  // move semantics

    std::cout << mx.m_str1.length() << "\n";  // output -> 0
    std::cout << mx.m_str2.length() << "\n";  // output -> 0

    // -----------------------------------------------------
  }
*/

/*
  #include <string>

  class Myclass {
  public:
    std::string m_str1{ "hello" };
    std::string m_str2{ "world" };
  };

  int main()
  {
    Myclass mx;

    auto [str1] = mx; // syntax error
    // error: only 1 name provided for structured binding
    // note: while 'Myclass' decomposes into 2 elements
  }
*/

/*
  struct Point {
    int m_x, m_y, m_z;
  };

  int main()
  {
    Point p1{ 1, 2, 3 };
    Point p2{ 4, 5, 6 };

    auto [a, b, _] = p1; // VALID
    // "_" is been used as an identifier.

    auto [k, __, ___] = p2; // VALID
    // "__" and "___" are been used as identifiers.
  }
*/

/*
  #include <utility>  // std::pair
  #include <string>

  std::pair<std::string, std::string> foo();

  int main()
  {
    using namespace std;

    string str1;
    string str2;
    // str1 and str2 objects default ctor is called.

    auto ps = foo();

    str1 = ps.first;
    str2 = ps.second;
    // str1 and str2 objects copy assignment operator is called.
  }
*/

/*
  #include <tuple>  // std::tie

  int main()
  {
    using namespace std;

    // -----------------------------------------------------

    tuple<int, double, string> tp1{ 11, 3.14, "hello" };
    tuple<int, double, string> tp2;

    tp2 = tp1;  // VALID

    // -----------------------------------------------------

    // we can pass reference type to std::tuple and std::pair's
    // template argument.

    int ival, ival_2;
    double dval, dval_2;
    string str, str_2;  

    tuple<int&, double&, string&>{ ival, dval, str } = tp1;
    std::tie(ival_2, dval_2, str_2) = tp1;
    // Those 2 lines are equivalent.

    // we basically seperate the tuple's elements to identifiers
    // (ival, dval, str). Old way of structured binding.

    // -----------------------------------------------------
  }
*/

/*
  #include <utility>  // std::pair
  #include <tuple>    // std::tie

  std::pair<int, double> foo();


  int main()
  {
    int ival;
    double dval;

    std::tie(ival, dval) = foo();
  }
*/

/*
  // set container's insert member function

  #include <set>

  int main()
  {
    std::set<int> myset{ 11, 22, 33, 44, 55 };

    auto [iter, success] = myset.insert(66);
    // insert member function returns a pair<iterator, bool>
    // if insertion is successful, bool is true and
    // iterator points to the inserted element.

    // if insertion is not successful, bool is false and
    // iterator points to the element that is equal to the
    // element that we want to insert.

    if (success)
      std::cout << "insertion successful\n";
    else
      std::cout << "insertion failed\n";
    // output -> insertion successful
  }
*/

/*
  // structured binding can be used inside if with initializer syntax

  #include <set>

  int main()
  {
    std::set<int> myset{ 11, 22, 33, 44, 55 };

    if (auto [iter, success] = myset.insert(66); success)
      std::cout << "insertion successful\n";
    else
      std::cout << "insertion failed\n";

    // "iter" and "success" identifiers scope is limited to if block.
  }
*/

/*
  // minmax_element algorithm

  #include <vector>
  #include <string>
  #include <algorithm>  // std::minmax_element

  #include "../nutility.h"

  int main()
  {
    using namespace std;

    vector<string> svec;
    rfill(svec, 20, rname);

    auto [iter_min, iter_max] = minmax_element(svec.begin(), svec.end());
    // returns min and max elements' iterators 
    // as pair<iterator, iterator>
  }
*/



