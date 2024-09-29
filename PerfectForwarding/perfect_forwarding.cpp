#include <iostream>

/*
            ----------------------------------------------
            | universal(forwarding) reference (REMINDER) |
            ----------------------------------------------
*/

/*
  template <typename T>
  void func(T&& a);     // a is a universal reference

  void foo(auto&& b);   // b is a universal reference

  int main()
  {
    auto&& c = 10;      // c is a universal reference
  }
*/

/*
  // const or non-const, LValue or RValue expressions 
  // can be sent to both foo and func functions

  template <typename T>
  void foo(const T&) 
  {
    // inside foo() function we lost the information of the
    // value category and the constness of the argument
  }

  template <typename T>
  void func(T&&)
  {
    // inside func() function we have the information that 
    // the argument is an RValue expression or an LValue expression
    // and also the constness of the argument

    // we can use those informations in compile time
  }
*/

/*
  for universal references T will deduce to
    - for LValue expression argument -> T&
    - for RValue expression argument -> T


  argument      constness         value category
  --------------------------------------------------------------
  Scenario 1:   non-const         LValue expression     
  T   ===> Myclass&         
  arg ===> Myclass& (& - && -> &)
  --------------------------------------------------------------
  Scenario 2:   non-const         RValue expression
  T   ===> Myclass          
  arg ===> Myclass&& (no reference collapsing)
  --------------------------------------------------------------
  Scenario 3:   const             LValue expression
  T   ===> const Myclass&   
  arg ===> const Myclass& (& - && -> &)
  --------------------------------------------------------------
  Scenario 4:   const             RValue expression
  T   ===> const Myclass    
  arg ===> const Myclass&& (no reference collapsing)
  --------------------------------------------------------------
*/

/*
  template <typename T>
  void func(T&& x)
  {
    // when we check T, we can understand 
    // arguments constness and value category
    // if T is reference type     -> argument is LValue expression
    // if T is not reference type -> argument is RValue expression

    // when we check x's declaration type, we can understand
    // arguments constness and value category
    // if x is R value reference -> argument is RValue expression
    // if x is L value reference -> argument is LValue expression
  }

  class Myclass{};

  int main(){
    Myclass m;
    func(m);              // Scenario 1 
    // "m" is an LValue expression(non-const)

    func(Myclass{});      // Scenario 2, 
    //  "Myclass{}" is an (P)RValue expression(non-const)

    const Myclass m2;
    func(m2);             // Scenario 3
    // "m2" is an LValue expression (const)

    func(std::move(m2));  // Scenario 4
    // "std::move(m2)" is an RValue expression(const)
  }
*/

/*
  template <typename T>
  void foo(T&& arg)
  { 
    // sending an argument without losing its 
    // value category and constness

    // if T type is reference type, argument is LValue expression
    bar(arg);

    // if T type is not reference type, argument is RValue expression
    bar(std::move(arg));
  }

  template <typename T>
  void func(T&& arg)
  {
    std::forward<T>(arg); 
    // "std::forward<T>(arg)" is an expression

    // if arg is an LValue reference, 
    // "std::forward<T>(arg)" expression is an LValue expression

    // if arg is an RValue reference,
    // "std::forward<T>(arg)" expression is an RValue expression

    bar(std::forward<T>(arg));
    // sending an argument without losing its
    // value category and constness
    // it is called perfect forwarding
  }
*/

/*
                ---------------------------------
                | perfect forwarding (REMINDER) |
                ---------------------------------
*/

/*
  - container's emplace functions 
      (emplace, emplace_back, emplace_front, emplace_after)

  - std::make_unique, std::make_shared functions
*/

/*
                <--- check emplace_back.png --->

  --- std::vector<Myclass>::push_back function overloads ---
  push_back(const Myclass&) -> Myclass's copy constructor will be called
  push_back(Myclass&&)      -> Myclass's move constructor will be called

  --- std::vector<Myclass>::emplace_back function ---
  emplace_back(Args&&...)   -> Myclass's constructor will be called
  arguments will be perfect forwarded to Myclass's constructor
*/

/*
  std::make_unique will return std::unique_ptr object

  std::make_unique<Myclass>(Args&&...) function will call
    - Myclass objects constructor 
    - arguments will perfect forwarded to Myclass's constructor
*/

/*
  #include <utility>  // std::forward

  class Myclass{};

  void func(Myclass&)
  {
    std::cout << "Myclass&\n";
  }

  void func(const Myclass&)
  {
    std::cout << "const Myclass&\n";
  }

  void func(Myclass&&)
  {
    std::cout << "Myclass&&\n";
  }

  void func(const Myclass&&)
  {
    std::cout << "const Myclass&&\n";
  }

  template <typename T>
  void call_func(T&& t)
  {
    func(std::forward<T>(t));
  }

  // perfect forwarding with call_func overloads 
  // void call_func(Myclass& m)
  // {
  //   func(m);
  // }

  // void call_func(const Myclass& m)
  // {
  //   func(m);
  // }

  // void call_func(Myclass&& m)
  // {
  //   func(std::move(m));
  // }

  // void call_func(const Myclass&& m)
  // {
  //   func(std::move(m));
  // }

  int main()
  {
    Myclass m;
    const Myclass cm;

    // "m" is an LValue expression
    func(m);                    // output -> Myclass&
    call_func(m);               // output -> Myclass&

    // "cm" is an LValue expression
    func(cm);                   // output -> const Myclass&
    call_func(cm);              // output -> const Myclass&

    // "Myclass{}" is PRValue expression 
    func(Myclass{});            // output -> Myclass&&
    call_func(Myclass{});       // output -> Myclass&&

    // "std::move(m)" is an XValue expression
    func(std::move(m));         // output -> Myclass&&
    call_func(std::move(m));    // output -> Myclass&&

    // "std::move(cm)" is an XValue expression
    func(std::move(cm));        // output -> const Myclass&&
    call_func(std::move(cm));   // output -> const Myclass&&
  }
*/

/*
  // if a function that we want to call have 2 arguments
  template <typename T, typename U>
  void call_foo(T&& t, U&& u)
  {
    foo(std::forward<T>(t), std::forward<U>(u));
  }

  // if a function that we want to call more than 1 argument
  template <typename ...Args>
  void call_foo(Args&&... args)
  {
    foo(std::forward<Args>(args)...); // pack expansion
  }
*/

