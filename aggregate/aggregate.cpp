#include <iostream>

/*
                          =============
                          | Aggregate |
                          =============
*/

/*
  Aggregate(Bileşik) type : 
    - non-static veri elemanlarını doğrudan kullanıma açan türler.
    - veri elemanları interface'inin doğrudan parçası.
    - veri elemanlarını koruma(data - information hiding) yok.

  - diziler(arrays) aggregate türler.
    std::string aggregate değil, std::string[10] aggregate.
*/

/*
  Aggregate olma koşulları :
    1. user declared constructor olamaz.
      (C++20 öncesi user declared defined constructor olamaz.)
    2. non-static veri elemanlarının tamamı public olmalı.
    3. protected or private inheritance olamaz.
    4. virtual inheritance olamaz.
    5. inherited constructor olamaz.
    6. sanal(virtual) fonksiyonu olamaz.
    7. lambda ifadesi olamaz.
*/

/*
  #include <type_traits>  // is_aggregate metafunction (C++17)
  #include <string>

  struct Mystruct {
    int m_x, m_y, m_z;
  };

  struct Mystruct_2 {
    int m_x, m_y;
  private:
    int m_z;   // non-static private data member
  };

  struct Mystruct_3 {
    int m_x, m_y;
    std::string m_str;
  };

  int main()
  {
    // ---------------------------------------------------

    static_assert(std::is_aggregate_v<Mystruct>);     // Aggregate
    static_assert(std::is_aggregate_v<Mystruct_2>);   // FAILED

    // ---------------------------------------------------

    static_assert(std::is_aggregate_v<std::string>);  // FAILED
    static_assert(std::is_aggregate_v<Mystruct_3>);   // Aggregate
    // elemanların aggregate olması gerekliliği yok.

    // ---------------------------------------------------
  }
*/

/*
  #include <type_traits>  // is_aggregate

  class AClass {};

  struct BClass : public AClass {
    int m_x, m_y;
  };
  // public inherited Derived classes can be an aggregate type.

  struct CClass : private AClass {
    int m_x, m_y;
  };
  // private inherited Derived classes CAN NOT be an aggregate type.

  struct DClass : protected AClass {
    int m_x, m_y;
  };
  // protected inherited Derived classes CAN NOT be an aggregate type.

  int main()
  {
    // ---------------------------------------------------

    static_assert(std::is_aggregate_v<BClass>);  // Aggregate
    static_assert(std::is_aggregate_v<CClass>);  // FAILED
    static_assert(std::is_aggregate_v<DClass>);  // FAILED

    // ---------------------------------------------------
  }
*/

/*
  #include <string>
  #include <type_traits>  // is_aggregate

  class BClass {
  private:
    int m_x;
  };  // BClass is not an aggregate type.

  struct AClass : public std::string {
    int m_x, m_y;
    BClass m_b;
  };
  // public inherited Derived class from not aggreated Base class
  // can be an aggregate type.

  // a class that have a not aggregate member type 
  // can be an aggregate type.

  int main()
  {
    static_assert(std::is_aggregate_v<BClass>);       // FAILED
    static_assert(std::is_aggregate_v<std::string>);  // FAILED

    static_assert(std::is_aggregate_v<AClass>);       // Aggregate
  }
*/

/*
  #include <type_traits>  // is_aggregate

  class Myclass {
  public:
    int m_x, m_y;
  };

  struct AClass : virtual Myclass {
    int m_a, m_b;
  };

  int main()
  {
    static_assert(std::is_aggregate_v<AClass>);       // FAILED
  }
*/

/*
  #include <type_traits>  // is_aggregate

  class AClass {};
  class BClass {};
  class CClass {};

  struct Mystruct : AClass, BClass, CClass {
    int m_x, m_y, m_z;
  };
  // multiple inherited Derived class can be an aggregate type.

  int main()
  {
      static_assert(std::is_aggregate_v<Mystruct>); // Aggregate
  }
*/

