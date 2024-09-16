#include <iostream>

/*
  "expression" has 2 main characteristics:
  1. It has a type.
  2. It has a value category.
*/

/*
  int main(){
    int x = 6;

    int& r = x;
    // "r" expression's type is int
    
    int* p = &x;
    // "p" expression's type is int*(pointer to int)
  }
*/

/*
  değişkenlerin(variables) türü referans türü olabilir.
  ifadelerin(expressions)  türü referans türü olamaz.

  <--- check value_categories.png --->
  Each expression has some non-reference type.
*/

/*
                      ====================
                      | Value Categories |
                      ====================
*/

/*
  int main(){
    int x = 6;
    int&& r = x + 4;

    // r variable's type is int&& (rvalue reference to int)
    // expression that r variable creates, is an lvalue expression.
  }
*/

/*
  C++ have 5 value categories:
  ----------------------------
    1. primary value categories 
      a. PRValue  (pure RValue)
      a. Lvalue   (locators value)
      b. Xvalue   (expiring value)

    // an expression should be related with one of those 
    // three main primary value categories.
  
    2. mixed (combined) value categories
      a. GLValue  (LValue  | XValue) 
      b. Rvalue   (PRValue | XValue)
*/

/*
  -----------------------------------------------------------------------
  <--- check identity_moveability.png --->

  *  -> primary value categories         [PRValue, LValue, XValue]
  ** -> mixed (combine) value categories [GLValue, RValue]
  -----------------------------------------------------------------------
  an expression's value category, 
  indicates whether the expression has an identity

  (bir ifadenin değer kategorisi, 
  o ifadenin bir kimliğe sahip olup olmadığını belirtir.)
  -----------------------------------------------------------------------
  <--- check value_categories2.png --->
  <--- check value_categories3.png --->
  -----------------------------------------------------------------------
  ** -> if an expression has an identity it's value category is [GLValue]
  there is a variable and our expression is representing that variable.

  identity means :
    that the expression has a memory location.
    can be an operand of the address operator(&) [there are some exceptions]
  -----------------------------------------------------------------------
  ** : if an expression can be moved from, it's value category is [RValue]
  -----------------------------------------------------------------------
  * : expression that has an identity, can not be moved(from)  [LValue]
  * : expression that has no identity, can be moved(from)      [PRValue]
  * : expression that has an identity, can be moved(from)      [XValue]
  -----------------------------------------------------------------------
*/

/*
  Those value categories used in 
    1. references (which value categories can bind or can not bind)
    2. type deduction
    3. function overload resolution 
      phase of which function will be selected and be called
*/

/*
  There are 3 types of reference categories in C++
    1. LValue reference
    2. RValue reference
    3. Universal(Forwarding) reference
*/

/*
  T&  r   -> LValue reference
  T&& r   -> RValue reference
*/

/*
                        ----------------------
                        | decltype specifier |
                        ----------------------
*/

/*
  decltype specifier has 2 main jobs:
  -----------------------------------
    1. decltype(x), decltype(x.a),  decltype(x->p)  
      -> using an identifier as an operand

    2. decltype(expression)
      -> decltype's operand will be an expression 
         which is not an identifier
*/

/*
  // 1. decltype(x), decltype(x.a),  decltype(x->p)  
  
  struct Myclass{
    int a;
  };

  int main()
  {
    int x = 10;
    int& r = x;
    const int& cr = x;
    int* ptr = &x;

    decltype(r)  decl_1 = x;    // decl_1's type is int&
    decltype(cr) decl_2 = x;    // decl_2's type is const int&

    Myclass m{};
    Myclass* pm{ &m };

    decltype(m.a)   decl_3{};   // decl_3's type is int
    decltype(pm->a) decl_4{};   // decl_4's type is int

    int&& b = 5;
    decltype(b) decl_5 = 5;     // decl_5's type is int&&
  }
*/

/*
  int&& foo();

  struct Mystruct{
    int x{};
  }

  int main(){
    // 2. decltype(expression)
    //  a. if expression's value category is PRValue -> T
    //  b. if expression's value category is LValue  -> T&
    //  c. if expression's value category is XValue  -> T&&

    decltype(10);     // PRValue -> (T = int) -> int

    int x = 3;
    decltype(x + 5);  // PRValue -> (T = int) -> int

    int*p{ &x };
    decltype(*p);     // LValue  -> (T& = int&) -> int&

    decltype((x));    // LValue  -> (T& = int&) -> int&

    decltype(foo());  // XValue  -> (T&& = int&&) -> int&&

    decltype(Mystruct{}.x);    // XValue  -> (T&& = int&&) -> int&& 
  }
*/

