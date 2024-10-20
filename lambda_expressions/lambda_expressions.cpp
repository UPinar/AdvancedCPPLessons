#include <iostream>

/*
                      ======================
                      | lambda expressions |
                      ======================
*/

/*
  - bir lambda ifadesi(lambda expression), derleyicinin oluşturduğu
    bir sınıf türünden bir geçici nesnedir(temporary object).,
    PRValue expression
  - lambda ifadesi karşılığı derleyicinin oluşturduğu sınıf türüne 
    closure type denir.
  - lamda ifadesi karşılığı derleyicinin oluşturduğu geçici nesneye
    closure object denir.
*/

/*
  int g_x = 99;

  auto fx = [=] { return g_x + 1; };
  auto fy = [g_x = g_x] { return g_x + 1; };

  int main()
  {
    g_x = 500;

    std::cout << fx() << '\n';
    std::cout << fy() << '\n';
  }
*/

/*
  auto x = []{ static int x{}; return ++x; };
  auto y = []{ static int x{}; return ++x; };

  int main()
  {
    std::cout << x() << x() << x() << '\n';
    std::cout << y() << y() << y() << '\n';
  }
*/

/*
  int main()
  {
    auto x = []{ static int x{}; return ++x; };
    decltype(x) y;
    decltype(x) z;

    std::cout << y() << y() << y() << '\n';
    std::cout << z() << z() << z() << '\n';
  }
*/

/*
  struct Mystruct {

    // -----------------------------------------------------

    // -------- SCENARIO 1 --------
    void foo_x() { ++m_x; }       // VALID
    // changing non-const data member inside non-const member function

    // -------- SCENARIO 2 --------
    void foo_x() const { ++m_x; } // syntax error
    // error: increment of member 'Mystruct::m_x' in read-only object

    // trying to change non-const data member inside
    // Mystruct's const member function 

    // -----------------------------------------------------

    // -------- SCENARIO 3 --------
    void foo_y() { ++m_y; }       // syntax error
    // error: increment of read-only member 'Mystruct::m_y'

    // -------- SCENARIO 4 --------
    void foo_y() const { ++m_y; } // syntax error
    // error: increment of member 'Mystruct::m_y' in read-only object

    // -----------------------------------------------------

    int m_x{};
    const int m_y{};
  };

  int main()
  {
    const int x = 10;

    // -----------------------------------------------------

    auto f = [x]() mutable { ++x; }; // syntax error
    // error: increment of read-only variable 'x'

    // "x" identifier is being used in capture clause. (copy capture)
    // because of copy capture, 
    // compiler generated closure type's data member type 
    // will be `const int`

    // Closure type has a `mutable` keyword.
    // So lambda function is a non-const member function

    // syntax error because non-const member function is 
    // trying to change const data member

    // <---- Check SCENARIO 3 above ---->

    // -----------------------------------------------------

    auto g = [x = x](){ ++x; };     // syntax error

    // lambda init capture 
    // compiler generated closure type's data member type
    // will be `int` (constness will be removed)
    // data member will be initialized with the value of "x"

    // Because of no `mutable` keyword, 
    // lambda function is a const member function

    // syntax error because const member function is
    // trying to change non-const data member

    // <---- Check SCENARIO 2 above ---->

    // -----------------------------------------------------

    auto h = [x = x]() mutable { ++x; };  // VALID

    // lambda init capture
    // compiler generated closure type's data member type
    // will be `int` (constness will be removed)
    // data member will be initialized with the value of "x"

    // Because of `mutable` keyword,
    // lambda function is a non-const member function

    // VALID because non-const member function is trying to change
    // non-const data member

    // <---- Check SCENARIO 1 above ---->

    // -----------------------------------------------------
  }
*/

/*
  int main()
  {
    int x = 4;
    auto y = [x = x + 1, &ref = x](){
      ref += 2;
      return x * x;
    }();

    std::cout << "x = " << x << '\n';   // output -> x = 6
    std::cout << "y = " << y << '\n';   // output -> y = 25
  }
*/

/*
  int g_x = 5;

  int main()
  {
    auto f = [](int x = ++g_x){ return x * x; };

    auto x = f();
    auto y = f();

    std::cout << "x = " << x << '\n';       // output -> x = 36
    std::cout << "y = " << y << '\n';       // output -> y = 49
    std::cout << "g_x = " << g_x << '\n';   // output -> g_x = 7
  }
*/

/*
  #include <type_traits>  // std::is_same

  double i{};

  int main()
  {
    std::boolalpha(std::cout);

    auto f = [i = 0]() -> decltype(i) { return 1; }();

    std::cout << std::is_same_v<decltype(f), int> << '\n'; 
    // output -> false
  }
*/

/*
  <---- check lambda_syntax.png ---->
*/

/*
  stateless lambda : if lambda is not capturing anything  
  stateful lambda : if lambda is capturing something
*/

/*
  int main()
  {
    // -----------------------------------------------------

    [](){}; 
    // "[](){}" is PRValue expression
    // compiler generated closure type temporary object
    // closure object has an operator() member function

    // -----------------------------------------------------

    []{}; 
    // because of operator() member function does not have any 
    // parameter variable, parentheses are not-manadatory
    // "[]{}" is PRValue expression

    // -----------------------------------------------------

    [](){}();
    // "[](){}" closure objects operator() member function is called
    
    // -----------------------------------------------------
  }
*/

/*
  template <typename T>
  void func(T);

  int main()
  {
    // -----------------------------------------------------
    
    [](int x){ return x * 6; };
    // "[](int x){ return x * 6; }" is PRValue expression
    // it is a temporary object, not a function call.

    // -----------------------------------------------------

    [](int x){ return x * 6; }(10);
    // immidiately invoked function expression

    // -----------------------------------------------------

    auto f = [](int x){ return x * 6; };
    // "[](int x){ return x * 6; }" is PRValue expression
    // mandatory copy ellision will be applied

    // if this lambda expression's operator call member function
    // will be called more than once, it can be assigned to a variable.

    f(10);

    // -----------------------------------------------------

    func([](int x){ return x * 6; });
    // Template Argument Deduction will be done for T type

    // compiler written "func" function's specialization 
    // have the same type of closure type

    // -----------------------------------------------------
  }
*/

/*
  int main()
  {
    auto f = []{};
    // "f" is a closure object

    decltype(f);  
    // "decltype(f)" is a closure type 
  }
*/