/*
  // std::make_unique implementation

  #include <memory>   // std::unique_ptr

  template <typename T, typename ...Args>
  std::unique_ptr<T> Make_Unique(Args&&... args)
  {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
  }
*/

/*
  // basic std::forward implementation

  #include <type_traits>  // std::remove_reference_t

  // not a universal reference parameter
  template <typename T>
  constexpr T&& Forward(std::remove_reference_t<T>& t) noexcept
  {
    return static_cast<T&&>(arg);
    // if T is L value reference type, (& - && -> &)
    // return type will be T& (L value reference)

    // if T is not a reference type, (no reference collapsing)
    // return type will be T&& (R value reference)
  }


  // if t is an LValue expression, T will be deduced to T&
  // if t is an RValue expression, T will be deduced to T
  template <typename T>
  void call_foo(T&& t)
  {
    foo(Forward<T>(t));   
    // if t is an LValue expression,
    // T will be deduced to T& in call_foo
    // "Forward<T>(t)" expressions T'type is call_foo functions T&

    // if t is an RValue expression,
    // T will be deduced to T in call_foo
    // "Forward<T>(t)" expressions T'type is call_foo functions T

    // so Forward function can be called with
    //  - L value reference type
    //  - non-reference type
  }
*/

/*
  // libc++ implementation of std::forward

  #include <type_traits>  
  // std::remove_reference_t, std::is_lvalue_reference_v

  template <typename T>
  T&& Forward(std::remove_reference_t<T>& t) noexcept
  {
    return static_cast<T&&>(t);
  }

  template <typename T>
  T&& Forward(std::remove_reference_t<T>&& t) noexcept
  {
    static_assert(!std::is_lvalue_reference_v<T>, 
      "Can not forward an RValue as an LValue");
    return static_cast<T&&>(t);
  }

  class Myclass{};

  int main()
  {
    Forward<Myclass&>(Myclass{});
    // error: static assertion failed: 
    // Can not forward an RValue as an LValue
  }
*/

/*
  #include <utility>  // std::forward

  class Myclass{};

  void func(Myclass&)
  {
    std::cout << "Myclass&\n";
  }

  void func(const Myclass&)
  {
    std::cout << "const Myclass&\n";
  }

  void func(Myclass&&)
  {
    std::cout << "Myclass&&\n";
  }

  void func(const Myclass&&)
  {
    std::cout << "const Myclass&&\n";
  }

  template <typename T>
  void call_func_1(T&& arg)
  {
    func(std::forward<T>(arg));
  }

  template <typename T>
  void call_func_2(T&& arg)
  {
    func(std::forward<decltype(arg)>(arg));
  }

  int main()
  {
    Myclass m;
    const Myclass cm;

    auto fn = [](auto&& r){
      func(std::forward<decltype(r)>(r));
    };
    // compiler generated closure type's operator() function
    // is a template and its parameter is a universal reference

    // "m" is an LValue expression
    call_func_1(m);               // output -> Myclass&
    call_func_2(m);               // output -> Myclass&
    fn(m);                        // output -> Myclass& 

    // "cm" is an LValue expression
    call_func_1(cm);              // output -> const Myclass&
    call_func_2(cm);              // output -> const Myclass&
    fn(cm);                       // output -> const Myclass&

    // "Myclass{}" is PRValue expression 
    call_func_1(Myclass{});       // output -> Myclass&&
    call_func_2(Myclass{});       // output -> Myclass&&
    fn(Myclass{});                // output -> Myclass&&

    // "std::move(m)" is an XValue expression
    call_func_1(std::move(m));    // output -> Myclass&&
    call_func_2(std::move(m));    // output -> Myclass&&
    fn(std::move(m));             // output -> Myclass&&

    // "std::move(cm)" is an XValue expression
    call_func_1(std::move(cm));   // output -> const Myclass&&
    call_func_2(std::move(cm));   // output -> const Myclass&&
    fn(std::move(cm));            // output -> const Myclass&&
  }
*/

/*
        <--- check function_overload_resolution.png --->
*/

/*
  class Myclass{
  public:
    Myclass(const Myclass&);  // copy constructor
    Myclass(Myclass&&);       // move constructor

    template <typename T>
    Myclass(T&&);             // universal constructor
  };

  int main()
  {
    Myclass m(12);
    Myclass m2(3.4);
  }
*/

/*
  #include <utility>  // std::move

  class Myclass{
  public:
    Myclass() = default;

    Myclass(const Myclass&){
      std::cout << "copy ctor\n";
    }

    Myclass(Myclass&&){
      std::cout << "move ctor\n";
    }

    template <typename T>
    Myclass(T&&){
      std::cout << "universal ctor\n";
    }
  };

  int main()
  {
    Myclass mx;
    const Myclass cmx;

    Myclass a{ cmx };             
    // "cmx" is an LValue expression
    // There is a Myclass(const Myclass&) overload    [exact match]
    // output -> copy ctor

    Myclass b{ mx };             
    // "mx" is an LValue expression
    // There is no Myclass(Myclass&) overload         [exact match]
    // so universal constructor is called             [2'nd choice]
    // output -> universal ctor

    // Generally we wrote this line to call copy ctor
    // but universal constructor is called

    Myclass c{ std::move(mx) };   
    // "std::move(mx)" is an XValue expression
    // There is a Myclass(Myclass&&) overload         [exact match]
    // output -> move ctor

    Myclass d{ std::move(cmx) };  
    // "std::move(cmx)" is an XValue expression
    // There is no Myclass(const Myclass&&) overload  [exact match]
    // so universal constructor is called             [2'nd choice]
    // output -> universal ctor
  }
*/

/*
  // We want this function to take std::string argument 
  // but its value category and constness will retain(kaybetmemek)
  template <typename T>
  void func(T&&);

  // - we can use SFINAE
  // - we can use concepts(require expression)
*/