/*
  // const R value reference generally not being used but it is VALID 

  struct Myclass{
  public:
    void foo(); // non-const member function
  };

  int main(){
    const Myclass&& mr = Myclass{};
    mr.foo(); // syntax error
    // passing 'const Myclass' as 'this' argument discards qualifiers
  }
*/

/*
  // varible templates 

  template <typename T> constexpr const char* p = "PRValue";
  // primary template

  template <typename T> constexpr const char* p<T&> = "LValue";
  // partial specialization for LValue reference types

  template <typename T> constexpr const char* p<T&&> = "XValue";
  // partial specialization for RValue reference types

  int main(){
    std::cout << p<int> << '\n';      // output -> PRValue
    std::cout << p<int&> << '\n';     // output -> LValue
    std::cout << p<int&&> << '\n';    // output -> XValue

    std::cout << p<decltype(10)> << '\n';   // output -> PRValue

    int x = 10;
    std::cout << p<decltype((x))> << '\n';   // output -> LValue
    // we need to use parentheses to not get into 1. rule set
    // decltype(x)'s operand is an identifier
    // decltype((x))'s operand is an expression but not an identifier

    int* ptr = &x;
    std::cout << p<decltype(*ptr)> << '\n';   // output -> LValue

    // ------------------------------------------------------------------
    // decltype operator's operand is generating unevaluated context !!!!
    // p<decltype(*ptr)> > dereferencing operation is not being evaluated
    // ------------------------------------------------------------------
  }
*/

/*
  template <typename T> constexpr const char* p       = "PRValue";
  template <typename T> constexpr const char* p<T&>   = "LValue";
  template <typename T> constexpr const char* p<T&&>  = "XValue";


  #define   pvalcat(e)    std::cout << "value category of '" << \
                          #e << "' is " << p<decltype((e))> << '\n'

  int foo();   
  int& bar();  
  int&& baz();  
  void func();

  struct Mystruct{
    int x{};
    int&& y{};
  };

  struct Mystruct2{
    static int s_x;
  };

  int main(){
    int x = 10;
    int* ptr = &x;

    pvalcat(x);         // output -> value category of 'x' is LValue
    pvalcat(x + 5);     // output -> value category of 'x + 5' is PRValue
    pvalcat(x++);       // output -> value category of 'x++' is PRValue
    pvalcat(x--);       // output -> value category of 'x--' is PRValue
    pvalcat(++x);       // output -> value category of '++x' is LValue
    pvalcat(--x);       // output -> value category of '--x' is LValue
    pvalcat(foo());     // output -> value category of 'foo()' is PRValue
    pvalcat(bar());     // output -> value category of 'bar()' is LValue
    pvalcat(baz());     // output -> value category of 'baz()' is XValue

    // void expressions value category is PRValue
    pvalcat(func());    // output -> value category of 'func()' is PRValue

    // constants are PRValues
    pvalcat(45);        // output -> value category of '45' is PRValue
    pvalcat(3.14);      // output -> value category of '3.14' is PRValue
    pvalcat('A');       // output -> value category of ''A' is PRValue

    // string literals are LValues because they are arrays
    pvalcat("hello");   // output -> value category of '"hello"' is LValue

    // function identifiers are LValues
    pvalcat(foo);       // output -> value category of 'foo' is LValue

    // std::move functions return value &&(R value reference)
    pvalcat(std::move(x));   
    // output -> value category of 'std::move(x)' is XValue

    pvalcat(static_cast<int&&>(x));
    // output -> value category of 'static_cast<int&&>(x)' is XValue

    // functions are always been LValues  (MSVC compiler problem !!!)
    pvalcat(std::move(foo));
    // output -> value category of 'std::move(foo)' is LValue

    pvalcat(nullptr);  // output -> value category of 'nullptr' is PRValue

    // temp object expressions PRValue
    pvalcat(Mystruct{});  
    // output -> value category of 'Mystruct{}' is PRValue

    // lambda expressions are temp objects generated by compiler
    // so they are PRValues
    pvalcat([]{return 1;});
    // output -> value category of '[]{return 1;}' is PRValue

    // ------------------------------------------------------------

    // expressions which are variables are LValues
    Mystruct m;
    pvalcat(m);      // output -> value category of 'm' is LValue

    auto& ref{ m };
    auto&& uref = Mystruct{};
    pvalcat(ref);    // output -> value category of 'ref' is LValue
    pvalcat(uref);   // output -> value category of 'uref' is LValue

    // L value class object's non-static data members are LValues
    pvalcat(m.x);    // output -> value category of 'm.x' is LValue

    // R value class object's non-static data members are XValues
    pvalcat(Mystruct{}.x);
    // output -> value category of 'Mystruct{}.x' is XValue

    // R value class objects non-static data members are XValues
    pvalcat(std::move(m).x);
    // output -> value category of 'std::move(m).x' is XValue

    // R value class objects non-static data members 
    // which are references are LValues
    pvalcat(Mystruct{}.y);
    // output -> value category of 'Mystruct{}.y' is LValue

    Mystruct2 m2;

    // R value class object's static data members are LValues
    pvalcat(std::move(m2).s_x);
    // output -> value category of 'std::move(m2).s_x' is LValue
  }
*/

