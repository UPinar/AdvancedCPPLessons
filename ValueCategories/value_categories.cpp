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
    - an expression's value category, 
      indicates whether the expression has an identity

    - bir ifadenin değer kategorisi, 
      o ifadenin bir kimliğe sahip olup olmadığını belirtir.)
  -----------------------------------------------------------------------
                <--- check value_categories2.png --->
                <--- check value_categories3.png --->
  -----------------------------------------------------------------------
  ** : if an expression has an identity it's value category is [GLValue]
  there is a variable and our expression is representing that variable.

  identity means :
    that the expression has a memory location.
    can be an operand of the address operator(&) 
    [there are some exceptions]
                  ------------------------------------
  ** : if an expression can be moved from, it's value category is [RValue]
  -----------------------------------------------------------------------
  * : expression that has an identity, can not be moved(from)  [LValue]
                  ------------------------------------
  * : expression that has no identity, can be moved(from)      [PRValue]
                  ------------------------------------
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

    // --------------------------------------------------------------
    // decltype operator's operand is generating unevaluated context 
    // p<decltype(*ptr)>> 
    //  dereferencing operation will not be evaluated
    // --------------------------------------------------------------
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

    pvalcat(x);       // output -> value category of 'x' is LValue
    pvalcat(x + 5);   // output -> value category of 'x + 5' is PRValue
    pvalcat(x++);     // output -> value category of 'x++' is PRValue
    pvalcat(x--);     // output -> value category of 'x--' is PRValue
    pvalcat(++x);     // output -> value category of '++x' is LValue
    pvalcat(--x);     // output -> value category of '--x' is LValue
    pvalcat(foo());   // output -> value category of 'foo()' is PRValue
    pvalcat(bar());   // output -> value category of 'bar()' is LValue
    pvalcat(baz());   // output -> value category of 'baz()' is XValue

    // void expressions value category is PRValue
    pvalcat(func());  // output -> value category of 'func()' is PRValue

    // constants are PRValues
    pvalcat(45);      // output -> value category of '45' is PRValue
    pvalcat(3.14);    // output -> value category of '3.14' is PRValue
    pvalcat('A');     // output -> value category of ''A' is PRValue

    // string literals are LValues because they are arrays
    pvalcat("hello"); // output -> value category of '"hello"' is LValue

    // function identifiers are LValues
    pvalcat(foo);     // output -> value category of 'foo' is LValue

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
  ----------------------------------------------------------------------
  |               |   T&     |   const T&    |    T&&    |  const T&&  |
  ----------------------------------------------------------------------
  | LValue        |    1     |       2       |     X     |      X      |
  ----------------------------------------------------------------------
  | const LValue  |    X     |       1       |     X     |      X      |
  ----------------------------------------------------------------------
  | RValue        |    X     |       3       |     1     |      2      |
  ----------------------------------------------------------------------
  | const RValue  |    X     |       2       |     X     |      1      |
  ----------------------------------------------------------------------
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
  - if L Value expression comes to std::move function
      it will be converted to R value expression
  
  - if R Value expression comes to std::move function
      it will stay as R value expression

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

/*
              -----------------------------------
              | universal(forwarding) reference |
              -----------------------------------
*/

/*
  template <typename T>
  void func(T&& r); // universal(forwarding) reference

  template <typename T>
  void func(const T&& r); // NOT universal reference

  template <typename T>
  class Myclass{
  public:
    void foo(T&&); // NOT universal reference
  };

  template <typename T>
  class Myclass{
  public:
    template <typename U> // member template
    void foo(U&&); // universal reference
  };

  template <typename T>
  class Myclass{
  public:
    template <typename ...Args>
    void emplace_back(Args&&... args); // universal reference
    // will be used for perfect forwarding
    // std::vector classes emplace functions
  };
*/

/*
  #include <vector>

  template <typename T>
  void func(std::vector<T>&&);  // NOT universal reference  
*/

