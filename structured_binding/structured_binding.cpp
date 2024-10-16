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

/*
  #include <tuple>
  #include <vector>
  #include <string>
  #include <fstream>    // std::ofstream
  #include <iomanip>    // std::setw, std::setf
  #include <algorithm>  // std::sort

  #include "../nutility.h"

  using Person = std::tuple<int, std::string, std::string>;

  int main()
  {
    using namespace std;

    ofstream ofs{ "out.txt" };
    if (!ofs){
      std::cerr << "out.txt can not created\n";
      exit(EXIT_FAILURE);
    }

    ofs.setf(ios::left, ios::adjustfield);

    vector<Person> pvec;
    pvec.reserve(10'000u);

    for (int i = 0; i < 10'000; ++i){
      pvec.emplace_back(Irand{ 0, 100'000 }(), 
                        rname() + ' ' + rfname(),
                        rtown());
    }

    std::sort(pvec.begin(), pvec.end());

    for (const auto& [id, name, town] : pvec){
      ofs << setw(12) << id << '\t' 
          << setw(24) << name << '\t' 
          << town << '\n';
    }
  }
*/

/*
  #include <tuple>  
  // std::tuple_size_v(metafunction)
  // std::tuple_element_t(metafunction)
  // std::get function template

  using tpl_type = std::tuple<int, double, long>;

  int main()
  {
    // -----------------------------------------------------

    std::tuple_size<tpl_type>::value; // 3
    std::tuple_size_v<tpl_type>;      // 3
    // (compile-time constant)

    // -----------------------------------------------------

    std::tuple_element<0, tpl_type>::type x1{}; // int
    std::tuple_element_t<0, tpl_type> x2{};     // int

    std::tuple_element<1, tpl_type>::type x3{}; // double
    std::tuple_element_t<1, tpl_type> x4{};     // double

    std::tuple_element<2, tpl_type>::type x5{}; // long
    std::tuple_element_t<2, tpl_type> x6{};     // long

    // -----------------------------------------------------

    std::get<0>(tpl_type{ 1, 3.14, 100L }); // 1
    // std::get is for run-time access
  }
*/

/*
  #include <array>  
  // std::tuple_element_t, std::tuple_size_v, std::get

  using array_type = std::array<double, 20>;

  int main()
  {
    // -----------------------------------------------------

    std::tuple_size_v<array_type>;  // 20

    // -----------------------------------------------------

    std::tuple_element_t<0, array_type> x1{}; // double

    // -----------------------------------------------------

    array_type arr{ 1., 5., 6. };
    auto first_elem = std::get<0>(arr);

    std::cout << "first_elem = " << first_elem << "\n";  
    // output -> first_elem = 1

    // -----------------------------------------------------
  }
*/

/*
  --------------------------------------------------------------
  |                  auto& [ival, str] = t1;                   |
  --------------------------------------------------------------

  - derleyici, önce bizim doğrudan görmediğimiz bir referans 
    değişken tanımlar.

  auto& anon_var = t1;

  --------------------------------------------------------------

  - std::tuple_size meta-fonksiyonu ile bu tuple türünün öğe sayısını
    derleme zamanında kontrol ediyor.

  std::tuple_size_v<std::remove_reference_t<decltype(anon_var)>>;

  --------------------------------------------------------------

  - eğer bu sayı structured binding içinde kullanılan 
    special identifier sayısına eşit değilse derleyici hata verir.

  --------------------------------------------------------------

  - std::get<> fonksiyonuna çağrılar yaparak tuple'ın öğelerine
    referans döndürür.

  std::get<0>(t1);
  std::get<1>(t1);

  --------------------------------------------------------------

  - std::get fonksiyonlarının geri dönüş değerleriyle 
    std::tuple_element_t türünden gizli değişkenler
    oluşturur(aynı işlem tekrar yapılmasın diye).

  std::tuple_element_t<0, 
    std::remove_reference_t<decltype(anon_var)>>& 
      anon_elem_0 = std::get<0>(t1);

  std::tuple_element_t<1, 
    std::remove_reference_t<decltype(anonymous_variable)>>& 
      anon_elem_1 = std::get<1>(t1);

  --------------------------------------------------------------
*/

/*
  #include <tuple>
  #include <string>

  int main()
  {
    std::tuple t1(444, std::string("hello"));
    auto& [ival, str] = t1;

    str = "world";
    std::cout << "ival = " << ival
              << ", str = " << str << "\n";  // output -> 444

    // -----------------------------------------------------

    // auto& anon_var = t1;
    // int& anon_elem_0 = std::get<0>(t1);
    // std::string& anon_elem_1 = std::get<1>(t1);

    // -----------------------------------------------------
  }
*/

/*
  auto [x, y] = object;   
    hidden variables types are not a reference type.

  auto& [x, y] = object;
    hidden variables types are L value reference type.

  auto&& [x, y] = object;
    hidden variables types are reference type
    (L value OR R value reference).
*/

/*
  Question: What if get<>() function's return type is 
            not an L value reference?

  for tuple get function's return type is an L value reference 
  but for a custom type it might also be the value type or 
  R value reference.

  for L value reference, special identifiers are,
    int& anon_elem_0 = std::get<0>(t1);
    std::string& anon_elem_1 = std::get<1>(t1);
  
  for value or R value reference special identifiers are,
    int&& anon_elem_0 = std::get<0>(t1);          // life extension
    std::string&& anon_elem_1 = std::get<1>(t1);  // life extension
*/