/*
  #include <type_traits>  // std::is_same

  int main()
  {
    std::boolalpha(std::cout);

    // -----------------------------------------------------

    auto f1 = [](int x) { return x * 5; };
    auto f2 = [](int x) { return x * 5; };

    constexpr bool b1 = std::is_same_v<decltype(f1), decltype(f2)>;
    // b1 -> false

    // compiler will generate different closure types 
    // for each lambda expression even if they are identical.

    // -----------------------------------------------------

    auto f3 = f1;

    constexpr bool b2 = std::is_same_v<decltype(f1), decltype(f3)>;
    // b2 -> true

    // -----------------------------------------------------
  }
*/

/*
  lambda function : 
    compiler generated closure type's operator() member function

  - lambda function is `const` member function by default
  - if there is no return statement in lambda function
    it will be a void function(return type is void)

*/

/*
  // Mental Model for lambda expressions

  class compiler_generated_closure_type_f1 {
  public:
    void operator()(int x) const {}
  };

  class compiler_generated_closure_type_f2 {
  public:
    int operator()(int x) const { return x * 5; }
  };

  class compiler_generated_closure_type_f3 {
  public:
    int operator()(int x) { return x * 5; }
  };

  int main()
  {
    auto f1 = [](int x) {};
    auto f2 = [](int x) { return x * 5; };
    auto f3 = [](int x) mutable { return x * 5; };
  }
*/

/*
  int main()
  {
    // -----------------------------------------------------

    auto f1 = [](int x){
      if (x > 10) 
        return 3;
      else 
        return 5.2;
    };   // syntax error
    //  error: inconsistent types 'int' and 'double' 
    // deduced for lambda return type

    // -----------------------------------------------------

    auto f2 = [](int x) -> double {
      if (x > 10) 
        return 3;
      else 
        return 5.2;
    };  // VALID (using trailing return type)

    // -----------------------------------------------------
  }
*/

/*
  int main()
  {
    auto f1 = []() {
      static int x = 10;
      ++x;
      std::cout << "x = " << x << '\n';
    };

    auto f2 = []() {
      static int x = 10;
      ++x;
      std::cout << "x = " << x << '\n';
    };

    f1();  // output -> x = 11
    f1();  // output -> x = 12

    f2();  // output -> x = 11
    f2();  // output -> x = 12
  }
*/

/*
  int main()
  {
    auto f = [](int x = 10){
      std::cout << "x = " << x << '\n';
    };

    f(35);  // output -> x = 35
    f();    // output -> x = 10
  }
*/

/*
  class compiler_generated_closure_type_f1 {
  public:
    template <typename T>
    void operator()(T x) const {}
  };

  class compiler_generated_closure_type_f2 {
  public:
    template <typename T, typename U>
    void operator()(T x, U y) const {}
  };

  int main()
  {
    auto f1 = [](auto x){};
    auto f2 = [](auto x, auto y){};
  }
*/

/*
  - static storage duration objects can be 
    directly used in lambda expressions.
    No need to capture them.
    
*/

/*
  // static storage duration objects can be directly used 
  // in lambda expressions, no need to capture them.

  int g_x = 10;

  int main()
  {
    []{ std::cout << "g_x = " << g_x << '\n'; }();
    // output -> g_x = 10

    [g_x] {};  // syntax error
    // warning: capture of variable 'g_x' 
    // with non-automatic storage duration
  }
*/

/*
  int main()
  {
    // -----------------------------------------------------

    int val = 5;
    auto f1 = [](int a){ return a * val; }; // syntax error
    // error: 'val' is not captured

    // -----------------------------------------------------

    const int val2 = 5;
    auto f2 = [](int a){ return a * val2; };      // VALID
    auto f3 = [val2](int a){ return a * val2; };  // VALID
    // constants can be captured by default
    // capturing a constant is not a syntax error

    // -----------------------------------------------------
  }
*/

/*
  int main()
  {
    int x, y, z, t;

    auto f1 = [x, y, z, t](){};
    // x, y, z, t are copy captured 
    // closure type will have 4 data members which are  
    // initialized with x, y, z, t

    auto f2 = [=](){};
    // all variables which are visible will be copy captured
    // closure type will have 4 data members which are
    // initialized with x, y, z, t
  }
*/

/*
  #include <type_traits>  // std::is_same

  int main()
  {
    using namespace std;

    int arr[5]{};

    // no array decay for array type in capture clause
    auto f = [arr](){
      constexpr bool b1 = is_same_v<decltype(arr), int*>;
      // b1 -> false

      constexpr bool b2 = is_same_v<decltype(arr), int[5]>;
      // b2 -> true
    };
  }
*/

/*
  #include <type_traits>  // std::is_same

  int main()
  {
    using namespace std;

    int arr[5]{};

    // lambda init capture
    // array decay will be happened
    auto f = [arr = arr](){
      constexpr bool b1 = is_same_v<decltype(arr), int*>;
      // b1 -> true

      constexpr bool b2 = is_same_v<decltype(arr), int[5]>;
      // b2 -> false
    };
  }
*/

/*
  int main()
  {
    int x = 50;

    // -----------------------------------------------------

    auto f1 = [x] { x = 67; }; // syntax error
    // operator() member function is const member function

    // trying to change non-static data member 
    // in const member function

    // -----------------------------------------------------

    auto f2 = [x]() mutable { x = 67; }; // VALID

    // -----------------------------------------------------
  }
*/

/*
  class Myclass {
  public:
    void func() const 
    {
      int ival{ 35 };
      mp_ip = &ival; // syntax error
      // error: assignment of member 'Myclass::mp_ip' in read-only object
      // changing non-static data member inside const member function

      *mp_ip = ival;  // VALID
      // not changing non-static data member, changing the object 
      // that non-static data member is pointing to.

      mr_i = ival;    // VALID
    }
  private:
    int* mp_ip;
    int& mr_i;
  };

  int main()
  {
    int x = 10;

    // capturing with reference semantics
    auto f = [&x]() {
      ++x;
      std::cout << "x = " << x << '\n';
    }; 

    x = 999;
    f();  // output -> x = 1000
  }
*/