/*
  template <typename T>
  class Myclass{
  public:
    struct  Mystruct{
    }
  };

  template <typename T>
  void func(Myclass<T>::Mystruct&&);  // NOT universal reference
*/

/*
  int main(){
    auto&& x = 10;  // universal reference

    for(auto&& x : container){
      // universal reference
    }
  }
*/

/*
                      ------------------------
                      | reference collapsing |
                      ------------------------
*/

/*
      ----------------
      |   T&    &    |  -> T&
      |   T&    &&   |  -> T&
      |   T&&   &    |  -> T&
      |   T&&   &&   |  -> T&&
      ----------------
*/

/*
  1. template parameters forwarding reference
  2. auto&&
  2. type alias declarations(using)
  3. decltype specifier
*/

/*
  template <typename T>
  void func(T&&){
  }
*/

/*
  class Myclass{};

  using LREF = Myclass&;
  using RREF = Myclass&&;

  int main(){
    LREF& r1 = Myclass{};  // syntax error 
    // reference collapsing rule & & -> & 
    // LValue reference can not bind to RValue expression

    // error: cannot bind non-const lvalue reference of type 'Myclass&' 
    // to an rvalue of type 'Myclass'

    Myclass m;
    RREF&&  r6 = m;           // syntax error
    // cannot bind rvalue reference of type 'RREF' {aka 'Myclass&&'} 
    // to lvalue of type  Myclass'

    LREF&   r2 = m;           // VALID    &   &   -> &
    LREF&&  r3 = m;           // VALID    &   &&  -> & 
    RREF&   r4 = m;           // VALID    &&  &   -> &
    RREF&&  r5 = Myclass{};   // VALID    &&  &&  -> && 
  }
*/

/*
  int main(){
    int x{ 234 };
    int* ptr = &x;

    decltype(*ptr)& r = x;
    // *ptr -> LValue expression 
    // decltype(LValue expression) -> LValue reference
    // int& & -> reference collapsing -> &  &  -> &
    // r is LValue reference
  }
*/

/*
  int&& foo();

  int main(){
    decltype(foo())&& x = 10;
    // foo() -> XValue expression
    // decltype(XValue expression) -> RValue reference
    // int&& && -> reference collapsing -> &&  && -> &&
    // x is RValue reference
  }
*/

/*
  #include <string>

  std::string foo();

  int main(){
    std::string str{"hello world"};
    auto& r = str;
    std::string&& r2 = foo(); // life extension

    // "str", "r" and "r2" expression's value category
    // is LValue expression
  }
*/

/*
  #include <utility>
  #include <string>

  // to use it for move operation
  void func(std::string&& s){

    auto str1 = s; 
    // "s" is an identifier
    // "s" expression's value category is LValue expression
    // for str1 std::string's copy constructor will be called

    auto str2 = std::move(s);
    // "std::move(s)" is RValue expression
    // for str2, std::string's move constructor will be called
  }

  // to use it for copy operation
  void func(const std::string&){}
*/

/*
  #include <array>
  #include <vector>

  class Myclass{
  private:
    std::array<int, 500> mx;
  };
  // Myclass does not have a member that stored in heap memory 

  // Myclass class to have a move constructor, 
  // does not create an advantage for move operation, 
  // move operation will become a copy operation


  class Myclass2{
  private:
    std::vector<int> mx;
  };
  // Myclass2 class has a member that stored in heap memory(std::vector)

  // Myclass2 class to have a move constructor, 
  // does create an advantage for move operation
*/

/*
  #include <string>

  compilers does small string optimization 
  when string is small it will be stored in stack memory
  so move operation will not be useful (won't create advantage)
*/

/*
  // Best scenario is copy elision

  #include <string>

  int main(){
    std::string{"hello world"}; 
    // std::string{"hello world"} is PRValue expression

    auto str = std::string{"hello world"};
    // temporary materialization happens 
    // std::string{"hello world"} expression becomes XValue expression
  }
*/

