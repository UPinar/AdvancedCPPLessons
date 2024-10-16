#include <iostream>

/*
          ============================================
          | Three-way comparison(spaceship) operator |
          ============================================
*/

/*
  --------------------------------------------------------

  comparison operators
    - equality operators    (==) (!=)
    - relational operators  (<) (>) (<=) (>=)

  --------------------------------------------------------

  all comparison operators generates bool value

  --------------------------------------------------------

  (a != b)  ==>   !(a == b)
  (a > b)   ==>   (b < a)
  (a >= b)  ==>   !(a < b)
  (a <= b)  ==>   !(b < a)

  --------------------------------------------------------

  a.operator()<(5)   

  (a < 5) is valid but (5 < a) is not valid
  because 5 is not a class object

  -> hidden friend function OR
  -> global operator function are used to solve this problem

  --------------------------------------------------------
*/

/*
  #include <cmath>  // NAN

  int main()
  {
    using namespace std;
    boolalpha(cout);
    
    // --------------------------------------------------------

    double dval { NAN };
    cout << (dval == dval) << '\n';  // output -> false

    // --------------------------------------------------------

    double d1 { NAN };
    double d2 { 4.56 };

    cout << "d1 < d2  = " << (d1 < d2) << '\n';   // output -> false
    cout << "d1 > d2  = " << (d1 > d2) << '\n';   // output -> false
    cout << "d1 <= d2 = " << (d1 <= d2) << '\n';  // output -> false
    cout << "d1 >= d2 = " << (d1 >= d2) << '\n';  // output -> false
    cout << "d1 == d2 = " << (d1 == d2) << '\n';  // output -> false
    cout << "d1 != d2 = " << (d1 != d2) << '\n';  // output -> true

    // --------------------------------------------------------
  }
*/

/*
  strcmp is a three-way comparison function

  ret = strcmp(str1, str2);

  ret > 0 ==>  str1 > str2
  ret < 0 ==>  str1 < str2
  ret = 0 ==>  str1 == str2
*/

/*
  #include <string>

  int main()
  {
    using namespace std;

    string s1{ "abc" };
    string s2{ "def" };
    string s3{ "xyz" };

    if (s2 <=> s1 < 0)
      cout << "(s2 < s1)\n";
    else
      cout << "(s2 > s1)\n";
    // output -> (s2 > s1)

    if (s2 <=> s3 < 0)
      cout << "(s2 < s3)\n";
    else
      cout << "(s2 > s3)\n";
    // output -> (s2 < s3)
  }
*/

/*
  #include <string>

  template <typename T>
  void T_print()
  {
    std::cout << typeid(T{} <=> T{}).name() << '\n';
  }

  int main()
  {
    T_print<int>(); 
    // output -> struct std::strong_ordering

    T_print<std::string>(); 
    // output -> struct std::strong_ordering

    T_print<double>();
    // output -> struct std::partial_ordering
  }
*/

/*
  --------------------------------------------------------
  |                   Equality        Ordering           |
  --------------------------------------------------------
  | Primary           (==)            (<=>)              |  
  --------------------------------------------------------
  | Secondary         (!=)            (<) (>) (<=) (>=)  |
  --------------------------------------------------------

  - primary operators are reversable.
    compiler will reverse the operands in the namelookup phase
    compiler can reverse (a == b) to (b == a)
  
  - secondary operators are rewritable
    compiler can rewrite expressions that contain secondary operators
    to expressions that contain primary operators

    (a != b)  ==>   !(a == b)
    (a > b)   ==>   (a <=> b > 0)
    (a >= b)  ==>   (a <=> b >= 0)
*/


/*
  class Myclass {
  public:
    bool operator==(int) const {}
  };

  int main()
  {
    Myclass m;

    // --------------------------------------------------------
    // |                   C++17 Standart                     |
    // --------------------------------------------------------

    bool b1 = (m == 5);   // VALID

    bool b2 = (m != 5);   // syntax error in C++17
    //  error: no match for 'operator!=' 
    // (operand types are 'Myclass' and 'int')

    bool b3 = (5 == m);   // syntax error in C++17
    //  error: no match for 'operator=='
    // (operand types are 'int' and 'Myclass')

    // operator==() is a member operator function
    // because of left operand is not a class object 
    // it is not possible to call operator==() function

    bool b4 = (5 != m);   // syntax error in C++17
    //  error: no match for 'operator!='
    // (operand types are 'int' and 'Myclass)

    // operator==() is a member operator function
    // because of left operand is not a class object 
    // it is not possible to call operator==() function

    // --------------------------------------------------------
  }
*/

/*
  class Myclass {
  public:
    bool operator==(int) const { return 1;}
  };

  int main()
  {
    Myclass m;

    // --------------------------------------------------------
    // |                   since C++20 Standart               |
    // --------------------------------------------------------

    bool b1 = (m == 5);   // VALID

    bool b2 = (m != 5);   // VALID 
    //  (rewritten as !(m == 5))

    bool b3 = (5 == m);   // VALID 
    //  (reversed as (m == 5))

    bool b4 = (5 != m);   // VALID 
    //  (rewritten as !(5 == m)) -  (reversed as !(m == 5))

    // --------------------------------------------------------
  }
*/