/*
  int main()
  {
    int x{}, y{}, z{}, t{};

    auto f1 = [x, y, z, t](){};   
    // all copy captured

    auto f2 = [&x, &y, &z](){};   
    // x, y, z are reference captured

    auto f3 = [&x, y, z](){};
    // x is reference captured, y, z are copy captured

    auto f4 = [=](){};
    // all variables are copy captured

    auto f5 = [=, &z](){};
    // z is reference captured, others are copy captured

    auto f6 = [=, &z, &t](){};
    // z, t are reference captured, others are copy captured

    auto f7 = [&](){};
    // all variables are reference captured

    auto f8 = [&, z](){};
    // z is copy captured, others are reference captured

    auto f9 = [&, z, t](){};
    // z, t are copy captured, others are reference captured
  }
*/

/*
  auto foo(int i)
  {
    auto f = [&](int x) { return x + i; };
    return f;
  }

  int main()
  {
    auto fn = foo(10);
    auto val = fn(290);
    std::cout << val << '\n'; // undefined behavior(UB)

    // using "fn" will be a run-time error because
    // foo function's local variable is captured by reference
    // and because its scopes ends, captured variable is destroyed.
    // so, using "fn" will be a dangling reference.
  }
*/

/*
  #include <vector>

  int main()
  {
    using namespace std;

    vector<int> ivec(10'000, 1);

    auto f1 = [ivec](){};
    // vector is being copied (copy captured)

    auto f2 = [&ivec](){};
    // vector is being captured by reference
  }
*/

/*
  #include <memory>
  #include <string>

  int main()
  {
    using namespace std;

    auto uptr = make_unique<std::string>(10'000, 'A');

    auto f1 = [uptr](){}; // syntax error
    // error: use of deleted function 
    // 'std::unique_ptr<_Tp, _Dp>::unique_ptr(
    //                    const std::unique_ptr<_Tp, _Dp>&)
    // std::unique_ptr is move-only type
  }
*/

/*
  #include <memory>
  #include <string>
  #include <utility>  // std::move

  int main()
  {
    using namespace std;

    auto uptr = make_unique<std::string>(10'000, 'A');

    // -----------------------------------------------------

    auto f1 = [&uptr](){};
    f1();

    cout << (uptr ? "uptr != nullptr" : "uptr = nullptr") << '\n';
    // output -> uptr != nullptr

    // closure object's non-static data member is a reference
    // to the unique_ptr object

    // -----------------------------------------------------

    // what if we want uptr to be moved to 
    // lambda expression's non-static data member?

    auto f2 = [uptr = std::move(uptr)](){};
    cout << (uptr ? "uptr != nullptr" : "uptr = nullptr") << '\n';
    // output -> uptr = nullptr

    // lambda init capture
    // closure object's non-static data member is a unique_ptr object
    // which is moved from the uptr object

    // -----------------------------------------------------
  }
*/

/*
  int main()
  {
    const int cival = 66;

    // -----------------------------------------------------

    auto f1 = [cival]() mutable{ ++cival; };  // syntax error
    // closure object's non-static data member is const.
    // lambda function's constness is does not mather.

    // -----------------------------------------------------

    auto f2 = [cival = cival]() mutable{ return ++cival };  // VALID
    // lambda init capture
    // closure object's non-static data member is int type
    // (constness is removed)
    // which is initialized with the value of cival
    // because of `mutable` keyword lambda function is non-const
    // so it is valid to change non-const data member
    // inside non-const member function

    // -----------------------------------------------------
  }
*/

/*
  struct Mystruct {
    // non-static member function
    void foo()
    {
      int local_var = 5;

      // -----------------------------------------------------

      auto f1 = [](int val) { return val * local_var; }; // syntax error
      // error: 'local_var' is not captured

      // -----------------------------------------------------

      auto f2 = [local_var](int val) { return val * local_var; }; // VALID

      // -----------------------------------------------------
    }
  };
*/

/*
  struct Mystruct {
    int m_x{}, m_y{};

    // non-static member function
    void foo()
    {
      // -----------------------------------------------------

      auto f3 = [](int val){ return val * m_x; };     // syntax error
      // error: 'this' was not captured for this lambda function
      // error: invalid use of non-static data member 'Mystruct::m_x'

      // -----------------------------------------------------

      auto f4 = [m_x](int val){ return val * m_x; };  // syntax error
      // error: capture of non-variable 'Mystruct::m_x'
      // error: 'this' was not captured for this lambda function
      // error: invalid use of non-static data member 'Mystruct::m_x'

      // -----------------------------------------------------

      auto f5 = [this](int val){ return val * m_x; };   // VALID
      // this pointer is copy captured

      // -----------------------------------------------------

      auto f6 = [=](int val){ return val * m_x; }; 
      // VALID (deprecated in C++20)

      // -----------------------------------------------------

      auto f7 = [&](int val){ return val * m_x; };
      // VALID (not deprecated same as copy captured)
      // we can now change Mystruct's data members inside lambda function

      // -----------------------------------------------------
    }
  };
*/

/*
  struct Mystruct {
    auto foo()
    {
      auto f = [&](int val){};
      return f;
    }
  };

  // when Mystruct object's life ends, lambda fucntions will 
  // be a dangling reference because of this pointer is captured
*/

/*
  struct Mystruct {
    int m_x{};

    auto func()
    {
      auto fn = [copy_this = *this](){
        copy_this.m_x;  // reaching copy of *this's m_x data member
      };  
      // lambda init capture

      return fn;  // no dangling reference
    }
  };
*/

/*
            <---- check capture_this_in_classes.png ---->
*/

/*
  int main()
  {
    auto f = [](auto&&... args){};
  }
*/

/*
  int main()
  {
    // -----------------------------------------------------

    auto f1 = [](int x){ return x * 5; }; 
    // if there is no problem this lambda expression 
    // being a constexpr function
    // it will be a constexpr function.

    constexpr auto val = f1(10);   // val -> 50
    
    // -----------------------------------------------------

    auto f2 = [](int x){ 
      static int s_count{};
      ++s_count;

      return x * 5;
    };

    constexpr auto val2 = f2(10);  // syntax error
    // error: 's_count' defined 'static' in 'constexpr' context

    // -----------------------------------------------------
  }
*/