/*
  #include <type_traits>  // is_aggregate

  class AClass {
  public:
    AClass(int);
    AClass(double);
  };

  struct Mystruct_1 : AClass {
    int m_x, m_y;
  };

  struct Mystruct_2 : AClass {
    using AClass::AClass; // inherited constructor
    int m_x, m_y;
  };
  // Aggregate types CAN NOT have inherited constructor.

  int main()
  {
    static_assert(std::is_aggregate_v<Mystruct_1>);  // Aggregate
    static_assert(std::is_aggregate_v<Mystruct_2>);  // FAILED
  }
*/

/*
  struct Mystruct {
    int m_x, m_y;
    int &r1, &&r2;
  };
  // Aggregate types can have reference data members.

  class Myclass {
  public:
    int m_x, m_y;
    int f1();
  private:
    int f2();
  protected:
    int f3();
  };
  // Aggregate types can have private and protected member functions

  int main()
  {
    static_assert(std::is_aggregate_v<Mystruct>);   // Aggregate
    static_assert(std::is_aggregate_v<Myclass>);    // Aggregate
  }
*/

/*
  #include <type_traits>  // is_aggregate

  class Myclass {
  public:
    int m_x, m_y;
    virtual void f1();
  };
  // Aggregate types CAN NOT have virtual member functions.

  int main()
  {
    static_assert(std::is_aggregate_v<Myclass>);    // FAILED
  }
*/

/*
  #include <type_traits>  // is_aggregate

  struct Mystruct {
    int m_x = 5;
    int m_y = 10;
  };
  // Aggregate types can have default member initializer. (since C++14)

  int main()
  {
    static_assert(std::is_aggregate_v<Mystruct>);   // Aggregate
  }
*/

/*
  #include <type_traits>  // is_aggregate

  int main()
  {
    static_assert(std::is_aggregate_v<decltype([](){})>); // FAILED
    // "[](){}" is a lambda expression (closure type)
  }
*/

/*
  #include <type_traits>  // is_aggregate

  auto f1 = [](int x){ return x * x; };
  auto f2 = [](int x){ return x * 5; };

  struct Mystruct : decltype(f1), decltype(f2) {
    int m_x;
  };
  // a class that inherited from closure type, can be an aggragate type

  int main()
  {
    static_assert(std::is_aggregate_v<Mystruct>);   // Aggregate
  }
*/

/*
  #include <string>
  #include <type_traits>  // is_aggregate

  struct Mystruct {
    int m_arr[10];
  };

  struct Mystruct2 {
    std::string m_str_arr[10];
  };

  int main()
  {
    static_assert(std::is_aggregate_v<Mystruct>);   // Aggregate
    static_assert(std::is_aggregate_v<Mystruct2>);  // Aggregate
  }
*/

/*
  #include <type_traits>  // is_aggregate

  struct Mystruct {
    int m_x;
    Mystruct operator+(const Mystruct&) const;
  };
  // type that have an operator overload member function
  // can be an aggregate type

  int main()
  {
    static_assert(std::is_aggregate_v<Mystruct>);   // Aggregate
  }
*/

/*
  #include <type_traits>  // is_aggregate
  #include <string>

  struct Mystruct {
    int m_x;
    static int ms_x;
    inline static std::string ms_str{"hello world"};
  };
  // type that have a static data member can be an aggregate type

  int main()
  {
    static_assert(std::is_aggregate_v<Mystruct>);   // Aggregate
  }
*/

/*
  struct Point {
    int m_x, m_y, m_z;
  };

  int main()
  {
    // --------------------------------------------------

    Point p1 = { 1, 3, 5 }; // aggregate initialization
    Point p2{ 2, 6, 10 };   // aggregate initialization
    
    // --------------------------------------------------

    Point p3 = { 2, 4, 5.3 }; // syntax error
    // error: narrowing conversion of '5.2999999999999998e+0' 
    // from 'double' to 'int'

    // --------------------------------------------------
  }
*/

/*
  #include <string>

  struct Mystruct {
    int m_ival;
    double m_dval;
    std::string m_str;
    int m_arr[4];
  };

  struct AStruct {
    int m_x, m_y;
  };

  struct BStruct {
    int m_x;
    AStruct m_as;
    double m_darr[3];
  };

  int main()
  {
    Mystruct m3 = { 11, 22.2 , "hello galaxy", { 22, 33, 44, 55 } };
    Mystruct m4 = { 11, 22.2 , "hello galaxy", 22, 33, 44, 55 };
    // Those 2 lines are equivalent.

    BStruct b1 = {10, { 11, 22 }, { 33.3, 44.4, 55.5 }};
    BStruct b2 = {10, 11, 22 , 33.3, 44.4, 55.5 };
    // Those 2 lines are equivalent.
  }
*/

