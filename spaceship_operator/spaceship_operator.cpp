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
  --------------------------------------------------------

  - types that are returned by the spaceship operator
    strong_ordering 
      - a > b
      - a < b
      - a == b

    weak_ordering 
      hello     HELLO   (equivalent)

    partial_ordering
      NAN
      positive infinity
      negative infinity

  --------------------------------------------------------

  - values that are returned by the spaceship operator

  strong_ordering::equal
  strong_ordering::equivalent
  strong_ordering::greater
  strong_ordering::less

  weak_ordering::equivalent
  weak_ordering::greater
  weak_ordering::less

  partial_ordering::equivalent
  partial_ordering::greater
  partial_ordering::less
  partial_ordering::unordered

  --------------------------------------------------------

  i.e (a <=> b) returns strong_ordering::greater 
      (a <=> b > 0)  ===> (strong_ordering::greater > 0)

  i.e (a <=> b) returns weak_ordering::less
      (a <=> b < 0)  ===> (weak_ordering::less < 0)

  --------------------------------------------------------

  - conversion between strong_ordering and weak_ordering
    and partial_ordering types

    strong_ordering can be converted to weak_ordering and 
    partial_ordering types
      strong_ordering ===> weak_ordering
      strong_ordering ===> partial_ordering

    weak_ordering can be converted to partial_ordering type
      weak_ordering ===> partial_ordering

  --------------------------------------------------------
*/

/*
  #include <compare>      // std::compare_three_way_result_t
  #include <string>
  #include <type_traits>  // std::is_same_v
  #include <cmath>        // NAN

  template <typename T, typename U>
  void print_compare(const T& t, const U& u)
  {
    using result_type = std::compare_three_way_result_t<T, U>;
    std::string stype = typeid(result_type).name();

    std::cout << "compare result_type: " << stype << '\n';
    auto result = t <=> u;

    std::cout << "result of comparison: ";
    if (result == 0){
      if (std::is_same_v<result_type, std::strong_ordering>)
        std::cout << "equal\n";
      else
        std::cout << "equivalent\n";
    }
    else if (result > 0)
      std::cout << "greater\n";
    else if (result < 0)
      std::cout << "less\n";
    
    else
      std::cout << "unordered\n";
  }

  int main()
  {
    // --------------------------------------------------------

    print_compare(12, 6);
    // output ->
    // compare result_type: struct std::strong_ordering
    // result of comparison: greater

    print_compare(12, 63);
    // output ->
    // compare result_type: struct std::strong_ordering
    // result of comparison: less

    print_compare(12, 12);
    // output ->
    // compare result_type: struct std::strong_ordering
    // result of comparison: equal

    // --------------------------------------------------------

    print_compare(6.3, 6.3);
    // output ->
    // compare result_type: struct std::partial_ordering
    // result of comparison: equivalent

    print_compare(6.3, 6.41);
    // output ->
    // compare result_type: struct std::partial_ordering
    // result of comparison: less

    print_compare(6.3, NAN); 
    // output ->
    // compare result_type: struct std::partial_ordering
    // result of comparison: unordered

    // --------------------------------------------------------
  }
*/

/*
  #include <string>

  class Person {
  public:
    Person(const char* p, int a) : m_name{ p }, m_age{ a } {}
    std::strong_ordering operator<=>(const Person& other) const
    {
      if (auto cmp = m_name <=> other.m_name; cmp != 0)
        return cmp;

      return m_age <=> other.m_age;
    }
    // when 2 std::string objects and 2 ints are compared 
    // with spaceship operator
    // both return types are std::strong_ordering
    // std::strong_ordering can be used instead of auto
    // for the return type of the operator<=>() function

    // if function's return type is std::weak_ordering
    // because of strong_ordering can be converted to weak_ordering
    // no syntax error will occur
  private:
    std::string m_name;
    int m_age;
  };

  int main()
  {
    using namespace std;
    boolalpha(cout);

    Person p1{"hello", 55};
    Person p2{"art", 40};
    Person p3{"art", 23};

    // --------------------------------------------------------

    cout << (p1 < p2) << '\n';        // output -> false
    cout << (p1 <=> p2 < 0) << '\n';  // output -> false
    // Those 2 lines are equivalent.

    // --------------------------------------------------------

    cout << (p2 > p3) << '\n';        // output -> true
    cout << (p2 <=> p3 > 0) << '\n';  // output -> true
    // Those 2 lines are equivalent.

    // --------------------------------------------------------
  }
*/