/*
  #include <array>

  int main()
  {
    // -----------------------------------------------------

    auto fsquare = [](auto val) { return val * val; };

    std::array<int, fsquare(5)> arr_1;
    std::cout << arr_1.size() << '\n'; // output -> 25

    // -----------------------------------------------------

    auto f1 = [](int x) { 
      static int s_count{};
      ++s_count;
      return x * s_count; 
    };

    //std::array<int, f1(10)> arr_2;  // syntax error
    // error: 's_count' defined 'static' in 'constexpr' context

    std::cout << f1(20) << '\n';  // output -> 20
    std::cout << f1(20) << '\n';  // output -> 40

    // -----------------------------------------------------

    auto f2 = [](int x) constexpr { 
      static int s_count{}; // syntax error
      ++s_count;
      return x * s_count; 
    };

    // -----------------------------------------------------
  }
*/

/*
  int main()
  {
    using namespace std;
    boolalpha(cout);

    auto f1 = [](int x) { return x * x; };
    constexpr auto b1 = noexcept(f1(344));   // b1 -> false

    auto f2 = [](int x) noexcept { return x * x; };
    constexpr auto b2 = noexcept(f2(344));   // b2 -> true
  }
*/

/*
  - bir lambda expression std::function ile sarmalandığında,
    std::function fazladan bellek alanı kullanır.
    std:: function bazı durumlarda dinamik bellek alanı kullanıyor.

  - std::function kullanımının zorunlu olmadığı temalarda 
    std::function kullanılmamalıdır.
*/

/*
  #include <functional>  // std::function
  #include <cstddef>     // std::size_t

  void* operator new(std::size_t size)
  {
    std::cout << "operator new called for the size of : " 
              << size << '\n';

    if (size == 0)
      ++size;

    if (void* ptr = std::malloc(size))
      return ptr;

    throw std::bad_alloc{};
  }

  class Myclass {
    unsigned char buffer[512]{};
  };

  int main()
  {
    using namespace std;

    // -----------------------------------------------------

    auto fn = [](double d){ return d * d + .3; };

    cout << fn(5.4723) << '\n';   // output -> 30.2461

    cout  << "sizeof(decltype(fn)) = " 
          << sizeof(decltype(fn)) << '\n';  
    // output -> sizeof(decltype(fn)) = 1

    // -----------------------------------------------------

    std::function f1 = fn;   // CTAD
    // std::function<double(double)> f1 = fn;

    cout << f1(5.4723) << '\n';  // output -> 30.2461

    cout  << "sizeof(f1) = " 
          << sizeof(f1) << '\n';
    // output -> sizeof(f1) = 32

    // -----------------------------------------------------

    Myclass m;
    auto fn2 = [m](double d){ return d * d + .3; };

    cout  << "sizeof(decltype(fn2)) = " 
          << sizeof(decltype(fn2)) << '\n';  
    // output -> sizeof(decltype(fn2)) = 512

    std::function f2 = fn2;
    // output -> operator new called for the size of : 512

    cout  << "sizeof(f2) = " 
          << sizeof(f2) << '\n';
    // output -> sizeof(f2) = 32

    // -----------------------------------------------------
  }
*/

/*
  #include <iterator>  // std::size

  int main()
  {
    // Q1. Is there any scenario that std::size(con) is being used
    // instead of con.size().
    // Answer : for C-style arrays, std::size(con) is being used.

    [](const auto& con){ return std::size(con); };
    [](const auto& con){ return con.size(); };
  }
*/

/*
  #include <iterator>  // std::size
  #include <vector>
  #include <list>
  #include <deque>

  int main()
  {
    using namespace std;

    auto get_size = [](const auto& con){ return std::size(con); };

    vector<int> ivec(30);
    list dlist { 34. , 45. , 56. };
    int i_arr[20]{};

    cout << get_size(ivec) << '\n';   // output -> 30
    cout << get_size(dlist) << '\n';  // output -> 3
    cout << get_size(i_arr) << '\n';  // output -> 20
  }
*/

/*
  #include <vector>
  #include <string>
  #include <utility>    // std::pair
  #include <algorithm>  // std::sort

  #include "../nutility.h"

  int main()
  {
    using namespace std;

    vector<pair<string, string>> pvec;

    for(int i = 0; i < 1000; ++i)
      pvec.push_back(pair{ rname(), rfname() });

    sort( pvec.begin(), pvec.end(), 
        []( const pair<string, string>& p1, 
            const pair<string, string>& p2){
          return pair{p1.second, p1.first} < pair{p2.second, p2.first};
        });

    for (const auto& [name, surname] : pvec)
      cout << name << surname << '\n';

    // using `auto` inside lambda expression
    // for increasing code readability
    // and decreasing code verbosity

    sort( pvec.begin(), pvec.end(), 
        []( const auto& p1, const auto& p2){
          return pair{p1.second, p1.first} < pair{p2.second, p2.first};
        });
  }
*/

/*
  #include <memory>     // std::shared_ptr, std::make_shared
  #include <string>
  #include <algorithm>  // std::sort, std::for_each
  #include <vector>

  using namespace std;

  void f1(vector<shared_ptr<string>>& svec)
  {
    sort(begin(svec), end(svec), 
      []( const shared_ptr<string>& sp1, 
          const shared_ptr<string>& sp2){
        return *sp1 < *sp2;
      });

    for_each(begin(svec), end(svec), 
      [](const shared_ptr<string>& sp){
        cout << *sp << ' ';
      });

    std::cout << "\n";
  }

  void f2(vector<shared_ptr<string>>& svec)
  {
    sort(begin(svec), end(svec), 
      []( const auto& sp1, const auto& sp2){
        return *sp1 < *sp2;
      });

    for_each(begin(svec), end(svec), 
      [](const auto& sp){
        cout << *sp << ' ';
      });

    std::cout << "\n";
  }

  int main()
  {
    vector<shared_ptr<string>> svec;

    svec.emplace_back(new string{ "hello" });
    svec.push_back(make_shared<string>("world"));
    svec.emplace_back(new string{ "universe" });

    f1(svec); // output -> hello universe world
    f2(svec); // output -> hello universe world
  }
*/