/*
  class Myclass {
  public:
    bool operator==(int) const 
    {
      std::cout << "operator==()\n";
      return true;
    }
  };

  int main()
  {
    Myclass m;

    bool b1 = (m == 5);   // output -> operator==()
    bool b2 = (m != 5);   // output -> operator==()
    bool b3 = (5 == m);   // output -> operator==()
    bool b4 = (5 != m);   // output -> operator==()
  }
*/

/*
  class Myclass{
  public:
    Myclass(int x) : m_x{x} {}
    auto operator<=>(const Myclass&) const = default;
  private:
    int m_x;
  };

  int main()
  {
    using namespace std;
    boolalpha(cout);

    Myclass m1{22}, m2{55}, m3{22};

    // --------------------------------------------------------

    cout << "(m1 < m2)  = " << (m1 < m2) << '\n';
    cout << "(m1 <= m2) = " << (m1 <= m2) << '\n';
    cout << "(m1 > m2)  = " << (m1 > m2) << '\n';
    cout << "(m1 >= m2) = " << (m1 >= m2) << '\n';
    cout << "(m1 == m2) = " << (m1 == m2) << '\n';
    cout << "(m1 != m2) = " << (m1 != m2) << '\n';

    // output ->
    //  (m1 < m2)  = true
    //  (m1 <= m2) = true
    //  (m1 > m2)  = false
    //  (m1 >= m2) = false
    //  (m1 == m2) = false
    //  (m1 != m2) = true

    // --------------------------------------------------------

    cout << "(m1 < m3)  = " << (m1 < m3) << '\n';
    cout << "(m1 <= m3) = " << (m1 <= m3) << '\n';
    cout << "(m1 > m3)  = " << (m1 > m3) << '\n';
    cout << "(m1 >= m3) = " << (m1 >= m3) << '\n';
    cout << "(m1 == m3) = " << (m1 == m3) << '\n';
    cout << "(m1 != m3) = " << (m1 != m3) << '\n';

    // output ->
    //  (m1 < m3)  = false
    //  (m1 <= m3) = true
    //  (m1 > m3)  = false
    //  (m1 >= m3) = true
    //  (m1 == m3) = true
    //  (m1 != m3) = false

    // --------------------------------------------------------
  }
*/

/*
  class Myclass_1 {
  public:
    Myclass_1(int x) : m_x{x} {}
    auto operator<=>(const Myclass_1&) const = default;
  private:
    int m_x;
  };

  // when spaceship operator is user declared defaulted
  // compiler will implicitly defaulted operator==() function

  class Myclass_2 {
  public:
    Myclass_2(int x) : m_x{x} {}
    auto operator<=>(const Myclass_2&) const = default;
    bool operator==(const Myclass_2&) const = default;
  private:
    int m_x;
  };

  // Myclass_1 and Myclass_2 are equivalent
*/

/*
  class Myclass {
  public:
    Myclass(int x) : m_x{x} {}
    bool operator==(const Myclass&) const = default;
  private:
    int m_x;
  };
  // When operator==() is user declared defaulted
  // compiler WILL NOT implicitly defaulted operator<=>() function

  int main()
  {
    using namespace std;

    Myclass m1{22}, m2{55}, m3{22};

    // --------------------------------------------------------

    cout << "(m1 == m2) = " << (m1 == m2) << '\n';  // VALID
    cout << "(m1 != m2) = " << (m1 != m2) << '\n';  // VALID

    // --------------------------------------------------------

    cout << "(m1 < m2)  = " << (m1 < m2) << '\n';   // syntax error
    // error: no match for 'operator<' 
    // (operand types are 'Myclass' and 'Myclass')
    cout << "(m1 <= m2) = " << (m1 <= m2) << '\n';  // syntax error
    // error: no match for 'operator<=' 
    cout << "(m1 > m2)  = " << (m1 > m2) << '\n';   // syntax error
    // error: no match for 'operator>' 
    cout << "(m1 >= m2) = " << (m1 >= m2) << '\n';  // syntax error
    // error: no match for 'operator>=' 

    // --------------------------------------------------------
  }
*/

/*
  class Date {
  public:
    Date(int d, int m, int y) : 
        m_year{ y }, m_month{ m }, m_day{ d } {}
    auto operator<=>(const Date&) const = default;
  private:
    int m_year, m_month, m_day;
  };

  int main()
  {
    using namespace std;
    boolalpha(cout);

    cout << (Date{ 30, 7, 2023 } > Date{ 30, 6, 2023} ) << '\n';
    // output -> true

    cout << (Date{ 30, 7, 2023 } == Date{ 30, 7, 2023} ) << '\n';
    // output -> true
  }
*/

/*
*/

// Lesson_11 -> 02:46:00