/*
  #include <utility>      // std::move
  #include <type_traits>  // std::is_same_v, std::remove_cvref_t

  class Myclass{
  public:
    Myclass() = default;

    Myclass(const Myclass&){
      std::cout << "copy ctor\n";
    }

    Myclass(Myclass&&){
      std::cout << "move ctor\n";
    }

    template <typename T>
      requires (!std::is_same_v<std::remove_cvref_t<T>, Myclass>)
    Myclass(T&&){
      std::cout << "universal ctor\n";
    }
  };

  int main()
  {
    Myclass mx;
    const Myclass cmx;

    Myclass a{ cmx };               // output -> copy ctor
    Myclass b{ mx };                // output -> copy ctor    
    Myclass c{ std::move(mx) };     // output -> move ctor
    Myclass d{ std::move(cmx) };    // output -> copy ctor
    Myclass e{ 10 };                // output -> universal ctor
  }
*/

/*
  // ---------------- REMINDER ----------------

  #include <utility>  // std::forward

  template <typename T>
  void func(T&& t)
  {
    foo(std::forward<T>(t));            // perfect forwarding
  }

  // abbreviated template syntax (C++20)
  void bar(auto&& t)
  {
    foo(std::forward<decltype(t)>(t));  // perfect forwarding
  }


  int main()
  {
    auto fn = [](auto&& t){
      foo(std::forward<decltype(t)>(t));  // perfect forwarding
    };

    auto fn_2 = []<typename T>(T&& r){    // (C++20)
      foo(std::forward<T>(r));            // perfect forwarding
    };
  }
*/

/*
  // using forwarding reference parameter 
  // for checking constness of the argument

  #include <string>

  void navigate(std::string::iterator beg, 
                std::string::iterator end)
  {
    std::cout << "non const semantics on the passed range\n";
  }

  void navigate(std::string::const_iterator beg, 
                std::string::const_iterator end)
  {
    std::cout << "const semantics on the passed range\n";
  }

  template <typename T>
  void process_container(T&& container)  // universal reference
  {
    navigate(container.begin(), container.end());
    // if parameter is const qualified, 
    // becuase of the universal reference will not drop constness
    // begin() and end() functions 
    // will return const_iterator, otherwise iterator
  }

  int main()
  {
    std::string str{ "mutable world" };
    const std::string c_str{ "immutable universe" };

    process_container(str);
    // output -> non const semantics on the passed range

    process_container(c_str);
    // output -> const semantics on the passed range

    process_container(std::string { "temporary life" });
    // output -> non const semantics on the passed range

    process_container(std::move(str));
    // output -> non const semantics on the passed range

    process_container(std::move(c_str));
    // output -> const semantics on the passed range
  }
*/

/*
  // using forwarding reference parameter 
  // for checking constness of the argument

  #include <type_traits>  // std::is_const_v, std::remove_reference_t
  #include <string>

  template <typename T>
  void func(T&&)    // universal reference parameter
  {
    if constexpr (std::is_const_v<std::remove_reference_t<T>>)
      std::cout << "const argument\n";
    else
      std::cout << "non-const argument\n";
  }

  // for reference type when T deduce to const int& (low-level const)
  // without using std::remove_reference_t metafunction
  // std::is_const_v<T> will return false 
  // if will check that if a reference type is const qualified

  int main()
  {
    std::string str{ "hello world" };
    const std::string c_str{ "hello universe" };

    func(str);      // output -> non-const argument
    func(c_str);    // output -> const argument
  }
*/

/*
  // using forwarding reference parameter 
  // for checking the value category of the argument

  #include <type_traits>  // std::is_lvalue_reference_v

  // T is an L value reference type (if argumnen is LValue expression)
  // T is a non-reference type (if argument is RValue expression)
  template <typename T>
  void func(T&&)    // universal reference parameter
  {
    if constexpr (std::is_lvalue_reference_v<T>)
      std::cout << "LValue argument\n";
    else
      std::cout << "RValue argument\n";
  }

  class Myclass{};

  int main()
  {
    func(Myclass{});    // output -> RValue argument
    // "Myclass{}" is PRValue expression

    Myclass m;
    func(m);            // output -> LValue argument
    // "m" is an LValue expression

    func(std::move(m)); // output -> RValue argument
    // "std::move(m)" is an XValue expression
  }
*/

/*
  // <------------------ REMINDER ------------------>
  template <typename T>
  void func(T, T);

  int main()
  {
    func(1, 2);
    func(1, 2.);  // syntax error
    // error: no matching function for call to 'func(int, double)'
  }
*/

/*
  // <------------------ REMINDER ------------------>
  template <typename T>
  void func(T, T);

  template <typename T>
  void foo(T&, T&);

  int main()
  {
    func("hello", "universe"); // VALID
    // const char*, const char*

    foo("hello", "universe");  // syntax error
    // error: no matching function for call to 
    // 'foo(const char [6], const char [9])'

    foo("hello", "world");     // VALID
    // const char[6], const char[6]
  }
*/

/*
  // <------------------ Problem ------------------>

  #include <vector>
  #include <string>

  template <typename T>
  void insert(std::vector<T>& vec, T&& elem)
  {
    vec.push_back(std::forward<T>(elem));
  }

  int main()
  {
    std::vector<std::string> svec;
    std::string str;

    insert(svec, str);  // syntax error
    // error: no matching function for call to 
    // 'insert( std::vector<std::__cxx11::basic_string<char> >&, 
    //          std::string&)'

    // for 1st param (std::vector<T>&)  
    //  -> T will deduce to std::string
    // for 2nd param (T&&)              
    //  -> T will deduce to std::string&
  }
*/

/*
  // <------------------ Solution_1 ------------------>

  #include <vector>
  #include <string>
  #include <type_traits>  // std::remove_reference_t

  template <typename T>
  void insert(std::vector<std::remove_reference_t<T>>& vec, 
              T&& elem)
  {
    vec.push_back(std::forward<T>(elem));
  }

  int main()
  {
    std::vector<std::string> svec;
    std::string str;

    insert(svec, str);  // VALID

    // for 2nd param (T&&)
    //  -> T will deduce to std::string& 
    // because of 1st param T can be deduced to std::string&
    // T will deduce to std::string&
  }
*/

/*
  // <------------------ Solution_2 ------------------>

  #include <vector>
  #include <string>

  template <typename ElemType, typename T>
  void insert(std::vector<ElemType>& vec, T&& elem)
  {
    vec.push_back(std::forward<T>(elem));
  }

  int main()
  {
    std::vector<std::string> svec;
    std::string str;

    insert(svec, str);  // VALID
  }
*/