/*
  // ------------------- PROMBLEM -------------------

  #include <string>

  class Person {
  public:
    Person(const char* p, int a, double w) : 
        m_name{ p }, m_age{ a }, m_wage{ w } {}

    auto operator<=>(const Person& other) const
    {
      if (auto cmp = m_name <=> other.m_name; cmp != 0)
        return cmp;

      if (auto cmp = m_age <=> other.m_age; cmp != 0)
        return cmp;

      return m_wage <=> other.m_wage; // syntax error
      // error: inconsistent deduction for auto return type: 
      // 'std::strong_ordering' and then 'std::partial_ordering'
    }

  private:
    std::string m_name;
    int m_age;
    double m_wage;
  };
*/

/*
  // ------------------- SOLUTION 1 -------------------

  class Person_1 {
  public:
    Person_1(const char* p, int a, double w) : 
        m_name{ p }, m_age{ a }, m_wage{ w } {}

    std::partial_ordering operator<=>(const Person_1& other) const
    {
      if (auto cmp = m_name <=> other.m_name; cmp != 0)
        return cmp;

      if (auto cmp = m_age <=> other.m_age; cmp != 0)
        return cmp;

      return m_wage <=> other.m_wage;
      // because of std::strong_ordering can be converted to
      // std::partial_ordering no syntax error will occur
    }

  private:
    std::string m_name;     // <=> will return std::strong_ordering
    int m_age;              // <=> will return std::strong_ordering
    double m_wage;          // <=> will return std::partial_ordering
  };
*/

/*
  // ------------------- SOLUTION 2 -------------------

  #include <compare>  // std::common_comparison_category

  class Employee {
  private:
    std::string m_name;
    double m_wage;
  public:
    // trailing return type
    auto operator<=>(const Employee& other) const -> 
        std::common_comparison_category_t<
              decltype(m_name <=> other.m_name), 
              decltype(m_wage <=> other.m_wage)>
    {
      if (auto cmp = m_name <=> other.m_name; cmp != 0)
        return cmp;

      return m_wage <=> other.m_wage;
    }
  };
*/

/*
  // ------------------- SOLUTION 3 -------------------

  #include <compare>  // std::strong_ordering, std::partial_ordering
  #include <cassert>

  class Employee {
  private:
    std::string m_name;
    double m_wage;
  public:
    std::strong_ordering operator<=>(const Employee& other) const 
    {
      if (auto cmp = m_name <=> other.m_name; cmp != 0)
        return cmp;

      auto cmp = m_wage <=> other.m_wage;

      assert(cmp != std::partial_ordering::unordered);

      return  cmp == 0  ? std::strong_ordering::equal : 
              cmp > 0   ? std::strong_ordering::greater 
                        : std::strong_ordering::less;
    }
  };
*/

/*
  // ------------------- SOLUTION 4 -------------------
  
  #include <compare>  // std::strong_ordering, std::strong_order
  class Employee {
  private:
    std::string m_name;
    double m_wage;
  public:
    std::strong_ordering operator<=>(const Employee& other) const 
    {
      if (auto cmp = m_name <=> other.m_name; cmp != 0)
        return cmp;

      return std::strong_order(m_wage, other.m_wage);
    }
  };
*/

/*
  class Myclass {
  public:
    constexpr Myclass(int x = 0) : m_x{ x } {}
    bool operator==(const Myclass&) const;
  private:
    int m_x;
  };

  class Myclass_2 {
  public:
    constexpr Myclass_2(int x = 0) : m_x{ x } {}
    bool operator==(const Myclass_2&) const = default;
  private:
    int m_x;
  };

  int main()
  {
    using namespace std;

    // --------------------------------------------------------

    // if there is no problem being operator==() function
    // to be a constexpr function, compiler will implicitly
    // define operator==() function as a constexpr function

    // --------------------------------------------------------

    constexpr Myclass m1{ 300 };
    constexpr Myclass m2{ 400 };

    constexpr auto b1 = m1 == m2;  // syntax error
    // error: call to non-'constexpr' function 
    // 'bool Myclass::operator==(const Myclass&) const'

    constexpr Myclass_2 m3{ 300 };
    constexpr Myclass_2 m4{ 400 };

    constexpr auto b2 = m3 == m4;  // VALID

    // --------------------------------------------------------

    // if comparison of data members is giving a noexcept 
    // guarantee, when operator==() function is defaulted
    // compiler will implicitly define operator==() function
    // as a noexcept function

    // --------------------------------------------------------

    constexpr auto b3 = noexcept(m1 == m2);  // b3 -> false

    constexpr auto b4 = noexcept(m3 == m4);  // b4 -> true

    // --------------------------------------------------------
  }
*/