/*
  template <typename T> constexpr const char* p       = "PRValue";
  template <typename T> constexpr const char* p<T&>   = "LValue";
  template <typename T> constexpr const char* p<T&&>  = "XValue";


  #define   pvalcat(e)    std::cout << "value category of '" << \
                          #e << "' is " << p<decltype((e))> << '\n'

  int main(){
    int x{}, y{}, z{};

    // because of operands are LValues, common type is LValue
    pvalcat(x > 10 ? y : z);
    // output -> value category of 'x > 10 ? y : z' is LValue

    pvalcat(x > 10 ? y : 10);
    // output -> value category of 'x > 10 ? y : 10' is PRValue

    pvalcat(x > 10 ? 10 : y);
    // output -> value category of 'x > 10 ? 10 : y' is PRValue
  }
*/

/*
  #include <type_traits>
  // used for generic programming
  // metafunctions in this header 
  // are calculating type or value at compile time

  // is_rvalue_reference    -> metafunction
  // is_rvalue_reference_v  -> variable template

  // is_rvalue_reference<T>::value
  // is_rvalue_reference_v<T>
  // Those 2 lines are same


  template <typename T>
  void func()               // function template
  {
    // static(compile time)-if (C++17) 
    if constexpr(std::is_rvalue_reference_v<T>)
      std::cout << "X value\n";
    else if constexpr (std::is_lvalue_reference_v<T>)
      std::cout << "L value\n";
    else if constexpr (std::is_reference_v<T>)
      std::cout << "PR value\n";
  }

  #define  exprvalcat(e)    decltype((e))

  int main()
  {
    func<exprvalcat(10)>();     // output -> PR value

    int x{ 43 };
    func<exprvalcat(x)>();      // output -> L value
  }
*/

/*
  struct Myclass{};

  int main(){
    Myclass x;
    const Myclass cx;

    Myclass& r = x;
    // only non-const L value expression can bind 
    // to non-const LValue reference 

    Myclass& r2 = cx;         // syntax error
    Myclass& r3 = Myclass{};  // syntax error
    // trying to bind const LValue expression or R value expression
    // is not allowed to non-const L value reference
  }
*/

/*
  struct Myclass{};

  int main(){
    Myclass x;
    const Myclass cx;

    const Myclass& r = x;                 // VALID
    const Myclass& r2 = cx;               // VALID
    const Myclass& r3 = Myclass{};        // VALID
    const Myclass& r4 = std::move(cx);    // VALID
    // 1. non-const L value expression 
    // 2. const L value expression
    // 3. non-const R value expression 
    // 4. const R value expression
    // can bind to const LValue reference 
  }
*/

/*
  struct Myclass{};

  int main(){
    Myclass x;
    const Myclass cx;

    Myclass&& r = Myclass{};              // VALID
    // non-const R value expressions can bind to 
    // non-const R value reference

    Myclass&& r2 = std::move(cx);         // syntax error
    // const R value expressions can not 
    // bind to non-const R value reference

    Myclass&& r2 = x;                     // syntax error
    Myclass&& r2 = cx;                    // syntax error
    // L value expressions(const or non-const) 
    // can not bind to R value reference 
  }
*/

/*
  struct Myclass{};

  int main(){
    Myclass x;
    const Myclass cx;

    const Myclass&& r = Myclass{};              // VALID
    const Myclass&& r2 = std::move(cx);         // VALID
    // const or non-const R value expressions can bind to 
    // const R value reference

    const Myclass&& r2 = x;                     // syntax error
    const Myclass&& r2 = cx;                    // syntax error
    // L value expressions(const or non-const) 
    // can not bind to R value reference 
  }
*/