/*
  #include <string>
  #include <utility>  // std::forward

  void foo(std::string& s)
  {
    std::cout << "foo(std::string&) - L value reference\n"; 
  }

  void foo(const std::string& s)
  {
    std::cout << "foo(const std::string&) - const L value reference\n"; 
  }

  void foo(std::string&& s)
  {
    std::cout << "foo(std::string&&) - R value reference\n"; 
  }

  int main()
  {
    auto fn = [](auto&& s){
      foo(std::forward<decltype(s)>(s));
    };

    std::string str{ "hello" };
    const std::string cstr{ "world" };

    fn(str);  
    // output -> foo(std::string&) - L value reference

    fn(cstr);
    // output -> foo(const std::string&) - const L value reference

    fn(std::move(str));
    // output -> foo(std::string&&) - R value reference
  }
*/

/*
  #include <cstdio>     // std::putchar
  #include <utility>    // std::forward
  #include <initializer_list>

  template <typename ...Args>
  void print(Args&&... args)
  {
    std::initializer_list<int>{
          ((std::cout << std::forward<Args>(args) << ' '), 0)...};

    // because of "," comma operator is creating a sequence point
    // its left operand is evaluated before its right operand
    // the value of the expression generated with comma operator 
    // is the value of its right operand so it is 0 
    // so temporary initializer_list is created with 0 values
  }

  int main()
  {
    // -----------------------------------------------------

    print(10, "hello", 3.14, std::string{ "world" });
    // output -> 10 hello 3.14 world

    putchar('\n');

    // -----------------------------------------------------

    auto fn  = [](auto&&... args){ 
      print(std::forward<decltype(args)>(args)...); };

    fn(23, 7.6 , "lambda", std::string{ "expression" });
    // output -> 23 7.6 lambda expression

    // -----------------------------------------------------
  }
*/

/*
  #include <memory> // std::make_unique
  #include <string>

  int main()
  {
    using namespace std;

    auto up = make_unique<string>("hello");

    auto f = [up = move(up)]{ return *up + " world"; };
    // lambda init capture
    
    cout << f() << '\n';  // output -> hello world

    if(up)
      cout << "up != nullptr\n";
    else
      cout << "up = nullptr\n";
    // output -> up = nullptr
  }
*/

/*
  struct Mystruct {
    auto foo() const
    {
      return [m_str = m_str]{ std::cout << m_str << '\n'; };
      // lambda init capture
    }
    std::string m_str;
  };

  int main()
  {
    const auto f1 = Mystruct{ "hello" }.foo();
    const auto f2 = Mystruct{ "world" }.foo();
    f1();  // output -> hello
    f2();  // output -> world
  }
*/

/*
  #include <vector>
  #include <string>
  #include <algorithm>  // std::find_if

  int main()
  {
    using namespace std;

    vector<string> svec(1000);
    string str{ "hello" };

    // -----------------------------------------------------

    auto iter = find_if(svec.begin(), svec.end(),
      [&str](const string& s){
        return s == str + " world";
      });

    // because of svec is a vector of 1000 elements
    // (str + " world") expression --> will be calculated 1000 times

    // -----------------------------------------------------

    auto iter = find_if(svec.begin(), svec.end(),
    [str = str + " world"](const string& s){
      return s == str;
    });

    // (str + " world") expression --> will be calculated 1 time

    // -----------------------------------------------------
  }
*/

/*
  #include <type_traits>

  template <typename F>
  void func(F&& f) // F is a callable type
  {
    if constexpr (std::is_nothrow_invocable_v<F, int>)
      std::cout << "no throw\n";
    else
      std::cout << "may throw\n";
  }

  int main()
  {
    auto fn1 = [](int x) { return x * 5; };
    func(fn1); // output -> may throw

    auto fn2 = [](int x) noexcept { return x * 5; };
    func(fn2); // output -> no throw
  }
*/

/*
  class compiler_generated_closure_type {
  public:
    int operator()(int x) const { return x * 5; }

    using ftype = int(*)(int);
    operator ftype() const;
    // stateless lambda expression is convertible to function pointer
  };

  int main()
  {
    // -----------------------------------------------------

    auto fn = [](int x){ return x * 5; }; 
    // stateless lambda expression

    int(*fp1)(int) = fn;
    // "fp1" is a function pointer. Its data type is int(*)(int)

    std::cout << fp1(10) << '\n';  // output -> 50

    // -----------------------------------------------------

    int ival{};
    auto fn2 = [ival](int x){ return x * ival; };

    int(*fp2)(int) = fn2; // syntax error
    // error: cannot convert 'main()::<lambda(int)>'  
    // to 'int (*)(int)' in initialization
    // no implicit conversion from closure type to function pointer

    // -----------------------------------------------------
  }
*/

/*
  // If we have a C API to use.

  void C_function(int(*fp)(int))
  {
    int ival = fp(20);
  }

  int main()
  {
    C_function([](int x){ return x * 7; }); // VALID
    // "[](int x){ return x * 7; }" is a stateless lambda expression
    // implicit conversion from closure type to function pointer
  }
*/

/*
  int foo(int);

  int main()
  {
    // -----------------------------------------------------

    char c = 'A';

    +c;
    // "+c" is RValue expression, 
    // its data type is int because of the integral promotion.
    // expression that generated with sign operator is RValue expression

    // -----------------------------------------------------

    int x = 10;
    int* ptr = &x;

    +ptr;
    // "+ptr" is RValue expression, its data type is int*

    // -----------------------------------------------------

    int(*fp)(int) = foo;

    auto val = +fp;
    // "+fp" is RValue expression, its data type is int(*)(int)

    // -----------------------------------------------------
  }
*/

/*
  class Myclass {
  public:
    operator int()const 
    { 
      std::cout << "Myclass::operator int() called"; 
    }
  };

  int main()
  {
    // -----------------------------------------------------

    Myclass mx;

    +mx;  
    // output -> Myclass::operator int() called
    +(mx.operator int()); 
    // output -> Myclass::operator int() called

    // -----------------------------------------------------

    [](int x){ return x * 5; }; // stateless lambda expression
    // "[](int x){ return x * 5; }" can be converted to function pointer

    +[](int x){ return x * 5; };
    // implicit conversion from closure type to function pointer
    // is done for stateless lambda expression to be an operand of 
    // sign operator.

    // -----------------------------------------------------
  }
*/

/*
                  -------------------------
                  | positive lambda idiom |
                  -------------------------
*/

/*
  // Sometimes we want lambda expression 
  // deduce to a function pointer

  int main()
  {
    auto f1 = [](int x){ return x * 5; };
    // f1's data type is closure type

    auto f2 = +[](int x){ return x * 5; };
    // f2's data type is int(*)(int)
  }
*/