/*
  // <------------------ Solution_3 ------------------>

  #include <vector>
  #include <string>

  template <typename Container, typename T>
  void insert(Container& con, T&& elem)
  {
    con.push_back(std::forward<T>(elem));
  }

  int main()
  {
    std::vector<std::string> svec;
    std::string str;

    insert(svec, str);  // VALID
  }
*/

/*
                            ----------
                            | auto&& |
                            ----------
*/

/*
  class Myclass{};

  template <typename T>
  void foo(T);

  template <typename T>
  void bar(T&);

  template <typename T>
  void baz(T&&);

  int main()
  {
    Myclass mx;
    // ----------------------------------------------
    foo(mx); 
    // deduction done for T type in foo function, 
    auto x = mx;
    // will be same as the deduction for auto type

    // ----------------------------------------------

    bar(mx);
    // deduction done for T type in bar function,
    auto& y = mx;
    // will be same as the deduction for auto type

    // ----------------------------------------------

    baz(mx);
    // deduction done for T type in baz function,
    auto&& z = mx;
    // will be same as the deduction for auto type

    // ----------------------------------------------

    Myclass m;
    const Myclass cm;

    auto&& r3 = m;              // Myclass& r3 = m;
    // "m" is an LValue expression
    // auto type deduction will be Myclass& (& - && -> &)

    auto&& r6 = cm;             // const Myclass& r6 = cm;
    // "cm" is an LValue expression
    // auto type deduction will be const Myclass& (& - && -> &)

    auto&& r4 = Myclass{};      // Myclass&& r4 = Myclass{};
    // "Myclass{}" is PRValue expression
    // auto type deduction will be Myclass 

    auto&& r5 = std::move(m);   // Myclass&& r5 = std::move(m);
    // "std::move(m)" is an XValue expression
    // auto type deduction will be Myclass

    auto&& r7 = std::move(cm);  // const Myclass&& r7 = std::move(cm);
    // "std::move(cm)" is an XValue expression
    // auto type deduction will be const Myclass
  }
*/

/*
  #include <utility>  // std::forward

  class Myclass{};

  void foo(Myclass&){
    std::cout << "foo(Myclass&)\n";
  }

  void foo(const Myclass&){
    std::cout << "foo(const Myclass&)\n";
  }

  void foo(Myclass&&){
    std::cout << "foo(Myclass&&)\n";
  }

  void foo(const Myclass&&){
    std::cout << "foo(const Myclass&&)\n";
  }

  int main()
  {
    Myclass m;
    const Myclass cm;

    // ---------------------------------------------------------

    // "Myclass{}" is PRValue expression
    // auto type deduction will be Myclass

    auto&& r1 = Myclass{};  // Myclass&& r1 = Myclass{};
    foo(std::forward<decltype(r1)>(r1));  // output -> foo(Myclass&&)
    foo(Myclass{});                       // output -> foo(Myclass&&)
    
    // ---------------------------------------------------------

    // "m" is an LValue expression  
    // auto type deduction will be Myclass& (& - && -> &)

    auto&& r2 = m;  // Myclass& r2 = m;
    foo(std::forward<decltype(r2)>(r2));  // output -> foo(Myclass&)
    foo(m);                               // output -> foo(Myclass&)

    // ---------------------------------------------------------

    // "cm" is an LValue expression  
    // auto type deduction will be const Myclass& (& - && -> &)

    auto&& r3 = cm; // const Myclass& r3 = cm;
    foo(std::forward<decltype(r3)>(r3));  // output -> foo(const Myclass&)
    foo(cm);                              // output -> foo(const Myclass&)

    // ---------------------------------------------------------

    Myclass m1;
    Myclass m2;

    // "std::move(m)" is an XValue expression
    // auto type deduction will be Myclass

    auto&& r4 = std::move(m1); // Myclass&& r4 = std::move(m);
    foo(std::forward<decltype(r4)>(r4));    // output -> foo(Myclass&&)
    foo(std::move(m2));                     // output -> foo(Myclass&&)         

    // ---------------------------------------------------------

    const Myclas cm1;
    const Myclas cm2;

    // "std::move(cm)" is an XValue expression
    // auto type deduction will be const Myclass

    auto&& r5 = std::move(cm1);  // const Myclass&& r5 = std::move(cm1);
    foo(std::forward<decltype(r5)>(r5));  // output -> foo(const Myclass&&)
    foo(std::move(cm2));                  // output -> foo(const Myclass&&)
  }
*/

/*
  // return value perfect passsing 

  class Myclass{};

  void foo(Myclass&){
    std::cout << "foo(Myclass&)\n";
  }

  void foo(const Myclass&){
    std::cout << "foo(const Myclass&)\n";
  }

  void foo(Myclass&&){
    std::cout << "foo(Myclass&&)\n";
  }

  const Myclass& func_const_lref(const Myclass& str){
    return str;
  }

  Myclass& func_non_const_lref(Myclass& str){
    return str;
  }

  Myclass&& func_rref(Myclass&& str){
    return std::move(str);
  }

  Myclass func_value(const Myclass& str){
    return str;
  }

  int main()
  {
    Myclass m;
    const Myclass cm;

    foo(func_rref(Myclass{}));    // output -> foo(Myclass&&)
    // "func_rref(Myclass{})" is R Value expression,
    // its data type is Myclass&&

    foo(func_non_const_lref(m));  // output -> foo(Myclass&)
    // "func_non_const_lref(m)" is L Value expression
    // its data type is Myclass&

    foo(func_const_lref(cm));     // output -> foo(const Myclass&)
    // "func_const_lref(cm)" is L Value expression
    // its data type is const Myclass&

    foo(func_value(m));           // output -> foo(Myclass&&)
    // "func_value(m)" is R Value expression
    // its data type is Myclass

    // ---------------------------------------------------------

    // If we want to use the return value of a function
    // and then pass it to another function without losing
    // its value category and constness, we can use auto&&

    auto&& r1 = func_rref(Myclass{});  
    foo(std::forward<decltype(r1)>(r1));  // output -> foo(Myclass&&)

    auto&& r2 = func_non_const_lref(m);
    foo(std::forward<decltype(r2)>(r2));  // output -> foo(Myclass&)

    auto&& r3 = func_const_lref(cm);
    foo(std::forward<decltype(r3)>(r3));  // output -> foo(const Myclass&)

    auto&& r4 = func_value(m);
    foo(std::forward<decltype(r4)>(r4));  // output -> foo(Myclass&&)

    // ---------------------------------------------------------
  }
*/