/*
  #include <string>

  struct Mystruct {
    int m_ival;
    double m_dval;
    bool m_b;
    std::string m_str;
  };

  int main()
  {
    std::boolalpha(std::cout);

    Mystruct m1 = { 10 };
    // m_dval, m_b and m_str data members are value initialized.

    std::cout << "m1.m_b = " << m1.m_b << '\n'; 
    // output -> m1.m_b = false

    std::cout << "m1.m_str.size() = " << m1.m_str.size() << '\n'; 
    // output -> m1.m_str.size() = 0
  }
*/

/*
  struct Mystruct {
    int m_x;
  };

  int main()
  {
    Mystruct m1 = 10;     // syntax error
    // error: conversion from 'int' to
    // non-scalar type 'Mystruct' requested

    Mystruct m2 = { 10 }; // VALID
    Mystruct m3{ 10 };    // VALID
  }
*/

/*
  #include <array>

  template <typename T, std::size_t N>
  struct Array {
    T m_arr[N];
  };

  int main()
  {
    std::array<int, 5> arr1 = { 1, 2, 3, 4 };
    std::array<int, 5> arr2{ 1, 2, 3 };
    // std::array is an aggregate type.

    Array<int, 5> a1 = { 1, 2, 3, 4 };  // aggregate initialization
    Array<int, 5>{ 1, 2, 3 };           // aggregate initialization
  }
*/

/*
  #include <array>

  struct Mystruct {
    int m_x, m_y;
  };

  int main()
  {
    // --------------------------------------------------

    Mystruct m1;  // default initialization
    std::cout << m1.m_x << '\n'; // undefined behavior(UB)

    // --------------------------------------------------

    std::array<int, 5> arr1;  // default initialization

    for (auto val : arr1)
      std::cout << val << ' '; // undefined behavior(UB)

    // --------------------------------------------------
  }
*/

/*
  #include <string>

  struct Mystruct {
    int m_x, m_y;
    std::string m_str;
  };

  int main()
  {
    Mystruct m1{ 10, 20, "hello world" };

    auto [x, y, str] = m1;  // structured binding
  }
*/

/*
  struct Mystruct {
    Mystruct() = delete;
    int m_x;
  };

  int main()
  {
    // ---------------------- C++17 ----------------------

    Mystruct m1;  // syntax error (default initialization)
    // error: use of deleted function 'Mystruct::Mystruct()'

    Mystruct m2{};      // VALID (aggregate initialization)
    Mystruct m3{ 10 };  // VALID (aggregate initialization)

    // --------------------------------------------------


    // ---------------------- C++20 ----------------------

    Mystruct m4;  // syntax error (default initialization)
    // error: use of deleted function 'Mystruct::Mystruct()'

    Mystruct m5{}; // syntax error (aggregate initialization)
    // error: use of deleted function 'Mystruct::Mystruct()'

    Mystruct m6{ 10 }; // syntax error (aggregate initialization)
    // error: no matching function for call to 
    // 'Mystruct::Mystruct(<brace-enclosed initializer list>)'

    // --------------------------------------------------
  }
*/

/*
  #include <string>

  struct Mystruct {
    int m_x;
    int m_y{ 45 };
    std::string m_str{ "hello world" };
    int m_z;
  };

  int main()
  {
    Mystruct m1 = { 12 };

    std::cout << "m1.m_x = " << m1.m_x << '\n';
    std::cout << "m1.m_y = " << m1.m_y << '\n';
    std::cout << "m1.m_str = " << m1.m_str << '\n';
    std::cout << "m1.m_z = " << m1.m_z << '\n';

    // output ->
    //  m1.m_x = 12
    //  m1.m_y = 45
    //  m1.m_str = hello world
    //  m1.m_z = 0
  }
*/

/*
        ------------------------------------------------------
        | designated initializer for aggregate types (C++20) |
        ------------------------------------------------------
*/