/*
  #include <type_traits>  // std::is_same

  template <typename T>
  void func(T x)
  {
    if constexpr (std::is_same_v<T, int(*)(int)>)
      std::cout << "function pointer type\n";
    else
      std::cout << "closure type\n";
  }

  int main()
  {
    func([](int x){ return x * 5; }); 
    // output -> closure type

    func(+[](int x){ return x * 5; }); 
    // output -> function pointer type
  }
*/

/*
  #include <map>
  #include <string>
  #include <algorithm>  // std::for_each
  #include <utility>    // std::pair

  int main()
  {
    std::map<std::string, int> numbers{
      { "one", 1 },
      { "two", 2 },
      { "three", 3 },
    };

    for (auto it = numbers.cbegin(); it != numbers.cend(); ++it)
      std::cout << &it->first << ", " << &it->second << '\n';
    // output ->
    //  0x237dde0db40, 0x237dde0db60
    //  0x237dde0d780, 0x237dde0d7a0
    //  0x237dde0d500, 0x237dde0d520

    // ----------------------------------------------

    // pairs are copied, copy semantics 
    // because of std::pair<std::string, int> is used
    // instead of std::pair<const std::string, int>

    std::for_each(std::begin(numbers), std::end(numbers),
      [](const std::pair<std::string, int>& entry){
        std::cout << &entry.first << ", " << &entry.second << " : "
                  << entry.first << ", " << entry.second << '\n';
      });
    // output ->
    //  0x7d159ffc10, 0x7d159ffc30 : one, 1
    //  0x7d159ffc10, 0x7d159ffc30 : three, 3
    //  0x7d159ffc10, 0x7d159ffc30 : two, 2

    // ----------------------------------------------

    // reference semantics 
    std::for_each(std::begin(numbers), std::end(numbers),
      [](const std::pair<const std::string, int>& entry){
        std::cout << &entry.first << ", " << &entry.second << " : "
                  << entry.first << ", " << entry.second << '\n';
      });

    // output ->
    //  0x237dde0db40, 0x237dde0db60 : one, 1
    //  0x237dde0d780, 0x237dde0d7a0 : two, 2
    //  0x237dde0d500, 0x237dde0d520 : three, 3

    // ----------------------------------------------

    // generic lambda is being used, reference semantics
    std::for_each(std::begin(numbers), std::end(numbers),
      [](const auto& entry){
        std::cout << &entry.first << ", " << &entry.second << " : "
                  << entry.first << ", " << entry.second << '\n';
      });
      
    // output ->
    //  0x237dde0db40, 0x237dde0db60 : one, 1
    //  0x237dde0d780, 0x237dde0d7a0 : two, 2
    //  0x237dde0d500, 0x237dde0d520 : three, 3

    // ----------------------------------------------
  }
*/

/*
        --------------------------------------------------
        | Immidiately Invoked Function Expression (IIFE) |
        --------------------------------------------------
*/

/*
  const int cival = [&]{
    int i = default_value;
    
    if (condition_is_true)
      i = other_value;

    return i;
  }();

  initialization of a const variable with 
  immidiately invoked lambda expression

  - global(free) function also can be use for this purpose
*/

/*
  int main()
  {
    int a = 34, b = 345;

    const int cival = [&]{  
      int val = a * b;
      ++val;

      return val;
    }();

    std::cout << cival << '\n';  // output -> 11731
  }
*/

/*
  struct Mysturct {
    int m_x;

    Mysturct(int x) : m_x( [&]{
      // complex initialization of m_x
      // with immidiately invoked lambda expression
    }() ) {}
  };
*/

/*
  #include <functional>  // std::invoke

  int main()
  {
    int x = 6, y = 7;

    const int cival = std::invoke([&]{ 
      int val = x * y;
      ++val;
      return val;
    });
  }
*/

/*
  class Myclass {
  public:
    Myclass() 
    {
      static auto once_var = []{
        std::cout << "this code will only run once\n";
        return 0;
      }();
    }
  };

  int main()
  {
    Myclass m1; // output -> this code will only run once
    Myclass m2;
    Myclass m3;
  }
*/

/*
                -----------------------------
                | overload resolution idiom |
                -----------------------------
*/

/*
  #include <vector>
  #include <algorithm>  // std::transform

  int foo(int);
  int foo(double);
  int foo(long);

  int main()
  {
    using namespace std;

    vector<int> ivec_1(100, 5);
    vector<int> ivec_2(100);

    // -----------------------------------------------------

    transform(ivec_1.begin(), ivec_1.end(), 
              ivec_2.begin(), foo);               // syntax error

    // error: no matching function for call to 
    // 'transform(std::vector<int>::iterator, 
    //            std::vector<int>::iterator, 
    //            std::vector<int>::iterator, 
    //            <unresolved overloaded function type>)'

    -----------------------------------------------------

    transform(ivec_1.begin(), ivec_1.end(), 
              ivec_2.begin(), 
              static_cast<int(*)(int)>(foo));     // VALID  

    // -----------------------------------------------------

    transform(ivec_1.begin(), ivec_1.end(),
              ivec_2.begin(), (int(*)(int))foo);  // VALID

    // -----------------------------------------------------

    transform(ivec_1.begin(), ivec_1.end(),
              ivec_2.begin(), 
              [](auto x){ return foo(x); });      // VALID

    // -----------------------------------------------------
  }
*/

/*
  #include <cstdio>     // std::putchar
  #include <vector>
  #include <algorithm>  // std::for_each

  void func(int) { std::putchar('i'); }
  void func(double) { std::putchar('d'); }
  void func(long) { std::putchar('l'); }

  int main()
  {
    std::vector<int> ivec(10);
    std::vector<double> dvec(10);
    std::vector<long> lvec(10);

    // -----------------------------------------------------

    //std::for_each(ivec.begin(), ivec.end(), func);  // syntax error
    // error: no matching function for call to 
    // 'for_each( std::vector<int>::iterator, 
    //            std::vector<int>::iterator, 
    //            <unresolved overloaded function type>)'

    // -----------------------------------------------------

    std::for_each(ivec.begin(), ivec.end(), (void(*)(int))func);  
    // output -> iiiiiiiiii

    std::putchar('\n');

    // -----------------------------------------------------

    std::for_each(dvec.begin(), dvec.end(),
                  static_cast<void(*)(double)>(func));
    // output -> dddddddddd

    std::putchar('\n');

    // -----------------------------------------------------

    std::for_each(lvec.begin(), lvec.end(),
                  [](auto x){ return func(x); });
    // output -> llllllllll

    // -----------------------------------------------------
  }
*/