/*
  #include <utility>  // std::forward

  template <typename T>
  void func(T&& t)
  {
    // --------- Perfect passing scenario 1 ---------
    foo(bar(std::forward<T>(t)));    

    // --------- Perfect passing scenario 2 ---------
    auto&& ret = bar(std::forward<T>(t));
    foo(std::forward<decltype(ret)>(ret));
  }
*/

/*
  #include <vector>

  int main()
  {
    using namespace std;

    std::vector<int> ivec(10);  // value initialize > first zero init

    for(auto val : ivec)
      cout << val << ' ';
    // output -> 0 0 0 0 0 0 0 0 0 0

    for(auto val : ivec)
      val = 10;
    cout << '\n';

    for(auto val : ivec)
      cout << val << ' ';
    // output -> 0 0 0 0 0 0 0 0 0 0
  }
*/

/*
  #include <vector>

  // template <>
  // class Vector{
  // public:
  //   class reference{
  //     operator=(bool){}
  //     operator bool() const{}
  //   };
  //   reference operator[](std::size_t idx){}
  // };

  int main()
  {
    using namespace std;

    std::vector<bool> bvec(10);  // value initialize > first zero init

    for(auto val : bvec)
      cout << val << ' ';
    // output -> 0 0 0 0 0 0 0 0 0 0

    for(auto val : bvec)
      val = true;
    cout << '\n';

    for(auto val : bvec)
      cout << val << ' ';
    // output -> 1 1 1 1 1 1 1 1 1 1

    // ---------------------------------------------------------

    // std::vector<bool> specialization is a partial specialization

    auto x = bvec[0]; 
    auto y = bvec.operator[](0);
    // Those 2 lines are equivalent.
    // "x" and "y"s type is std::vector<bool>::reference (proxy object)

    bvec[4] = false;
    bvec.operator[](4).operator=(false);
    // Those 2 lines are equivalent

    auto iter = bvec.begin(); 
    iter.operator*() = true;  
    bvec.operator[](0).operator=(true);
    // Those 2 lines are equivalent.

    // iter's type is std::vector<bool>::iterator
    // becuase of operator* can not return a reference to bit
    // it returns a proxy object (std::vector<bool>::reference)

    // ---------------------------------------------------------
  }
*/

/*
  vector<bool> bvec(4);

  // ---------- ranged based for loop 1 ----------
  for (auto : bvec){}

  // ----------- psudeo code range-based for loop 1 -----------
  auto&& rng = bvec;
  auto pos = rng.begin();
  auto end = rng.end();

  for(; pos != end; ++pos){
    auto temp = *pos;
  }


  // ---------- ranged based for loop 2 ----------
  for (auto& x : bvec){}

  // ----------- psudeo code range-based for loop 2 -----------
  auto&& rng = bvec;
  auto pos = rng.begin();
  auto end = rng.end();

  for(; pos != end; ++pos){
    auto& temp = *pos;
  }


  // ---------- ranged based for loop 3 ----------
  for (auto&& x : bvec){}

  // ----------- psudeo code range-based for loop 3 -----------
  auto&& rng = bvec;
  auto pos = rng.begin();
  auto end = rng.end();

  for(; pos != end; ++pos){
    auto&& temp = *pos;
  }
*/

/*
  #include <vector>

  int main()
  {
    using namespace std;

    vector<bool> bvec(10);

    for (auto val : bvec)
      cout << val << ' ';
    cout << '\n';
    // output -> 0 0 0 0 0 0 0 0 0 0

    // ranged based for loop implementation
    auto&& rng = bvec;         
    auto pos = rng.begin();
    auto end = rng.end();

    for(; pos != end; ++pos){
      auto temp = *pos;
      temp = true;
    }

    for (auto val : bvec)
      cout << val << ' ';
    // output -> 1 1 1 1 1 1 1 1 1 1
  }
*/

/*
  #include <vector>

  int main()
  {
    using namespace std;

    vector<bool> bvec(10);

    for (auto val : bvec)
      cout << val << ' ';
    cout << '\n';
    // output -> 0 0 0 0 0 0 0 0 0 0

    for (auto&& val : bvec)
      val = true;

    for (auto val : bvec)
      cout << val << ' ';
    // output -> 1 1 1 1 1 1 1 1 1 1
  }
*/

/*
  #include <vector>
  #include <string>

  template <typename C, typename T>
  void Fill(C& con, const T& val)
  {
    for(auto& elem : con)
      elem = val;
  }

  template <typename C, typename T>
  void Fill_equivalent(C& con, const T& val)
  {
    auto&& rng = con;
    auto pos = rng.begin();
    auto end = rng.end();

    for(; pos != end; ++pos){
      auto& elem = *pos;    
      // for vector<bool> specialization
      // *pos(pos.operator() function)'s return type is class type
      // so "*pos" is an R value expression
      // L value reference can not bind to R value expression
      elem = val;
    }
  }

  // for proxy classes better using 
  // auto&& (universal reference) insted of auto&
  template <typename C, typename T>
  void Proxy_Fill(C& con, const T& val)
  {
    for(auto&& elem : con)
      elem = val;
  }

  int main()
  {
    // ---------------------------------------------------------

    std::vector<int> ivec{ 1, 2, 3, 4, 5 };

    for (auto val : ivec)
      std::cout << val << ' ';
    std::cout << '\n';
    // output -> 1 2 3 4 5

    Fill(ivec, 100);

    for (auto val : ivec)
      std::cout << val << ' ';
    std::cout << '\n';
    // output -> 100 100 100 100 100

    // ---------------------------------------------------------

    std::vector<std::string> svec{ "hello", "world", "universe" };
    for (auto val : svec)
      std::cout << val << ' ';
    std::cout << '\n';
    // output -> hello world universe

    Fill(svec, "C++");

    for (auto val : svec)
      std::cout << val << ' ';
    std::cout << '\n';
    // output -> C++ C++ C++

    // ---------------------------------------------------------

    std::vector<bool> bvec{ false, false, false };
    for (auto val : bvec)
      std::cout << val << ' ';
    std::cout << '\n';
    // output -> 0 0 0

    Fill(bvec, true); // syntax error
    // error: cannot bind non-const lvalue reference of type 
    // 'std::_Bit_reference ' to an rvalue of type 
    // 'std::_Bit_iterator::reference'

    Proxy_Fill(bvec, true);  // VALID
    for (auto val : bvec)
      std::cout << val << ' ';
    std::cout << '\n';
    // output -> 1 1 1

    // ---------------------------------------------------------
  }
*/