/*
  ----------------------------------------------------------------------------
  |               |    T&     |    const T&     |    T&&      |   const T&&  |
  ----------------------------------------------------------------------------
  | LValue        |     1     |        2        |     X       |       X      |
  ----------------------------------------------------------------------------
  | const LValue  |     X     |        1        |     X       |       X      |   
  ----------------------------------------------------------------------------
  | RValue        |     X     |        3        |     1       |       2      |
  ----------------------------------------------------------------------------
  | const RValue  |     X     |        2        |     X       |       1      |
  ----------------------------------------------------------------------------
*/

/*
  struct S{};

  void foo(S&){ std::cout << "S&" << '\n';}
  void foo(const S&){ std::cout << "const S&" << '\n';}
  void foo(S&&){ std::cout << "S&&" << '\n';}
  //void foo(const S&&){ std::cout << "const S&&" << '\n';}

  // if non of the overloads is chosen it will be error
  // if there is an ambiguity, syntax error

  int main(){
    S x;

    // SCENARIO 1
    foo(x);  // output -> S&
    // if we commented out `void foo(S&)` overload  
    // const S& will be choosen

    // SCENARIO 2
    const S cx;
    foo(cx);  // output -> const S&

    // SCENARIO 3
    foo(S{});  // output -> S&&
    // if we commented out `void foo(S&&)` overload  
    // const S&& will be choosen
    // if we also commented out `void foo(const S&&)` overload
    // const S& will be choosen

    // SCENARIO 4
    foo(std::move(cx));  // output -> const S&&
    // if we commented out `void foo(const S&&)` overload
    // const S& will be choosen
  }
*/

/*
  void push_back(const T&);   // for copying
  void push_back(T&&);        // for moving

  class Myclass{
  public:
    Myclass(const Myclass&){ std::cout << "copy ctor\n"; }
    Myclass(Myclass&&){ std::cout << "move ctor\n"; }
  };
*/

/*
  class Myclass{};

  int main(){
    Myclass m;

    Myclass&& r = std::move(m);


    Myclass m2 = r; 
    // "r" expression's type is Myclass&&
    // "r" expression's value category is LValue expression
    // copy ctor will be called for m2 object

    Myclass m3 = std::move(r); 
    // move ctor will be called for m3 object

    Myclass m4;
    m4 = r;             // copy assignment will be called for m4 object
    m4 = std::move(r);  // move assignment will be called for m4 object
  }
*/

/*
  special member functions 
    a. not declared 
    b. user declared
      1. user declared to be defined
      2. user declared deleted
      3. user declared defaulted
    c. implicitly declared (compiler implicitly declares)
      1. implicitly declared defaulted
      2. implicitly declared deleted

  for more information check repo: CPPLessons/SpecialMemberFunctions
*/

/*
                        -------------
                        | std::move | 
                        -------------
  an alternative to std::copy algorithm <algorithm> header
*/

/*
  #include <algorithm>

  template <typename InIter, typename OutIter>
  OutIter Copy(InIter beg, InIter end, OutIter destbeg){
    while(beg != end)
      *destbeg++ = *beg++;
    
    return destbeg;
  }

  template <typename InIter, typename OutIter>
  OutIter Move(InIter beg, InIter end, OutIter destbeg){
    while(beg != end)
      *destbeg++ = std::move(*beg++);
    
    return destbeg;
  }
  // if Move algorithm is used
  // if source range elements are class type objects
  // those elements will become moved-from state
*/

/*
                        -------------
                        | std::move | 
                        -------------
  value category conversion function <utility> header
*/

/*
  - if L Value comes to std::move function
      it will be converted to R value
  
  - if R Value comes to std::move function
      it will stay as R value

  static_cast<T&&>(exp);
  std::move(exp);
  // Those 2 lines are equivalent 
  // but we need to add <utility> header to use std::move function
*/

/*
  template <typename T>
  constexpr ??? Move(T&&) noexcept  
  {
    // ---------------------------------------------------
    // function's parameter must be a reference
    // if R value argument has been sent to function
    // function's parameter will be R value reference

    // if L value argument has been sent to function
    // function's parameter will be L value reference
    // ---------------------------------------------------

    // T's type is deduced to L value reference 
    // if L value argument is sent

    // T's type is deduced to non reference type 
    // if R value argument is sent
    // ---------------------------------------------------
  }
  // Move function have forwarding(universal) reference parameter
*/