/*
  #include <string>
  #include <vector>
  #include <utility>  // std::move

  int main(){
    using namespace std;

    string str(100'000, 'A'); 
    // NO SBO-SSO(small buffer(string) optimization)


    // SCENARIO : we will not be use str anymore so better move 
    // ---------------------------------------------------------

    vector<string> svec;
    svec.push_back(str); 
    // becuase of "str(identifier)" is LValue expression
    // vector's copy constructor will be called

    svec.push_back(move(str));
    // "move(str)" expression is RValue expression
    // vector's move constructor will be called

    // ---------------------------------------------------------
    // after move operation str object will be in moved-from state
  }
*/

/*
  #include <fstream>
  #include <vector>
  #include <string>
  #include <utility>  // std::move

  int main(){

    std::ifstream ifs{"deneme.txt"};

    if (!ifs){
      std::cerr << "file could not be opened\n";
      return 1;
    }

    std::vector<std::string> svec;

    std::string sline;
    while (getline(ifs, sline)){
      svec.push_back(move(sline));
    }
    // using moved-from state object(sline) again and again
  }
*/

/*
  - moved-from state does not guaranteed to be as same as default 
  initialized object state.
  - but for containers in standart library moved-from state is
  is generally same as default initialized object state.
  (not technically guaranteed)
*/

/*
  #include <string>
  #include <utility>  // std::move

  int main(){

    using namespace std;

    string str(100'000, 'A'); // fill constructor
    cout << "str.size() : " << str.size() << '\n';
    // output -> str.size() : 100000

    auto s = move(str);
    cout << "str.size() : " << str.size() << '\n';
    // output -> str.size() : 0
  }
*/

/*
  #include <type_traits>

  // remove_reference -> type transformation metafunction

  // primary template
  template <typename T>
  struct RemoveReference{
    using type = T;
  };

  // partial specialization for LValue reference types
  template <typename T>
  struct RemoveReference<T&>{
    using type = T;
  };

  // partial specialization for RValue reference types
  template <typename T>
  struct RemoveReference<T&&>{
    using type = T;
  };

  // alias template for RemoveReference
  template <typename T>
  using RemoveReference_t = typename RemoveReference<T>::type; 
  // (before C++20)

  // template <typename T>
  // using RemoveReference_t = RemoveReference<T>::type;       
  // (after C++20)

  int main(){
    RemoveReference<int>::type x{};        // int
    RemoveReference<int&>::type y{};       // int
    RemoveReference<int&&>::type z{};      // int
  }
*/

/*
  #include <type_traits>

  template <typename T>
  std::remove_reference_t<T>&& Move(T&& t)
  {
    return static_cast<std::remove_reference_t<T>&&>(t);
  }

  // std::remove_reference_t<T>&&
  // return value type must be RValue reference
  // function's parameter is universal reference
*/

/*
                ---------------------------------------
                | special member functions (REMINDER) |
                ---------------------------------------
*/

/*
  - not declared (fonksiyonun olmaması)
  - user declared 
    - user declared to be defined
    - user declared deleted
    - user declared defaulted
  implicitly declared
    - implicitly declared defaulted
    - implicitly declared deleted
*/

/*
  struct Myclass{
    // default constructor : implicitly declared defaulted
  };

  // if we did not declare any special member functions
  // compiler will declare them implicitly
*/

/*
  struct Myclass{
    Myclass(int);
    // default constructor : not declared
  };
  // if we declare a constructor other than default constructor
  // default constructor will become not declared
*/

/*
  struct Myclass{
    Myclass();
    Myclass(const Myclass&);
    // move constructor : not declared
    // move assignment  : not declared
  };

  // if we declare one of copy members
  // (copy constructor OR copy assignment)
  // or a destructor, 
  // move members(move constructor OR move assignment) 
  // become not declared
*/

/*
  struct Myclass{
    Myclass();            // user declared to be defined
    Myclass() = default;  // user declared defaulted
    Myclass() = delete;   // user declared deleted  
  };
*/