/*
                      ------------------
                      | decltype(auto) |
                      ------------------
*/

/*
  int& foo();
  int&& bar();

  int main()
  {
    int x = 10;

    decltype(auto) val_1 = x;   
    // "x" is an identifier
    // decltype(x) -> int, decltype(auto) -> int

    decltype(auto) val_2 = (x);
    // "(x)" is an LValue expression, decltype(LValue expr) -> T&
    // decltype((x)) is int&, decltype(auto) -> int&

    decltype(auto) val_3 = 12;
    // "12" is PRValue expression, decltype(PRValue expr) -> T
    // decltype(12) is int, decltype(auto) -> int

    decltype(auto) val_4 = foo();
    // "foo()" is LValue expression, decltype(LValue expr) -> T&
    // decltype(foo()) is int&, decltype(auto) -> int&

    decltype(auto) val_5 = bar();
    // "bar()" is XValue expression, decltype(XValue expr) -> T&&
    // decltype(bar()) is int&&, decltype(auto) -> int&&

    int* ptr = &x;

    decltype(auto) val_6 = *ptr;
    // "*ptr" is an LValue expression, decltype(LValue expr) -> T&
    // decltype(*ptr) is int&, decltype(auto) -> int&
  }
*/

/*
  // https://www.scs.stanford.edu/~dm/blog/decltype.html

  decltype(auto) fn_A(int i)
  {
    return i;
    // "i" is an identifier
    // decltype(i) is int, decltype(auto) -> int
  }

  decltype(auto) fn_B(int i)
  {
    return (i);
    // "(i)" is an expression, decltype(LValue expr) -> T&
    // decltype((m)) is int&, decltype(auto) -> int&

    // returning a reference to automatic storage duration object
    // undefined behavior(ub)
  }

  decltype(auto) fn_C(int i)
  {
    return (i+1);
    // "(i + 1)" is an expression, decltype(PRValue expr) -> T
    // decltype((i + 1)) is int, decltype(auto) -> int
  }

  decltype(auto) fn_D(int i)
  {
    return i++;
    // "i++" is an expression, decltype(PRValue expr) -> T
    // decltype(i++) is int, decltype(auto) -> int
  }

  decltype(auto) fn_E(int i)
  {
    return ++i;
    // "++i" is an expression, decltype(LValue expr) -> T&
    // decltype(++i) is int&, decltype(auto) -> int&

    // returning a reference to automatic storage duration object
    // undefined behavior(ub)
  }

  decltype(auto) fn_F(int i)
  {
    return (i >= 0 ? i : 0);
    // ternary operators 3rd operand "0" is PRValue expression
    // "(i >= 0 ? i : 0)" is also PRValue expression
    // decltype(PRValue expr) -> T
    // decltype((i >= 0 ? i : 0)) is int, decltype(auto) -> int
  }

  decltype(auto)
  fn_G(int i, int j)
  {
    return i >= j ? i : j;
    // ternary operators 2nd and 3rd operands are LValue expressions
    // "(i >= j ? i : j)" is also LValue expression
    // decltype(LValue expr) -> T&
    // decltype((i >= j ? i : j)) is int&, decltype(auto) -> int&

    // returning a reference to automatic storage duration object
    // undefined behavior(ub)
  }

  struct S {
    int i = 0;
  };

  decltype(auto) fn_H()
  {
    return (S{});
    // "(S{})" is an expression, decltype(PRValue expr) -> T
    // decltype((S{})) is struct S, decltype(auto) -> struct S
  }

  // reaching R Value class objects non-static data members
  // is XValue expression
  decltype(auto) fn_I()
  {
    return (S{}.i);
    // "(S{}.i)" is an expression, decltype(XValue expr) -> T&&
    // decltype((S{}.i)) is int&&, decltype(auto) -> int&&

    // returning a reference to automatic storage duration object's
    // non-static data member -> undefined behavior(ub)
  }
*/

/*
  // ---- perfect returning ----

  #include <utility>  // std::forward

  // returning foo's return value as perfectly forwarded
  // to the caller of func

  template <typename T>
  decltype(auto) func(T&& val)
  {
    return foo(std::forward<T>(val));
  }

  // std::invoke() global function inside <functional> module

  template <typename... Args>
  decltype(auto) func_2(Args&&... args)
  {
    return foo(std::forward<Args>(args)...);
  }
*/

/*
  // ---- perfect returning with lambda expression----
  int main()
  {
    // using trailing return type syntax
    auto fn = [](auto&& r)-> decltype(auto){
      return foo(std::forward<decltype(r)>(r));  
    };
  }
*/

/*
  // ---- deferred perfect returning ----
  #include <type_traits>

  template <typename Func, typename... Args>
  decltype(auto) call(Func f, Args&&... args)
  {
    decltype(auto) ret{ f(std::forward<Args>(args)...) };

    // "ret" is an identifier and its data type can be 
    // - LValue reference type
    // - RValue reference type
    // - non-reference type


    // `if constexpr (std::is_rvalue_reference_v<decltype(ret)>)`
    //  --------------------------------------------------------
    // decltype(ret) -> decltype(identifier) 
    // if ret is LValue reference type(&)     -> T&
    // if ret is RValue reference type(&&)    -> T&&
    // if ret is non-reference type           -> T


    // ---- if ret's data type is R value reference type ----
    // inside return statement "std::move(ret)" is an expression
    // decltype(auto) needs to return RValue reference type to caller
    // for expressions decltype(auto)'s type will be the same as
    // decltype(expression)'s type and
    // decltype(XValue expression)'s type is T&&
    // "std::move(ret)" is an XValue expression

    // ---- if ret's data type is L value reference type ----
    // inside return statement, "ret" is an identifier
    // decltype(auto) needs to return LValue reference type to caller
    // for identifiers, decltype(auto)'s type will be
    // identifiers declaration type
    // if ret's declaration type T& -> decltype(auto)'s type will be T&

    // ---- if ret's data type is non-reference type(plain value) ----
    // inside return statement, "ret" is an identifier
    // decltype(auto) needs to return non-reference type to caller
    // for identifiers decltype(auto)'s type will be 
    // identifiers declaration type
    // if ret's declaration type T -> decltype(auto)'s type will be T

    if constexpr (std::is_rvalue_reference_v<decltype(ret)>)
      return std::move(ret);
      // move XValue expression returned by f() to the caller
    else
      return ret;
      // return the plain value or the lvalue reference
  }
*/