/*
  - C designated initializer can be used for arrays.  
  - C++ designated initializer can be used for aggregate types 
    not arrays.
*/

/*
  // ------------------- C Code -------------------

  struct Data {
    int m_a, m_b, m_c;
    char m_str[10];
    int m_arr[5];
  };

  int main()
  {
    int arr[10] = { [2] = 10, [5] = 20, 56 };
    // arr[2] -> 10
    // arr[5] -> 20
    // arr[6] -> 56

    struct Data d1 = {
      .m_str = "hello"
      .m_arr = { [2] = 6 },
    };
  }
*/

/*
  struct Mystruct {
    int m_x, m_y;
  };

  int main()
  {
    Mystruct m1 = { .m_x = 10, .m_y = 20 };

    Mystruct m2{ .m_x = 10 }; 
    std::cout << "m2.m_y = " << m2.m_y << '\n';
    // output -> m2.m_y = 0 (value initialized)
  }
*/

/*
  struct Mystruct {
    int m_x, m_y;
    double m_dval;
  };

  int main()
  {
    Mystruct m1 = { .m_x = 10, .m_y = 20, 3.14 }; // syntax error
    // error: either all initializer clauses should be designated 
    // or none of them should be

    Mystruct m2{ .m_x = 10, .m_y = 20, m_dval = 3.14 };  // syntax error
    // error: either all initializer clauses should be designated 
    // or none of them should be
  }
*/

/*
  struct Mystruct {
    int m_x, m_y;
    double m_dval;
    static int ms_x;
  };

  int main()
  {
    Mystruct m1 = { .ms_x = 10 }; // syntax error 
    // error: 'Mystruct' has no non-static data member named 'ms_x'
  }
*/

/*
  struct Time {
    int m_min;
    int m_hour;
  };

  struct Date{
    int m_year;
    int m_month;
    int m_day;
    Time m_time;
    static int ms_hmode;
  };

  int main()
  {
    // --------------------------------------------------

    Date d1 = { .ms_hmode = 0 };  // syntax error
    // error: 'Date' has no non-static data member named 'ms_hmode'

    // --------------------------------------------------

    Date d2_1 = { .m_month = 3, .m_year = 2003 }; // syntax error
    // error: designator order for field 'Date::m_year' 
    // does not match declaration order in 'Date'

    Date d2_2 = { .m_year = 2003, .m_month = 3 }; // VALID

    // --------------------------------------------------

    Date d3 = { 2003, .m_month = 3 }; // syntax error
    // error: either all initializer clauses should be designated 
    // or none of them should be

    // --------------------------------------------------

    Date d4 = { .m_time.m_min = 60 }; // syntax error
    // error: expected primary-expression before '.' token

    Date d4_2 = { .m_time = { 60 } };       // VALID
    Date d4_3 = { .m_time = { 60, 12 } };   // VALID

    // --------------------------------------------------
  }
*/

/*
  // using designated initializer with default member initializer
  struct Mystruct {
    int m_x;
    int m_y = 5;
    std::string m_str{"hello world"};
    double m_dval;
  };

  int main()
  {
    Mystruct m1{ .m_dval = 3.14 };

    std::cout << "m1.m_x = " << m1.m_x << '\n';
    std::cout << "m1.m_y = " << m1.m_y << '\n';
    std::cout << "m1.m_str = " << m1.m_str << '\n';
    std::cout << "m1.m_dval = " << m1.m_dval << '\n';
    // output ->
    //  m1.m_x = 0
    //  m1.m_y = 5
    //  m1.m_str = hello world
    //  m1.m_dval = 3.14
  }
*/

/*
  #include <vector>

  struct Person {
    int m_id;
    std::string m_name = "hello world";
    int m_age;
  };

  int main()
  {
    using namespace std;

    vector<Person> pvec;
    pvec.push_back(Person{ .m_id = 100, .m_name = "world", .m_age = 30 });
    pvec.push_back({ .m_id = 110, .m_age = 22 });
    pvec.push_back({ .m_id = 120 });
  }
*/

/*
  struct Person {
    int m_id;
    std::string m_name;
    int m_age;
  };

  Person get_person()
  {
    // return Person{.m_id = 100, .m_name = "hello", .m_age = 30};
    // return {100, "hello", 30};
    return {.m_id = 100, .m_name = "hello", .m_age = 30};
    // Those 3 lines are equivalent.
  }

  int main()
  {
    auto [id, name, age] = get_person();
  }
*/