/*
  struct Myclass{
    Myclass(const Myclass&) = delete;
    Myclass& operator=(const Myclass&) = delete;

    // copy constructor : user declared deleted
    // copy assignment  : user declared deleted
  };
  // because of copy members are user declared 
  // move members are not declared
  // so Myclass is non copyable and non movable
*/

/*
  struct Myclass1{
    Myclass1(const Myclass1&) = delete;
    Myclass1& operator=(const Myclass1&) = delete;
    Myclass1(Myclass1&&);
    Myclass1& operator=(Myclass1&&);
  };

  struct Myclass2{
    Myclass2(Myclass2&&);
    Myclass2& operator=(Myclass2&&);
  };
  // when one of move members is user declared
  // copy members will be deleted implicitly

  // so there is no difference between Myclass1 and Myclass2
  // but better using Myclass1 in move only types for readability
*/

/*
  - deleted function is exists but when it has been called it will be 
  syntax error.
  
  - deleted function is in function overload set but not declared 
  function is not in function overload set.
*/

/*
  void func(int)  = delete;
  void func(double);
  void func(float);
  void func(long);

  // in func function's overload set, there are 4 functions.

  int main(){
    func('A');  // char -> int (integral promotion) 
    // func(int) is deleted -> syntax error 
    // error: use of deleted function 'void func(int)'

    func(23u);  
    // all functions in overload set are viable -> syntax error
    // error: call of overloaded 'func(unsigned int)' is ambiguous
  }
*/

/*
  Question : In which scenarios we should delete move members ?
  Answer   : Never delete move members !!!

  - if move members are not declared, it will fallback to copy members 
  - if move members are deleted, copy members can not be called!
*/

/*
  #include <string> 

  class Myclass{
  public:
    Myclass() = default; // implicitly declared defaulted 

    // compiler will generate default constructor, 
    // will user default member initializers
  private:
    int mx{};
    std::string str{};
  };
*/

/*
  // there is no necessity to default a special member function in
  // public section of class

  class Myclass{
  public:

  protected:
    Myclass(const Myclass&) = default;
  };
*/

/*
  class Myclass{
  public:
    Myclass& operator=(const Myclass&)& = default;
    // compiler will generate copy assignment operator
    // with a reference qualifier.

    Myclass& operator=(Myclass&&)noexcept = default;
    // compiler will generate move assignment operator
    // with a noexcept specifier.
  };
*/

/*
  // myclass.h
  // ----------
  class Myclass{
  public:
    ~Myclass(); // user declared destructor
  };


  // myclass.cpp
  // ----------
  Myclass::~Myclass() = default;  // VALID

  // PIMPLE idioms implementation with unique_ptr 
  // complete type - incomplete type
*/

/*
  - compiler will write the code of special member functions
    which are user declared defaulted or implicitly declared defaulted.
    if there will be any situation that will create syntax error
    when the compiler is writing that function's code, it will
    implicitly delete that special member function.

    -> a call to some classes private constructor 
    -> a call to base classes private member function
    -> if a member class does not have a default constructor but 
      it is needed to default construct.
*/

/*
  class Myclass{
  public:
    // default constructor that compiler will generate is
    // default initialize non-static data members the class.

    // const variables can not be default initialized.
  private:
    const int mx;
  };

  int main(){
    Myclass m;  // syntax error
    // error: use of deleted function 'Myclass::Myclass()'
    // error: uninitialized const member in 'class Myclass'
  }
*/

/*
  class Myclass{
  public:

  private:
    int& mx;
  };

  int main(){
    Myclass m;  // syntax error
    // error: use of deleted function 'Myclass::Myclass()'
    // error: uninitialized reference member in 'class Myclass'
  }
*/