/*
  // ---- deferred perfect returning with lambda expression ----

  int main()
  {
    auto fn = [](auto func, auto&&... args) -> decltype(auto){
      decltype(auto) ret{ func(std::forward<decltype(args)>(args)...) };

      if constexpr (std::is_rvalue_reference_v<decltype(ret)>)
        return std::move(ret);
      else
        return ret;
    };
  }
*/

/*
  #include <vector>
  #include <string>

  std::vector<std::string> create_svec();

  int main()
  {
    const auto& r1 = create_svec();   // life extension

    auto& r2 = create_svec();   // syntax error
    // error: cannot bind non-const lvalue reference of type 
    // 'std::vector<std::__cxx11::basic_string<char>>&' to an 
    // rvalue of type 'std::vector<std::__cxx11::basic_string<char>>'

    std::vector<std::string>&& r3 = create_svec();   // life extension
  }
*/

/*
  #include <vector>
  #include <string>

  std::vector<std::string> create_svec();

  int main()
  {
    const auto& r1 = create_svec().at(0);   // NO life extension
    // r1, fonksiyonun geri dönüş değerine değil, fonksiyonun
    // geri döndürdüğü geçici nesnenin üye fonksiyonunun(at()) 
    // geri dönüş değeri olan referansa bağlanıyor.

    // r1 is not binding to the temporay object, it is binding 
    // to the return value of the temporary objects member function
  }
*/

/*
  #include <vector>

  class Myclass{
  public:
    ~Myclass()
    {
      std::cout << "object destructed\n";
    }
  private:
    std::vector<int> ivec{ 1, 2, 3, 4 };
  };
  
  Myclass foo()
  {
    return Myclass{};
  }

  int main()
  {
    {
      const auto& r1 = foo();     // life extension
      Myclass&& r2 = foo();       // life extension
      std::cout << "[1] main continues\n";
    }
    std::cout << "[2] main continues\n";

    // output -> 
    // [1] main continues
    // object destructed
    // object destructed
    // [2] main continues 
  }
*/

/*
  #include <vector>

  class Myclass{
  public:
    ~Myclass()
    {
      std::cout << "object destructed\n";
    }

    std::vector<int> get_vec() const
    {
      return ivec;  // NRVO
    }
  private:
    std::vector<int> ivec{ 1, 2, 3, 4 };
  };
  
  Myclass foo()
  {
    return Myclass{};
  }

  int main()
  {
    const auto& r = foo().get_vec();  // NO life extension
    std::cout << "[1] main continues\n";
    // output ->
    // object destructed
    // [1] main continues

    std::cout << r[0] << '\n';  // undefined behavior(ub)
    // r becomes a dangling reference
  }
*/

/*
  #include <vector>
  #include <string>

  std::vector<std::string> create_svec();

  int main()
  {
    for (std::string s : create_svec()){
      // VALID
    }

    for (char c : create_svec().at(0)){
      // undefined behavior(ub)
    }

    // auto&& rg = create_svec().at(0);
    // // ------> containers lifetime ends here
    // auto pos = rg.begin();   // rg is a dangling reference (ub)
    // auto end = rg.end();
    // for(; pos != end; ++pos)
    //   char c = *pos;

    for (char c : create_svec()[0]){
      // undefined behavior(ub)
    }

    for (char c : create_svec().front()){
      // undefined behavior(ub)
    }
  }
*/

/*
  class Myclass{
  public:
    std::string get_str() const
    {
      return m_str;
    }

    const std::string& get_str_ref() const
    {
      return m_str;
    }
  private:
    std::string m_str;
  };

  Myclass create_myclass();

  int main()
  {
    Myclass m;
    auto s = m.get_str();               // copy will be done
    const auto& s2 = m.get_str_ref();   // NO copy will be done

    for (auto c : create_myclass().get_str_ref()){
      // NO life extension
      // dangling reference -> undefined behaviour(ub)
    }
  }
*/

/*
                    -----------------
                    | ref qualifier |
                    -----------------
*/

/*
  #include <utility>  // std::move

  class Myclass{
  public:
    void foo();
    void bar()&;
    void baz()&&;
  };

  int main()
  {
    // ---------------------------------------------------------

    Myclass m;
    m.foo();              // VALID
    Myclass{}.foo();      // VALID
    std::move(m).foo();   // VALID

    // ---------------------------------------------------------

    m.bar();             // VALID
    Myclass{}.bar();     // syntax error
    // error: passing 'Myclass' as 'this' argument discards qualifiers
    std::move(m).bar();  // syntax error
    // error: passing 'std::remove_reference<Myclass&>::type' 
    // {aka 'Myclass'} as 'this' argument discards qualifiers

    // ---------------------------------------------------------
    m.baz();             // syntax error
    // error: passing 'Myclass' as 'this' argument discards qualifiers
    Myclass{}.baz();     // VALID
    std::move(m).baz();  // VALID

    // ---------------------------------------------------------
  }
*/

/*
  #include <utility>  // std::move

  class Myclass{
  public:
    // void foo(); // syntax error
    void foo()& 
    {
      std::cout << "foo()&\n";
    }

    void foo()&&
    {
      std::cout << "foo()&&\n";
    }

    void foo() const&
    {
      std::cout << "foo() const&\n";
    }

    void foo() const &&
    {
      std::cout << "foo() const&&\n";
    }
  };

  int main()
  {
    Myclass m;
    const Myclass cm;

    m.foo();              // output -> foo()&
    cm.foo();             // output -> foo() const&

    Myclass{}.foo();      // output -> foo()&&
    std::move(m).foo();   // output -> foo()&&
    std::move(cm).foo();  // output -> foo() const&&
  }
*/