/*
  class Myclass {
  public:
    constexpr Myclass(int x = 0) : m_x{ x } {}
    [[nodiscard]] auto operator<=>(const Myclass&) const = default;
  private:
    int m_x;
  };

  int main()
  {
    using namespace std;

    constexpr Myclass m1{ 300 };
    constexpr Myclass m2{ 400 };

    // --------------------------------------------------------

    // operator==() function will also have all the attributes 
    // of the operator<=>() function
    // i.e [[nodiscard]] attribute

    // --------------------------------------------------------

    m1 <=> m2;
    // warning: ignoring return value of 
    // 'constexpr auto Myclass::operator<=>(const Myclass&) const', 
    // declared with attribute 'nodiscard'

    m1 == m2;
    // warning: ignoring return value of 
    // 'constexpr bool Myclass::operator==(const Myclass&) const', 
    // declared with attribute 'nodiscard'

    // --------------------------------------------------------
  }
*/

/*
  #include <compare>  // std::strong_ordering
  #include <concepts> // std::same_as

  template <typename T>
  class Type {
  public:
    [[nodiscard]] 
    virtual std::strong_ordering operator<=>(const Type&) const 
    requires(!std::same_as<T, bool>) = default;
  };

  // when compiler sees Type class it will write a class that
  // exactly like below(compiler_Type class)

  template <typename T>
  class compiler_Type {
  public:
    [[nodiscard]]
    virtual std::strong_ordering operator<=>(const compiler_Type&) const 
    requires(!std::same_as<T, bool>) = default;

    [[nodiscard]]
    virtual bool operator==(const compiler_Type&) const
    requires(!std::same_as<T, bool>) = default;
  };
*/

/*
  #include <compare>  // std::strong_ordering

  class Date {
  public:
    std::strong_ordering operator<=>(const Date& other) const noexcept
    {
      if (auto cmp = m_year <=> other.m_year; cmp != 0)
        return cmp;

      if (auto cmp = m_month <=> other.m_month; cmp != 0)
        return cmp;

      return m_day <=> other.m_day;
    }
  private:
    int m_year, m_month, m_day;
  };

  // comparison functions of int data members are noexcept
*/

/*
  - default comparison of containers was done with 
    lexicographical comparison
*/

/*
  #include <vector>
  #include <list>
  #include <algorithm>  
    // std::lexicographical_compare
    // std::lexicographical_compare_three_way (C++20)
  #include <compare>   
    // std::strong_ordering

  std::ostream& operator<<(std::ostream& os, std::strong_ordering s_o)
  {
    return  os << ( s_o == 0  ? "equal" :
                    s_o < 0   ? "less"  : "greater");
  }

  int main()
  {
    using namespace std;
    boolalpha(cout);

    // --------------------------------------------------------

    vector<int> v1(100'000, 5);
    vector<int> v2{6, 9};

    cout << (v2 > v1) << '\n';  // output -> true
    // v2's first element(6) is greater than v1's first element(5)

    // --------------------------------------------------------

    list l1 = { 3, 6, 7, 9, 1 };
    vector v3 = { 3, 6, 9, 1 };

    auto b1 = lexicographical_compare(l1.begin(), l1.end(), 
                                      v3.begin(), v3.end());
    // auto b1 = l1 < v3;
    // default predicate argument  is std::less

    cout << b1 << '\n';  // output -> true
    // l1's third element(7) is less than v3's third element(9)

    // --------------------------------------------------------
    
    list l2 = { 3, 6, 7, 9, 8 };
    vector v4 = { 3, 6, 7, 9, 2 };

    auto result = 
      lexicographical_compare_three_way(l2.begin(), l2.end(), 
                                        v4.begin(), v4.end());
    // result's data type is std::strong_ordering

    cout << result << '\n';  // output -> greater  
    
    // --------------------------------------------------------
  }
*/