/*
  class Member{
  public:
    Member(int);
  };

  class Myclass{
  private:
    Member mx;
  };

  int main(){
    Myclass m;  // syntax error
    // error: use of deleted function 'Myclass::Myclass()'
    // error: no matching function for call to 'Member::Member()'

    // Myclass's default constructor is implicitly declared defaulted
    // compiler generated default constructor will default initialize
    // Member data member, 
    // but Member class does not have default constructor
    // this will cause syntax error and compiler will 
    // implicitly delete Myclass's default constructor
  }
*/

/*
  class Member{
  protected:
    Member();
  };

  class Myclass{
  private:
    Member mx;
  };

  int main(){
    Myclass m;  // syntax error
    // error: use of deleted function 'Myclass::Myclass()'
    // error: 'Member::Member()' is protected within this context

    // Myclass's default constructor is implicitly declared defaulted
    // compiler generated default constructor will default initialize
    // Member data member, but Member's default constructor is protected
    // this will cause syntax error and compiler will 
    // implicitly delete Myclass's default constructor
  }
*/

/*
  class Base{
  public:
    Base(int);
  };

  class Myclass : public Base{};

  int main(){
    Myclass m;  // syntax error
    // error: use of deleted function 'Myclass::Myclass()'
    // error: no matching function for call to 'Base::Base()'

    // Myclass's default constructor is implicitly declared defaulted
    // compiler generated default constructor need to call 
    // Base class's default constructor because it has a Base class
    // object inside of it, 
    // but Base class does not have a default constructor
    // this will cause syntax error and compiler will
    // implicitly delete Myclass's default constructor
  }
*/

/*
  #include <type_traits>  // std::is_nothrow_default_constructible_v
  #include <utility>      // std::declval

  class Class1{};
  class Class2{};

  class Myclass{
  private:
    Class1 mx;
    Class2 my;
  };

  int main(){
    // Question : Does Myclass's default constructor 
    //            have noexcept specifier?

    constexpr auto b = std::is_nothrow_default_constructible_v<Myclass>;  
    std::cout << std::boolalpha << b << '\n'; // output -> true

    constexpr bool x = noexcept(std::declval<Myclass>());
    std::cout << std::boolalpha << x << '\n'; // output -> true
  }
*/

/*
  #include <type_traits>  // std::is_nothrow_default_constructible_v

  class Class1{
  public:
    Class1(); // user declared default constructor
    // not giving noexcept guarantee
  };
  class Class2{};

  class Myclass{
  private:
    Class1 mx;
    Class2 my;
  };

  int main(){
    // Question : Does Myclass's default constructor 
    //            have noexcept specifier?

    constexpr auto b = std::is_nothrow_default_constructible_v<Myclass>;  
    std::cout << std::boolalpha << b << '\n'; // output -> false

    // because of Class1's default constructor does not 
    // have noexcept specifier compiler can not give nothrow 
    // guarantee to Myclass's default constructor
    // because if Class1's default constructor can throw an exception
    // Myclass's default constructor can also throw an exception
  }
*/

/*
  #include <type_traits>  // std::is_nothrow_default_constructible_v

  class Class1{
  public:
    Class1(); 
  };
  class Class2{};

  class Myclass{
  public:
    Myclass()noexcept = default; // implicitly declared defaulted
  private:
    Class1 mx;
    Class2 my;
  };

  int main(){
    constexpr auto b = std::is_nothrow_default_constructible_v<Myclass>;  
    std::cout << std::boolalpha << b << '\n'; // output -> true
  }
*/

/*
  #include <string>

  class Myclass{

  private:
    int mx{};
    std::string mname{ "noname yet" };
    // in-class initializer, default member initializer
  };

  // if we did not initialize mx and mname members in our constructor
  // compiler will initialize them with the values inside 
  // default member initiazers

  // when default ctor is implicitly declared defaulted, ctor 
  // will default initiazlize data members of the class
  // but if we use in-class initializer(default member initializer)
  // compiler will use those values to initialize data members 
*/