/*
                ---------------------------------
                | lambda expressions with C++20 |
                ---------------------------------
*/

/*
  // stateless lambda expression's default ctor and copy ctor
  // are not deleted special member functions anymore since C++20.

  int main()
  {
    auto f = [](int x){ return x * 5; };

    // -----------------------------------------------------

    decltype(f) f1; 
    // syntax error in C++17
    // error: use of deleted function 'main()::<lambda(int)>::<lambda>()'
    // VALID since C++20

    // -----------------------------------------------------

    auto g = f; 
    // VALID in C++17 and C++20
    // stateless lambda expression's copy ctor is not deleted
    // copy assignment operator is deleted 

    // -----------------------------------------------------

    f = g;
    // syntax error in C++17 
    // error: use of deleted function 
    // 'main()::<lambda(int)>& main()::<lambda(int)>::operator=(
    //  const main()::<lambda(int)>&)'
    // VALID since C++20

    // -----------------------------------------------------
  }
*/

/*
  #include <set>
  #include <functional>  // std::greater
  #include <cmath>       // std::abs

  int main()
  {
    using namespace std;

    set<int, greater<int>> x;
    // std::greater is a functor class(function object)

    // -----------------------------------------------------

    auto f1 = [](int lhs, int rhs){ return abs(lhs) < abs(rhs); };
    //set<int, decltype(f1)> set_1;  

    // syntax error in C++17
    // error: use of deleted function 
    // 'main()::<lambda(int, int)>::<lambda>()'

    // std::set's constructor try to default construct  
    // comparator type object in this case lambda expression
    // but closure type's default ctor is deleted function in C++17
    // VALID since C++20

    // -----------------------------------------------------

    auto f2 = [](int lhs, int rhs){ return abs(lhs) > abs(rhs); };

    std::set<int, decltype(f2)> set_2(f2);  
    // VALID in C++17 and C++20

    // -----------------------------------------------------
  }
*/

/*
  #include <set>
  #include <cmath>    // std::abs

  int main()
  {
    using namespace std;

    auto f = [](int lhs, int rhs){ return abs(lhs) < abs(rhs); };

    std::set<int, decltype(f)> set_x{ -4, 1, 4, 3, -2, 9, -6, 5 };

    for (auto val : set_x)
      cout << val << ' ';
    // output -> 1 -2 3 -4 5 -6 9
  }
*/

/*
  // using stateless lambda expressions 
  // inside unevaluated contexts is
  // VALID since C++20

  int main()
  {
    // -----------------------------------------------------

    constexpr auto size = sizeof([](int x){ return x * 5; });

    // -----------------------------------------------------

    decltype([](int x){ return x * 5; }) f;
    std::cout << f(210) << '\n';  // output -> 1050

    // -----------------------------------------------------
  }
*/

/*
  #include <set>

  int main()
  {
    // -----------------------------------------------------

    auto fn = [](int lhs, int rhs){ return lhs > rhs; };

    std::set<int, decltype(fn)> set_1{ 1, 2, 3, 4, 5 };

    // -----------------------------------------------------

    std::set<int, decltype([](int a, int b){ return a > b; })>
        set_2{ 1, 2, 3, 4, 5 };

    // -----------------------------------------------------
  }
*/

/*
  #include <memory>  // std::unique_ptr

  int main()
  {
    using namespace std;

    std::cout << "[0] main starts\n";

    // -----------------------------------------------------
    // before C++20 

    {
      auto fn = [](int* p){
        std::cout << "object at " 
                  << p << " address is being deleted\n";
        delete p;
      };

      std::unique_ptr<int, decltype(fn)> uptr(new int{ 10 }, fn);
    }

    std::cout << "[1] main continues\n";

    // -----------------------------------------------------

    {
      std::unique_ptr<int, decltype([](int* p){
        std::cout << "object at " 
                  << p << " address is being deleted\n";
        delete p;
      })> uptr(new int{ 110 });
    }

    std::cout << "[2] main continues\n";

    // -----------------------------------------------------

    // output ->
    //  [0] main starts
    //  object at 0x193cc24b9b0 address is being deleted
    //  [1] main continues
    //  object at 0x193cc24b9b0 address is being deleted
    //  [2] main continues
  }
*/

/*
  #include <set>
  #include <string>
  #include <cmath>    // std::abs

  template <typename C>
  void print(const C& con)
  {
    for (const auto& elem : con)
      std::cout << elem << ' ';
    std::cout << '\n';
  }

  int main()
  {
    using namespace std;

    // -----------------------------------------------------

    set<string> str_cities1{ "istanbul", "ankara", "izmir", "bursa", 
      "adana","kastamonu", "rize", "trabzon", "kayseri" };
    print(str_cities1);

    // output ->
    // adana ankara bursa istanbul izmir kastamonu kayseri rize trabzon

    // -----------------------------------------------------

    using g_set = set<string, decltype([](
      const string& s1, const string& s2){ return s1 > s2; })>;


    g_set str_cities2 = { "istanbul", "ankara", "izmir", "bursa", 
      "adana", "kastamonu", "rize", "trabzon", "kayseri" };

    print(str_cities2);
    // output ->
    // trabzon rize kayseri kastamonu izmir istanbul bursa ankara adana

    // -----------------------------------------------------

    using len_set = set<string, decltype([](
      const string& s1, const string& s2){ 
      return s1.size() > s2.size(); 
    })>;

    len_set str_cities3 = { "istanbul", "ankara", "izmir", "bursa", 
      "adana", "kastamonu", "rize", "trabzon", "kayseri" };

    print(str_cities3);
    // kastamonu istanbul trabzon ankara izmir rize

    // -----------------------------------------------------

    set<int> iset1 = { 9, -4, 15, -7, 39, -35, 89, -63 };

    print(iset1);
    // output -> -63 -35 -7 -4 9 15 39 89

    // -----------------------------------------------------

    using abs_set = set<int, decltype([](int lhs, int rhs){
      return abs(lhs) > abs(rhs); })>;

    abs_set iset2 = { 9, -4, 15, -7, 39, -35, 89, -63 };

    print(iset2);
    // output -> 89 -63 39 -35 15 9 -7 -4

    // -----------------------------------------------------
  }
*/

