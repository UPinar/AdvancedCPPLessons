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