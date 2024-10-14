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
*/

// Lesson_10 : 03:00:00