/*
  int main()
  {
    auto fn = [](int a) [[nodiscard]] { return a * 5; };  
    // syntax error
  }
*/

/*
  int main()
  {
    int x{ 0 };

    auto fn1 = [x]() mutable {++x; };   // VALID since C++20
    auto fn2 = [x] mutable { ++x; };    // VALID since C++23

    // -----------------------------------------------------

    auto fn3 = [](int x) constexpr {};
    auto fn4 = [](int x) mutable {};
    auto fn5 = [](int x) noexcept {};
    auto fn6 = [](int x) -> void {};
    auto fn7 = [](int x) consteval {};

    // -----------------------------------------------------
  }
*/

/*
            ------------------------------------
            | familiar template syntax (C++20) |
            ------------------------------------
*/


/*
  int main()
  {
    auto fn1 = [](int x, int y){ return x + y; }; 
    auto fn2 = [](auto x, auto y){ return x + y; };
    auto fn3 = [](auto x, decltype(x) y){ return x + y; };
    auto fn4 = []<typename T>(T x, T y){ return x + y; };

    // -----------------------------------------------------

    std::cout << fn1(2, 1.5) << '\n';   // output -> 3

    // -----------------------------------------------------

    std::cout << fn2(2, 1.5) << '\n';   // output -> 3.5

    // -----------------------------------------------------

    std::cout << fn3(2, 1.5) << '\n';   // output -> 3

    // -----------------------------------------------------

    std::cout << fn4(2, 3) << '\n';     // output -> 5

    std::cout << fn4(2.5, 3.5) << '\n'; // output -> 6

    std::cout << fn4(2, 1.5) << '\n';   // syntax error
    // error: no match for call to 
    // '(main()::<lambda(T, T)>) (int, double)'

    // -----------------------------------------------------
  }
*/

/*
  #include <vector>

  int main()
  {
    // -----------------------------------------------------

    auto fn = []<typename T>(const std::vector<T>& vec){
      return vec.size();
    };

    std::vector<int> ivec(10);
    std::vector<double> dvec(20);

    std::cout << fn(ivec) << '\n';  // output -> 10
    std::cout << fn(dvec) << '\n';  // output -> 20

    // -----------------------------------------------------

    auto fn2 = []<typename T>(const std::vector<T>& vec1, 
                              const std::vector<T>& vec2){
      return vec1.size() + vec2.size();
    };

    std::cout << fn2(ivec, dvec) << '\n';  // syntax error
    // error: no match for call to 
    // '(main()::<lambda( const std::vector<T>&, 
    //                    const std::vector<T>&)>) 
    // (std::vector<int>&, std::vector<double>&)'

    // -----------------------------------------------------
  }
*/

/*
  int main()
  {
    auto fn = []<class T, size_t N>(T(&ref_arr)[N]){
      for (auto& t : ref_arr)
        std::cout << t << ' ';
    };

    int arr[]{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    fn(arr);  // output -> 1 2 3 4 5 6 7 8 9 10
  }
*/

/*
  int main()
  {
    auto fn = [](auto&& x){
      foo(std::forward<decltype(x)>(x));
    };

    auto fn2 = []<typename T>(T&& x){
      foo(std::forward<T>(x));
    };
  }
*/

/*
  #include <vector>

  template <typename ...Args>
  void foo(Args&&... args);

  int main()
  {
    // -----------------------------------------------------

    auto fn_push = []<typename T>(std::vector<T>& vec, const T& val){
      vec.push_back(val);
    };

    std::vector<int> ivec;
    fn_push(ivec, 10);

    // -----------------------------------------------------

    auto call_foo = []<typename ...Args>(Args&&... args){
      foo(std::forward<Args>(args)...);
    };

    // -----------------------------------------------------
  }
*/

/*
  template <typename T>
  void foo(T x);

  int main()
  {
    // -----------------------------------------------------

    auto fn1 = [](auto x){ return x * x; };

    auto dval = fn1.operator()<double>(3.12);
    std::cout << dval << '\n';  // output -> 9.7344

    // -----------------------------------------------------

    auto fn2 = []<size_t N>(){
      static int arr[N]{};
      return arr;
    };

    fn2.operator()<10>();

    // -----------------------------------------------------
  }
*/

/*
  class Myclass {
  public:
    void foo()
    {
      int a = 10;

      // -----------------------------------------------------

      auto fn1 = [=, this] { return a * (m_x + m_y); };
      // syntax error in C++17 valid in C++20
      // capture all by copy but this by reference

      // -----------------------------------------------------

      auto fn2 = [=] { return a * (m_x + m_y); };
      // deprecated in C++20

      // -----------------------------------------------------
    }
  private:
    int m_x, m_y;
  };
*/

/*
  template <typename ...Args>
  void foo(Args... args)
  {
    std::cout << sizeof...(args) << '\n';
  }

  template <typename ...Args>
  void func_1(Args... args)
  {
    [=](){ foo(args...);}();
  }

  template <typename ...Args>
  void func_2(Args... args)
  {
    [&](){ foo(args...);}();
  }

  template <typename ...Args>
  void func_3(Args... args)
  {
    [args...](){ foo(args...);}();
  }

  template <typename ...Args>
  void func_4(Args... args)
  {
    [&args...](){ foo(args...);}();
  }

  int main()
  {
    func_1(2, 3.14, "hello"); // output -> 3
    func_2(2, 3.14, "hello"); // output -> 3
    func_3(2, 3.14, "hello"); // output -> 3
    func_4(2, 3.14, "hello"); // output -> 3
  }
*/

/*
  #include <tuple>    // std::make_tuple, std::apply
  #include <utility>  // std::move

  template <typename ...Args>
  auto delay_invoke_foo(Args... args)
  {
    return [tup = std::make_tuple(std::move(args...))]() -> decltype(auto){
      return std::apply(
        [](const auto&... args) -> decltype(auto){return foo(args...);}, 
        tup);
    };
  }
*/

/*
  #include <utility>  // std::move

  template <typename ...Args>
  auto delay_invoke_foo(Args... args)
  {
    return[...args = std::move(args)]() -> decltype(auto) {
      return foo(args...);
    };
  }
*/