//  void process_file(bool open, bool close, bool read, bool write);
//
//  struct FileProperties {
//    bool m_open;
//    bool m_close;
//    bool m_read;
//    bool m_write;
//  };
//
//  void process_file2(const FileProperties& fprop);
//
//  int main()
//  {
//    // ------- FIRST TECHNIQUE -------
//    process_file( /*open*/true, 
//                  /*close*/false, 
//                  /*read*/true, 
//                  /*write*/false);
//
//
//    // ------- SECOND TECHNIQUE -------
//    process_file2({ .m_open = true, 
//                    .m_close = false, 
//                    .m_read = true, 
//                    .m_write = false});
//  }


/*
  struct Point {
    int m_x, m_y;
  };

  struct Point3D {
    double m_dx, m_dy, m_dz;
  };

  void process(const Point& p)
  {
    std::cout << "const Point&\n";
  }

  void process(const Point3D& p)
  {
    std::cout << "const Point3D&\n";
  }

  int main()
  {
    process({.m_x = 45 });    // output -> const Point&
    process({.m_dx = 3.14 }); // output -> const Point3D&
  }
*/

/*
  #include <string>

  template <typename T>
  struct Mystruct {
    T m_x;
    int m_ival;
  };

  // deduction guide
  template <typename T>
  Mystruct(T, int) -> Mystruct<T>;


  template <typename T>
  struct Mystruct2 {
    T m_x;
    int m_ival;
  };

  int main()
  {
    // --------------------------------------------------
    // ---------------------- C++17 ---------------------
    // --------------------------------------------------

    Mystruct<double> m1{ 3.14, 10 };
    Mystruct<std::string> m2{ "hello world", 20 };

    Mystruct m3{ 3.14, 10 };            // syntax error
    // error: class template argument deduction failed:
    // error: no matching function for call to 'Mystruct(double, int)'

    Mystruct m4{ "hello galaxy", 22 };  // syntax error
    // error: class template argument deduction failed:
    // error: no matching function for call to 
    // 'Mystruct(const char [13], int)'

    // deduction guide needed for class template argument deduction
    

    // --------------------------------------------------
    // ---------------------- C++20 ---------------------
    // --------------------------------------------------

    Mystruct2 m2_1{ 3.14, 10 };           // VALID
    Mystruct2 m2_2{ "hello world", 20 };  // VALID

    // --------------------------------------------------
  }
*/

/*
  #include <memory>
  #include <vector>

  template <typename T, typename ...Args>
  std::unique_ptr<T> Make_Unique(Args&& ...args)
  {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
  }

  struct Mystruct {
    int m_x, m_y, m_z;
  };

  int main()
  {
    // --------------------------------------------------
    // ---------------------- C++17 ---------------------
    // --------------------------------------------------

    auto p = new Mystruct(10, 20, 30);      // syntax error
    // error: new initializer expression list treated as 
    // compound expression
    // error: no matching function for call to 'Mystruct::Mystruct(int)'

    auto up = std::make_unique<Mystruct>(10, 20, 30); // syntax error
    // error: new initializer expression list treated as 
    // compound expression
    // error: no matching function for call to 'Mystruct::Mystruct(int)'

    auto sp = std::make_shared<Mystruct>(10, 20, 30); // syntax error
    // error: new initializer expression list treated as 
    // compound expression
    // error: no matching function for call to 'Mystruct::Mystruct(int)'

    std::vector<Mystruct> mvec;
    mvec.emplace_back(10, 20, 30); // syntax error
    // error: new initializer expression list treated as 
    // compound expression
    // error: no matching function for call to 'Mystruct::Mystruct(int)'

    // before C++20, aggregate types can not be direct initialized.

    int arr[3](10, 20, 30); // syntax error
    // error: expression list treated as compound expression 
    // in initializer 
    // error: array must be initialized with a brace-enclosed initializer

    // --------------------------------------------------

    // --------------------------------------------------
    // ---------------------- C++20 ---------------------
    // --------------------------------------------------

    // no syntax error above lines.
  }
*/