/*
  #include <tuple>

  // generalized lambda expression generated closure type
  class xyzt_lambda{
  public:
    template <typename T>
    auto operator()(T x) {}
  }

  int main()
  {
    auto [x, y] = std::make_tuple(10, 3.14);

    std::cout << "x = " << x << "\n";  // output -> x = 10

    // -----------------------------------------------------

    auto f1 = [x] { return x * 2; };    // syntax error in C++17

    // -----------------------------------------------------

    auto f1_2 = [&x = x] { x *= 2; };   // VALID since C++17
    // lambda init. capture syntax 
    // to capture "x" by reference before C++20

    f1_2();
    std::cout << "x = " << x << "\n";   // output -> x = 20

    // -----------------------------------------------------

    auto f2 = [&x] { x *= 2; };         // VALID since C++20
    // to capture "x" by reference since C++20

    f2();
    std::cout << "x = " << x << "\n";   // output -> x = 40

    // -----------------------------------------------------

    auto f3 = [](auto x){ return x + 5 };
    // generalized lambda expression
    // closure type's operator() function is a member template
    
    // -----------------------------------------------------
  }
*/

/*
  struct Myclass{
    constexpr Myclass(int x, int y) : m_x{ x }, m_y{ y } {}
    int m_x, m_y;
  };

  int main()
  {
    constexpr Myclass m1(10, 20);

    constexpr auto[x, y] = m1;  // syntax error
    // error: structured binding declaration cannot be 'constexpr'
  }
*/

/*
  #include <utility>        // std::move
  #include <type_traits>    // std::integral_constant
  #include <tuple>          // std::tuple_size, std::tuple_element

  class Person{
  public:
    Person(int id, std::string name, double wage) :
      m_id{ id }, m_name{ std::move(name) }, m_wage{ wage } {}

    // getter functions for global get function template
    int get_id() const { return m_id; }
    std::string get_name() const { return m_name; }
    double get_wage() const { return m_wage; }
  private:
    int m_id;
    std::string m_name;
    double m_wage;
  };

  namespace std {
    // -----------------------------------------------------

    template <> // explicit(full) specialization
    struct tuple_size<Person> : std::integral_constant<size_t, 3> {};

    // template <>
    // struct tuple_size<Person>{
    //   constexpr static std::size_t value = 3;
    // };

    // -----------------------------------------------------

    template <> 
    struct tuple_element<0, Person> { using type = int; };

    template <> 
    struct tuple_element<1, Person> { using type = string; };

    template <>
    struct tuple_element<2, Person> { using type = double; };

    // -----------------------------------------------------
  }

  template<std::size_t N>
  auto get(const Person& p)
  {
    if constexpr (N == 0)
      return p.get_id();
    else if constexpr (N == 1)
      return p.get_name();
    else if constexpr (N == 2)
      return p.get_wage();
    else
      static_assert(N < 3, "Invalid index");
  }

  int main()
  {
    std::tuple_size<Person>::value;   // 3

    Person p1{ 348'975, "hello world", 3.14 };

    auto [id, name, wage] = p1;

    std::cout << "id = " << id << "\n";     
    // output -> id = 348975
    std::cout << "name = " << name << "\n"; 
    // output -> name = hello world
    std::cout << "wage = " << wage << "\n"; 
    // output -> wage = 3.14
  }
*/

/*
  #include <string>
  #include <utility>  // std::move , tuple-like API

  class Customer {
  private:
    std::string m_first;
    std::string m_last;
    long m_val;
  public:
    Customer(std::string f, std::string l, long v) :
      m_first{ std::move(f) }, m_last{ std::move(l) }, m_val{ v } {}

    const std::string& firstname() const { return m_first; }
    std::string& firstname() { return m_first; }

    const std::string& lastname() const { return m_last; }
    std::string& lastname() { return m_last; }

    long value() const { return m_val; }
    long& value() { return m_val; }
  };

  // -----------------------------------------------------

  template <>   // explicit(full) specialization
  struct std::tuple_size<Customer> {
    static constexpr int value = 3;
  };

  // -----------------------------------------------------

  template <>   // explicit(full) specialization
  struct std::tuple_element<2, Customer> {
    using type = long;
  };

  template <std::size_t Idx>  // partial specialization
  struct std::tuple_element<Idx, Customer> {
    using type = std::string;
  };

  // -----------------------------------------------------

  template <std::size_t I> 
  decltype(auto) get(Customer& c)
  {
    static_assert(I < 3, "Invalid index");

    if constexpr (I == 0)
      return c.firstname();
    else if constexpr (I == 1)
      return c.lastname();
    else
      return c.value();
  }

  template <std::size_t I> 
  decltype(auto) get(const Customer& c)
  {
    static_assert(I < 3, "Invalid index");

    if constexpr (I == 0)
      return c.firstname();
    else if constexpr (I == 1)
      return c.lastname();
    else
      return c.value();
  }

  template <std::size_t I> 
  decltype(auto) get(Customer&& c)
  {
    static_assert(I < 3, "Invalid index");

    if constexpr (I == 0)
      return std::move(c.firstname());
    else if constexpr (I == 1)
      return std::move(c.lastname());
    else
      return c.value();

    // check decltype(auto) return type in 
    // <Perfect Forwarding> chapter
  }
*/