/*
  #include <string>

  void foo(bool)
  {
    std::cout << "foo(bool)\n";
  }

  void foo(std::string)
  {
    std::cout << "foo(std::string)\n";
  }

  std::string get_str()
  {
    return "hello";
  }

  int main()
  {
    foo((get_str() == "hello"));  // output -> foo(bool)


    foo(get_str() = "hello");           // output -> foo(std::string)
    foo(get_str().operator=("hello"));  // output -> foo(std::string)
    // Those 2 lines are equivalent.

    // standart libraries standart classes operator() functions
    // are not ref qualified. (returns a reference to the object)
    // returns *this
  }
*/

/*
  class Myclass{
  public:
    void func()&;

    Myclass& operator=(const Myclass&)& = default;
  };

  int main()
  {
    Myclass mx;

    mx.func();              // VALID
    Myclass{}.func();       // syntax error

    mx = Myclass{};         // VALID
    Myclass{} = mx;         // syntax error  
    Myclass{} = Myclass{};  // syntax error
  }
*/

/*
  class Myclass{
  public:
    Myclass(const Myclass&) = default;
    Myclass(Myclass&&) = default;

    Myclass& operator=(const Myclass& str) & = default;
    Myclass& operator=(Myclass&& str) & = default;
  };

  Myclass foo();

  int main()
  {
    foo() = foo();  // syntax error
    // error: passing 'Myclass' as 'this' argument discards qualifiers 
  }
*/

/*
  #include <optional>
  #include <string>

  std::optional<std::string> foo(){}

  int main()
  {
    if (foo() = "hello")  // VALID
      ;

    if (foo() == "hello") // VALID
      ;
  }
*/

/*
  #include <string>
  #include <utility>  // std::forward

  class City{
  public:
    City(const std::string& name) : m_name(name){}

    std::string get_name()&&  // will be called for R value objects
    {
      std::cout << "get_name() && ";
      return std::move(m_name);
    }

    const std::string& get_name() const&
    {
      std::cout << "get_name() const& ";
      return m_name;
    }

    const std::string& get_name() &
    {
      std::cout << "get_name() & ";
      return m_name;
    }

  private:
    std::string m_name;
  };

  template <typename T>
  void foo(T&& t)
  {
    auto name = std::forward<T>(t).get_name();
    std::cout << name << '\n';
  }

  int main()
  {
    City c("Constantinople");   
    const City c_c("Istanbul");  

    foo(c);                   // output -> get_name() & Constantinople
    foo(c_c);                 // output -> get_name() const& Istanbul
    foo(City{"New York"});    // output -> get_name() && New York
  }
*/

/*
  template <typename T>
  void func(const T&&) {} 
  // It is not a universal reference, it is const R value reference

  class Myclass{};

  int main()
  {
    Myclass m;
    func(m);  // syntax error
    // error: cannot bind rvalue reference of type 'const Myclass&&' 
    // to lvalue of type 'Myclass'
  }
*/

/*
  #include <vector>

  template <typename Con>
  void func(Con &x, typename Con::value_type&&);
  // It is not a universal reference, it is R value reference

  int main()
  {
    std::vector<int> ivec;
    int x{};
    func(ivec, x);  // syntax error
    // error: cannot bind rvalue reference of type 
    // 'std::vector<int>::value_type&&' {aka 'int&&'} to 
    // lvalue of type 'int'
  }
*/

/*
  #include <vector>
  #include <string>

  template <typename T>
  class Stack{
  public:
    void push(const T& val)
    {
      std::cout << "L value overload\n";
      m_con.push_back(val);
    }

    // // not a universal reference, it is R value reference
    void push(T&& val)  
    {
      std::cout << "R value overload\n";
      m_con.push_back(std::move(val));
    }
  private:
    std::vector<T> m_con;
  };

  int main()
  {
    Stack<std::string> mystack;
    mystack.push(std::string{"PRValue expression"});

    std::string str{"LValue expression"};
    mystack.push(str);
  }
*/

/*
  template <typename T>
  class Myclass{
  public:
    template <typename U>
    void func(U&&);   // universal reference parameter
  };

  int main()
  {
    Myclass<int> mx;

    double dval = 6.7;
    const double cdval = 12.4;

    mx.func(dval);  // L value
    mx.func(cdval); // const L value
    mx.func(10);    // R value
  }
*/

/*
  #include <string>
  #include <vector>

  // primary template taking a universal reference
  template <typename Coll, typename T>
  void insert(Coll&& coll, T&& arg)
  {
    std::cout << "primary template for universal reference\n";
    coll.push_back(std::forward<T>(arg));
  }

  // full(explicit) specialization for const lvalues of type std::string
  template <>
  void insert(std::vector<std::string>& coll, const std::string& arg)
  {
    std::cout << "full specialization for type const std::string&\n";
    coll.push_back(arg);
  }

  // full specialization for non-const lvalues of type std::string
  template <>
  void insert(std::vector<std::string>& coll, std::string& arg)
  {
    std::cout << "full specialization for type std::string&\n";
    coll.push_back(arg);
  }

  // full specialization for non-const rvalues of type std::string
  template <>
  void insert(std::vector<std::string>& coll, std::string&& arg)
  {
    std::cout << "full specialization for type std::string&&\n";
    coll.push_back(std::move(arg));
  }

  // full specialization for const rvalues of type std::string
  template <>
  void insert(std::vector<std::string>& coll, const std::string&& arg)
  {
    std::cout << "full specialization for type const std::string&&\n";
    coll.push_back(std::move(arg));
  }
*/

/*
  template <typename T>
  void func(T&& t);

  class Myclass{};

  int main()
  {
    Myclass m;
    func(m);  
    // "m" is L value expression -> T& (T& && -> T&)
    // func's parameter variable is Myclass&(T&) 

    func<Myclass>(m);  // syntax error
    // error: cannot bind rvalue reference of type 'Myclass&&' 
    // to lvalue of type 'Myclass'

    func<Myclass&>(m);  
    // T's type is Myclass& 
    // func's parameter variable is (& - && -> &) Myclass&

    func<Myclass&&>(m); // syntax error
    // T's type is Myclass&&
    // func's parameter variable is (&& - && -> &&) Myclass&&

    // error: cannot bind rvalue reference of type 'Myclass&&' 
    // to lvalue of type 'Myclass' 

    func<Myclass&&>(Myclass{}); // VALID
    // T's type is Myclass&&
    // func's parameter variable is (&& - && -> &&) Myclass&&
  }
*/