/*
  #include <string>

  class Myclass{
  public:
    void print()const{
      std::cout << "mx : " << mx << " mname : (" << mname << ")\n";
    }
  private:
    int mx;
    std::string mname;
  };

  int main(){
    Myclass m;  // default initialization
    m.print();  // undefined behaviour(UB)
    // mx -> indeterminate value, mname -> ""

    // when m object(Myclass type) is default initialized
    // its class type data member's(std::string) default ctor
    // will be called
  }
*/

/*
  #include <string>

  class Myclass{
  public:
    void print()const{
      std::cout << "mx : " << mx << " mname : (" << mname << ")\n";
    }
  private:
    int mx;
    std::string mname;
  };

  int main(){
    Myclass m{};  // value initialization -> first step zero init
    m.print();    // output -> mx : 0 mname : ()
    // mx -> 0, mname -> ""
  }
*/

/*
  #include <string>

  class Myclass{
  public:
    void print()const{
      std::cout << "mx : " << mx << " mname : (" << mname << ")\n";
    }
  private:
    int mx{ 34 };
    std::string mname{ "Istanbul" };
  };

  int main(){
    Myclass m;  // default initialization
    m.print();  // output -> mx : 34 mname : (Istanbul)
  }
*/

/*
  #include <string>

  class Myclass{
  public:
    Myclass() = default;  // user declared defaulted
    void print()const{
      std::cout << "mx : " << mx << " mname : (" << mname << ")\n";
    }
  private:
    int mx{ 34 };
    std::string mname{ "Istanbul" };
  };

  int main(){
    Myclass m;  // default initialization
    m.print();  // output -> mx : 34 mname : (Istanbul)
  }
*/

/*
  #include <string>

  class Myclass{
  public:
    Myclass() {}  // user declared defined
    void print()const{
      std::cout << "mx : " << mx << " mname : (" << mname << ")\n";
    }
  private:
    int mx{ 34 };
    std::string mname{ "Istanbul" };
  };

  int main(){
    Myclass m;  // default initialization
    m.print();  // output -> mx : 34 mname : (Istanbul)


    // when Myclass's default constructor is user declared defined
    // Myclass() {} will act like
    // Myclass() : mx{ 34 }, mname{ "Istanbul" } {}
  }
*/

/*
  #include <string>

  class Myclass{
  public:
    Myclass() : mx(98) {}  // user declared defined
    void print()const{
      std::cout << "mx : " << mx << " mname : (" << mname << ")\n";
    }
  private:
    int mx{ 34 };
    std::string mname{ "Istanbul" };
  };

  int main(){
    Myclass m;  // default initialization
    m.print();  // output -> mx : 98 mname : (Istanbul)

    // when Myclass's default constructor is user declared defined
    // Myclass() : mx(98) {} will act like
    // Myclass() : mx{ 98 }, mname{ "Istanbul" } {}
  }
*/

/*
          ------------------------------------------------------
          | noexcept specifier |  noexcept operator (REMINDER) |
          ------------------------------------------------------
*/

/*
  #include <type_traits>  // std::is_nothrow_copy_constructible_v

  void func(int x)noexcept;
  // func function is giving nothrow guarantee

  void func(int x)noexcept(true);
  // func function is giving nothrow guarantee

  void func(int x)noexcept(sizeof(int) < 8);
  // func function is giving nothrow guarantee 
  // when sizeof(int) < 8 in the system

  template <typename T>
  void func(T x)noexcept(std::is_nothrow_copy_constructible_v<T>);
  // if T type's copy constructor is giving nothrow guarantee
  // func function also is giving nothrow guarantee
*/

/*
  int main(){
    using namespace std;

    // noexcept is compile time operator 
    // (unevaluated context -> no operation code will be generated)

    constexpr auto b = noexcept(cout << 1);     // b is false
    // "cout << 1" is an expression 

    constexpr auto b2 = noexcept(10 + 20);      // b2 is true
    // integer addition is nothrow guarantee, so b2 is true
  }
*/

/*
  template <typename T>
  void func(T x)noexcept(noexcept(x + x));

  //  void func(T x)noexcept(noexcept(x + x));
  // if x + x expression has nothrow guarantee
  //  void func(T x)noexcept(true);
  //  void func(T x)noexcept;
  // Those 2 lines are equivalent

  //  void func(T x)noexcept(noexcept(x + x));
  // if x + x expression DOES NOT HAVE nothrow guarantee
  //  void func(T x)noexcept(false);
  //  void func(T x);
  // Those 2 lines are equivalent
*/

/*
  #include <string>

  template <typename T>
  void func(T x)noexcept(noexcept(x + x));

  struct Myclass{
  public:
    Myclass operator+(const Myclass&);  
    // member operator+() function
    // better declare it as global, but for this examples sake
  };

  struct Myclass2{
  public:
    Myclass2 operator+(const Myclass2&)noexcept;  
  };

  int main(){
    constexpr bool b = noexcept(func(10));  // b is true
    // integer addition is nothrow guarantee

    std::string s;
    constexpr bool b2 = noexcept(func(s));  // b2 is false
    // std::string classes operator+() function 
    // does not giving nothrow guarantee

    Myclass m;
    constexpr bool b3 = noexcept(func(m));  // b3 is false

    Myclass2 m2;
    constexpr bool b4 = noexcept(func(m2));  // b4 is true
  }
*/

/*
  // <-------------------- REMINDER --------------------> 

  int foo(int);

  int main(){
    int x = 10;
    // "int x = 10;" there is no expression in here it is a statement
    // we can not talk about value category of a statement
    // value category is a property of expressions
    // we can talk about declaration type of "x" identifier

    int b = 0;

    foo(x);  
    // "foo(x)" is an expression 
    // "foo(x) expression's data type is function's return type -> int
    // "x" is sub expression (alt ifade) of "foo(x)" expression
    // "x" expression's data type is int
    // "x" expression's value category is LValue expression


    b = x;
    // "b = x" is an expression
    // "b" and "x" are sub expressions of "b = x" expression
    // "x" value category is LValue expression
    // "x"'s data type is int

    b = +x;
    // "b = +x" is an expression
    // "+x" and "b" is sub expression of "b = +x" expression
    // "+x" expression's value category is PRValue expression
    // "+x" expression's data type is int

    b = x + 5;
    // "b = x + 5" is an expression
    // "b", "x + 5" are sub expressions of "b = x + 5" expression
    // "x + 5" expression's value category is PRValue expression
    // "x + 5" expression's data type is int

    auto y = x + 5;
    // "x + 5"  is an expression
    // "x + 5" expression's value category is PRValue expression
    // "x + 5" expression's data type is int

    auto z = x + 5.;
    // "x + 5." is an expression
    // "x + 5." expression's value category is PRValue expression
    // "x + 5." expression's data type is double
  }
*/

/*
  // <-------------------- REMINDER --------------------> 

  int main()
  {
    char c = 'A';
    // "c" declaration data type is char
    // "char c = 'A'" is a declaration statement not an expression
    // we can not talk about value category of a statement

    c;
    // "c" is an expression
    // "c" expression's data type is char
    // "c" expression's value category is LValue expression

    +c;
    // "+c" is an expression
    // "+c" expression's data type is int(integral promotion)
    // "+c" expression's value category is PRValue expression
  }
*/

/*
  // <-------------------- REMINDER --------------------> 

  int main(){
    int&& r = 10;
    // "int&& r = 10" is a declaration statement
    // r's declaration type is int&&

    r;
    // "r" is an expression
    // "r" expression's data type is int (not int&&)
    // "r" expression's value category is LValue expression

    // RULE : expression's data type is a non-reference type!!!

    int* p{};
    // "int* p{}" is a declaration statement
    // p's declaration type is int*
    // we can not talk about value category of a statement

    p;
    // "p" is an expression
    // "p" expression's data type is int*
    // "p" expression's value category is LValue expression
  }
*/