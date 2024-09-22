#include <iostream>

/*
                          ================
                          | Copy Elision |
                          ================
*/

/*
  PRValue expression is not accepted as an object.
  PRValue expression is a blueprint for initializing an object.

  before C++17 PRValue expressions accepted as an object.  
  compilers were not creating the object itself when they saw
  PRValue expression, but class's copy or move ctor must be exists.

  - C++17 öncesinde PRValue expressionlar bir nesne statüsünde idi.
  Derleyiciler aşağıdaki gibi bir PRValue expression içeren seneryolarda
  copy veya move constructor'a çağrı yapmıyor olsalar da, sınıfın
  çağrılabilir bir copy veya move constructor'a sahip olması gerekiyordu.
*/

/*
  // compiled C++14 standard

  class Myclass {
  public:
    Myclass(const Myclass&) = delete;
  };

  Myclass foo(){
    return Myclass{};
    // error: use of deleted function 'Myclass::Myclass(const Myclass&)'
  }

  void bar(Myclass);

  int main(){
    Myclass m = Myclass{};
    // error: use of deleted function 'Myclass::Myclass(const Myclass&)'

    bar(Myclass{});
    // error: use of deleted function 'Myclass::Myclass(const Myclass&)'
  }
*/

/*
  // compiled C++17 standard
  // PRValue expression will materialize to XValue expression

  class Myclass {
  public:
    Myclass() = default;
    Myclass(const Myclass&) = delete;
  };

  Myclass foo(){
    return Myclass{};
  }

  void bar(Myclass){}

  int main(){
    Myclass m = Myclass{};

    bar(Myclass{});

    Myclass{};  // discarding also materialize

    const Myclass& r = Myclass{}; 
    // temporary materialization
    // reference needs to bind to an object

    Myclass&& r2 = Myclass{};
    // temporary materialization
    // reference needs to bind to an object
  }
*/

/*
  // MANDATORY COPY ELISION - SENERYO 1:
  // ------------------------------------------------
  // bir fonksiyonun parametresi bir sınıf türünden ise
  // o fonksiyonu bir temporary object ile çağırmak.
  // Unmaterialize object passing

  class Myclass{
  public:
    Myclass(){
      std::cout << "Myclass()\n";
    }
    Myclass(int){
      std::cout << "Myclass(int)\n";
    }
    Myclass(const Myclass&) = delete;
  };

  void func(Myclass) {}  // functions parameter is a class type

  int main(){
    func(Myclass{12});    // output -> Myclass(int)
    // Myclass{12} is a PRValue expression
    // when the function is called, 
    // PRValue expression will be materialized 
    // as function's parameter variable
    // only Myclass(int) ctor will be called [1 time]

    func(Myclass{});      // output -> Myclass()
    // only default ctor will be called [1 time]
  }
*/

/*
  // MANDATORY COPY ELISION - SENERYO 2:
  // ------------------------------------------------
  // fonksiyonun return ifadesinin(expression)
  // bir temporary object olması.

  class Myclass{
  public:
    Myclass(){
      std::cout << "Myclass()\n";
    }
    Myclass(int){
      std::cout << "Myclass(int)\n";
    }
    Myclass(const Myclass&) = delete;
  };

  Myclass func(int x){
    return Myclass{ x };
  } // return expression is a temporary object
  

  int main(){
    auto m = func(12);  
    // output -> Myclass(int)

    auto m2 = Myclass{ Myclass{ func(12) } }; 
    // output -> Myclass(int)
  }
*/

/*
  - copy elision, bir nesne oluşturma(hayata getirme) ile ilgilidir.
  - eğer bir nesneyi hayata başlatmıyorsak, kopyalama semantiği yerine
    move semantiği kullanılabilir, fakat bu durumda copy elision
    kullanılamaz.

    x = y;  // x and y are both class objects
    // we CAN NOT USE COPY ELISION here !
    // because we are not creating an object here

    x = std::move(y);  
    // if x and y are both std::vector<std::string> objects
    // better using move semantics instead of copy semantics
*/

/*
  Bir nesneyi 
    1. copy constructor 
    2. move constructor
    3. copy elision 
  ile hayata getirebiliriz.

*/

/*
  <--- check where_copy_elision_occur.png --->

  MyException ex;
  throw ex;   
  // we are initializing compiler generated exception object 
  // with "ex" expression, so copy elision can be used here

  void func(Myclass);
  // when we call func function 
  // function's parameter variable will be initialized with
  // copy constructor or move constructor or 
  // copy elision can be used here.

  catch(MyException x)
  // compiler generated exception object will initialize
  // catch block's parameter variable ("x")
  // so copy elision can be used here
*/

/*
                    --------------------------
                    | Mandatory Copy Elision |
                    --------------------------
*/

/*
  URVO(RVO) (unnamed return value optimization) [Mandatory Copy Elision]
  NRVO      (named return value optimization)    
*/

/*
  RVO (Mandatory Copy Elision - guaranteed by the standard)
  ---------------------------------------------------------
  - If a class object's copy or move constructor is deleted,
    or they are inaccessible, mandatory copy elision can be used.
  - before C++17, it was a syntax error and 
    this was a compiler optimization)
  - after C++17, it is not an optimization, it is 
    guaranteed by the standard. 

  
  NRVO (Compiler Optimization - not guaranteed by the standard)
  ------------------------------------------------------------
  - Class object's copy or move constructor should be accessible
    and not deleted for this optimization to be done. 
*/

/*
  Mandatory Copy Elision
    - Unmaterialize object passing
    - Unnamed return value optimization
*/

/*
  #define MYCLASS_1
  #include "../myclass.hpp"

  int main(){
    std::cout << "main started\n";
    {
      Myclass m;
    }
    std::cout << "main continues\n";

    // output ->
    //  main started
    //  Myclass() - default ctor
    //  ~Myclass() - destructor
    //  main continues
  }
*/

/*
  #define MYCLASS_1
  #include "../myclass.hpp"

  int main(){
    std::cout << "main started\n";
    {
      Myclass m1(34);
      Myclass m2(m1);
    }
    std::cout << "main continues\n";

    // output ->
    //  main started
    //  Myclass(int)
    //  Myclass(const Myclass&) - copy ctor
    //  ~Myclass() - destructor
    //  ~Myclass() - destructor
    //  main continues
  }
*/

/*
  #define MYCLASS_1
  #include "../myclass.hpp"

  int main(){
    std::cout << "main started\n";
    {
      Myclass m1(34);
      Myclass m2(std::move(m1));
    }
    std::cout << "main continues\n";

    // output ->
    //  main started
    //  Myclass(int)
    //  Myclass(Myclass&&) - move ctor
    //  ~Myclass() - destructor
    //  ~Myclass() - destructor
    //  main continues
  }
*/

/*
      ---------------------------------------------------------
      | Unmaterialize object passing (Mandatory Copy Elision) |
      ---------------------------------------------------------
*/

/*
  #define MYCLASS_1
  #include "../myclass.hpp"

  int main(){
    std::cout << "main started\n";
    {
      Myclass m = Myclass{ Myclass{ Myclass{} } };
      // mandatory copy elision
      // until it finds the result object, it will not create any object
    }
    std::cout << "main continues\n";

    // output ->
    //  main started
    //  Myclass() - default ctor
    //  ~Myclass() - destructor
    //  main continues

    // before C++17, if compiler did not optimize this code
    // Myclass{}                                    -> default ctor
    // Myclass{ Myclass{} }                         -> copy ctor 
    // Myclass{ Myclass{ Myclass{} } }              -> copy ctor
    // Myclass m = Myclass{ Myclass{ Myclass{} } }  -> copy ctor
  }
*/

/*
  #define MYCLASS_2
  #include "../myclass.hpp" 

  int main(){
    std::cout << "main started\n";
    {
      Myclass m = Myclass{ Myclass{ Myclass{} } };
    }
    std::cout << "main continues\n";
    
    // output ->
    //  main started
    //  Myclass() - default ctor
    //  ~Myclass() - destructor
    //  main continues

    // before C++17 it will be syntax error, 
    // even if compiler optimized this code it still needs 
    // copy constructor to be accessible
  }
*/

/*
  #define MYCLASS_2
  #include "../myclass.hpp"

  void foo(Myclass){}

  int main(){
    foo(Myclass{});
    // output ->
    //  Myclass() - default ctor
    //  ~Myclass() - destructor

    // "Myclass{}" 
    // is an expression that is the argument of foo function
    // is a PRValue expression
    // it will initialize the foo() function's parameter variable
    // Mandatory Copy Elision will occur

    foo(Myclass{12});
    // output ->
    //  Myclass(int)
    //  ~Myclass() - destructor
    
    foo(Myclass{ Myclass{ 35 } });
    // output ->
    //  Myclass(int)
    //  ~Myclass() - destructor

    // function's parameter variable will be the result object
    // unmateralize object passing
  }
*/

/*
  #define MYCLASS_1
  #include "../myclass.hpp"

  void foo(Myclass){}

  int main(){
    Myclass mx;

    foo(mx);  
    // we can not talk about copy elision here

    // output ->
    //  Myclass() - default ctor
    //  Myclass(const Myclass&) - copy ctor
    //  ~Myclass() - destructor
    //  ~Myclass() - destructor

    foo(std::move(mx));
    // we can not talk about copy elision here

    // output -> 
    //  Myclass(Myclass&&) - move ctor
    //  ~Myclass() - destructor
    //  ~Myclass() - destructor

    // if our purpose is only passing the object to the function
    // better creating temporary object
  }
*/

/*
  ---------------------------------------------------------------------
  | (Unnamed) Return Value Optimization (RVO) [Mandatory Copy Elision]|
  ---------------------------------------------------------------------
*/

/*
  #define MYCLASS_1
  #include "../myclass.hpp"

  Myclass foo(int x)
  {
    return Myclass{ x };
  }
  // "Myclass{ x }" expression is a temporary object
  // it is a PRValue expression

  int main(){
    auto mx = foo(123);
    // output ->
    //  Myclass(int)
    //  ~Myclass() - destructor
  }
*/

/*
  #define MYCLASS_2
  #include "../myclass.hpp"

  Myclass foo(int x)
  {
    return Myclass{ x };
  }

  int main(){
    auto mx = foo(123);
    // output ->
    //  Myclass(int)
    //  ~Myclass() - destructor

    // no syntax error when copy and move constructors are deleted
  }
*/

/*
  // even if non copyable and non movable class objects(i.e atomic types)
  // we can still write a factory functions

  template <typename T, typename ...Args>
  auto create_object(Args&& ...args)
  {
    return T(std::forward<Args>(args)...);
    // "T(std::forward<Args>(args)...)" expression is a temporary object
    // it is a PRValue expression
    // non copyable and non movable class objects can be created
  }
*/

/*
  // ------------------ Materialization Scenarios ------------------

  #define MYCLASS_2
  #include "../myclass.hpp"

  int main()
  {
    // ------------------------------------------------

    Myclass{};
    // "Myclass{}" expression is a temporary object
    // it is a PRValue expression

    // output ->
    //  Myclass() - default ctor
    //  ~Myclass() - destructor

    // even if expression is discarded
    // temporary materialization will be applied.

    // ------------------------------------------------

    Myclass& r1 = Myclass{};        // syntax error

    // ------------------------------------------------

    const Myclass& r2 = Myclass{};  // VALID
    // "Myclass{}" expression is a temporary object
    // it is a PRValue expression

    // because of reference will bind to an object
    // temporary materialization will be applied.
    // PRValue expression will be materialized to XValue expression

    // output ->
    //  Myclass() - default ctor
    //  ~Myclass() - destructor

    // ------------------------------------------------

    Myclass&& r3 = Myclass{};       // VALID
    // "Myclass{}" expression is a temporary object
    // it is a PRValue expression

    // because of reference will bind to an object
    // temporary materialization will be applied.
    // PRValue expression will be materialized to XValue expression

    // output ->
    //  Myclass() - default ctor
    //  ~Myclass() - destructor

    // ------------------------------------------------

    auto val = Myclass{}.foo();
    // if there is not an object, we can not call its member function
    // temporary materialization will be applied.

    std::cout << "val = " << val << '\n';

    // output ->
    //  Myclass() - default ctor
    //  Myclass::foo()
    //  val = 5
    //  ~Myclass() - destructor

    // ------------------------------------------------
  }
*/

/* 
        ---------------------------------------------------------
        | Named Return Value Optimization (NRVO) [Optimization] |
        ---------------------------------------------------------
*/

/*
  #define MYCLASS_2
  #include "../myclass.hpp"

  Myclass foo(){
    Myclass m;
    return m;
    // error: use of deleted function 'Myclass::Myclass(Myclass&&)'
  }
*/

/*
  #define MYCLASS_1
  #include "../myclass.hpp"

  Myclass foo(){
    Myclass m;
    return m;
  }

  Myclass foo2(int val){
    Myclass m(val);
    return m;
  }

  int main(){
    Myclass mx = foo();
    // Copy Elision is applied here

    // output ->
    //  Myclass() - default ctor
    //  ~Myclass() - destructor

    Myclass mx2 = foo2(12);
    // Copy Elision is applied here

    // output ->
    //  Myclass(int)
    //  ~Myclass() - destructor
  }
*/

/*
  // disabling NRVO in gcc with -fno-elide-constructors flag
  // g++ -fno-elide-constructors -o prog copy_elision.cpp -std=c++20

  #define MYCLASS_1
  #include "../myclass.hpp"

  Myclass foo(int val){
    Myclass m(val); 
    return m;
    // automatic storage duration objects are move eligible 
    // L value to R value conversion
  }

  int main(){
    Myclass mx = foo(12);
    // COPY ELISION is not applied here(move semantics is applied)
    // output ->
    // Myclass(int)
    // Myclass(Myclass&&) - move ctor
    // ~Myclass() - destructor
    // ~Myclass() - destructor
  }
*/

/*
  #define MYCLASS_1
  #include "../myclass.hpp"

  Myclass foo(int val){
    std::cout << "foo() has been called\n";

    Myclass m(val); 
    std::cout << "&m = " << &m << '\n';

    return m;
  }

  int main(){
    Myclass mx = foo(12);
    std::cout << "&mx = " << &mx << '\n';

    // output ->
    //  foo() has been called
    //  Myclass(int)
    //  &m = 0x8c437ff6df
    //  &mx = 0x8c437ff6df
    //  ~Myclass() - destructor

    // NRVO, Copy Elision is applied here
    // basically, we are sending the address of the object 
    // from main function to foo function with an int parameter
  }

  //  foo(int):
  //    push rbp
  //    mov rbp, rsp
  //    sub rsp, 16
  //    mov QWORD PTR [rbp-8], rdi    -----------> ptr = &mx
  //    mov DWORD PTR [rbp-12], esi
  //    mov edx, DWORD PTR [rbp-12]
  //    mov rax, QWORD PTR [rbp-8]    -----------> rax = ptr = &mx
  //    mov esi, edx
  //    mov rdi, rax          -----------> (3)
  //    call Myclass::Myclass(int) [complete object constructor]
  //    mov rax, QWORD PTR [rbp-8]
  //    leave
  //    ret
  //  main:
  //    push rbp
  //    mov rbp, rsp
  //    sub rsp, 16
  //    lea rax, [rbp-1]      -----------> (1)
  //    mov esi, 12           
  //    mov rdi, rax          -----------> (2)
  //    call foo(int)
  //    lea rax, [rbp-1]
  //    mov rdi, rax
  //    call Myclass::~Myclass() [complete object destructor]
  //    mov eax, 0
  //    leave
  //    ret

  //  (1) lea rax, [rbp-1]  
  //  -> address of mx object stored in rax

  //  (2) mov rdi, rax      
  //  -> address of mx object passed to foo function inside 
  //     rdi(first caller saved register) then 12 is passed with 
  //     esi(second caller saved register)

  //  (3) mov rdi, rax
  //  -> inside foo function address of mx object is sent to constructor
*/

/*
  #define MYCLASS_3
  #include "../myclass.hpp"

  #include <memory>
  #include <string>

  // std::unique_ptr is move only type
  std::unique_ptr<std::string> foo()
  {
    auto up = std::make_unique<std::string>("hello world");
    return up;
    // L value to R value conversion (move eligible)
  }

  // Myclass in this example is move only type
  Myclass foo2(int val){
    Myclass m(val);
    return m;
    // L value to R value conversion (move eligible)
  }

  int main(){
    Myclass mx = foo2(12);  
    // copy members are deleted -> no syntax error 
  }
*/

/*
  ----------------------------------------------------------------------
  side effect(yan etki) : situations that change the state of the program
    - sending data to an output device
    - changing global variable's value
    - changing static local variable's value
    - data transfer to a file
    - calling exit() function and terminate the program
  ----------------------------------------------------------------------
  if classes copy constructor or move constructor has a side effect 
  and programs logic depends on this side effect
  NRVO can still be applied, and that side effect will not be executed
  better not writing side effects in copy or move constructors !!  
  ----------------------------------------------------------------------
*/

/*
  void foo(Myclass&);   // old codes (out parameter)
  Myclass foo();        // new codes (value return)

  -> move semantics or copy elision can be used in new codes

  // --------------------------------------------------------------
  std::vector<std::string> foo1()
  {
    return PRValue (temporary object)
  }
  // RVO (Mandatory Copy Elision) will be applied here

  std::vector<std::string> foo2()
  {
    std::vector<std::string> vec; (automatic storage duration object)
    return vec;
  }
  // NRVO (Compiler Optimization) or move semantics will be applied here
  // Copy Elision or move semantics.

  std::vector<std::string> vec;
  vec = foo1();  
  // move semantics, not copy elision 
  // (new object is NOT created, existing object used)

  std::vector<std::string> vec2 = foo1();   // RVO
  std::vector<std::string> vec2 = foo2();   // NRVO
  // copy elision (new object is created)
  // --------------------------------------------------------------
*/

/*
                    ---------------------
                    | No NRVO Scenarios |
                    ---------------------
*/

/*
  // SCENARIO 1: returning a parameter variable of a function

  class Myclass{
  public:
    Myclass(){
      std::cout << "Myclass()\n";
    }
    Myclass(const Myclass&){
      std::cout << "Myclass(const Myclass&)\n";
    }
    Myclass(Myclass&&){
      std::cout << "Myclass(Myclass&&)\n";
    }
  };

  Myclass func(Myclass x){
    return x; 
    // returning parameter variable No copy elision
    // NO NRVO

    // because of parameter variable 
    // is an automatic storage duration object
    // L value to R value conversion (move eligible) is applied
    // when returning the parameter variable
  }

  int main(){
    Myclass mx;   
    // output -> Myclass()

    Myclass my{ func(mx) };
    // output ->
    //  Myclass(const Myclass&)
    //  Myclass(Myclass&&)  ----> L value to R value conversion
  }
*/

/*
  // SCENARIO 2:  depends on the situation function's 
  //              return value types are different objects

  #define MYCLASS_1
  #include "../myclass.hpp"

  Myclass f1(int x){
    Myclass m{ x };  
    return x > 10 ? m : m; 
  }
  // returning the same object but ternary operator is used
  // compiler can not apply NRVO here 
  // move semantics can not be used also

  Myclass f1(int x)
  {
    return x > 10 ? Myclass{ 5 } : Myclass{ 10 }; 
  }

  Myclass f2(int x){
    auto m = Myclass(x);  // Mandatory Copy Elision

    if (x > 10)
      return m;   // move eligible (L to R value conversion)

    return Myclass{ x + 5 };
  } 

  Myclass f3(int x)
  {
    if (x > 10){
      Myclass mx(x);
      return mx;
    }
    else{
      return Myclass(x + 3);
    }
  }

  Myclass f4(int x){
    if (x > 10){
      Myclass mx(x);
      return mx;
    }
    else{
      Myclass my(x + 3);
      return my;
    }
  }

  Myclass f5_a(int x){
    Myclass mx(x);
    return mx;
  }

  Myclass f5_b(int x){
    Myclass mx(x + 5);
    return mx;
  }

  Myclass f5(int x){
    if (x > 10)
      return f5_a(x);
    else 
      return f5_b(x);
  }

  // compiled with gcc version 14.1.0
  int main()
  {
    // -----------------------------------------------------------------
    auto mx = f1(20);
    // output ->
    //  Myclass(int)
    //  Myclass(const Myclass&) - copy ctor
    //  ~Myclass() - destructor
    //  ~Myclass() - destructor
    // NRVO CAN NOT be applied here
    // Also L value to R value conversion is not applied here

    // -----------------------------------------------------------------
    auto mx1 = f1_2(20);
    // output ->
    //  Myclass(int)
    //  ~Myclass() - destructor
    // NRVO (Copy Ellision) can be applied here

    auto mx1_2 = f1_2(5);
    // output ->
    //  Myclass(int)
    //  ~Myclass() - destructor
    // NRVO (Copy Ellision) can be applied here

    // -----------------------------------------------------------------
    auto mx2 = f2(20);
    // output ->
    //  Myclass(int)
    //  Myclass(Myclass&&) - move ctor   --> NRVO can not be applied
    //  ~Myclass() - destructor
    //  ~Myclass() - destructor

    auto mx2_2 = f2(5);
    // output ->
    //  Myclass(int)
    //  Myclass(int)    ----> RVO (Mandatory Copy Elision) applied
    //  ~Myclass() - destructor
    //  ~Myclass() - destructor

    // -----------------------------------------------------------------
    auto mx3 = f3(20);
    // output ->
    // Myclass(int)
    // ~Myclass() - destructor
    // NRVO (Copy Ellision) can be applied here 

    auto mx3_2 = f3(5);
    // output ->
    // Myclass(int)
    // ~Myclass() - destructor
    // RVO (Mandatory Copy Elision) applied here

    // -----------------------------------------------------------------
    auto mx4 = f4(20);
    // output ->
    // Myclass(int)
    // ~Myclass() - destructor
    // NRVO (Copy Ellision) can be applied here

    auto mx4_2 = f4(5);
    // output ->
    //  Myclass(int)
    //  Myclass(Myclass&&) - move ctor
    //  ~Myclass() - destructor
    //  ~Myclass() - destructor
    // NRVO CAN NOT be applied here.

    // -----------------------------------------------------------------
    auto mx5 = f5(5);
    // output ->
    //  Myclass(int)
    //  ~Myclass() - destructor
    // NRVO (Copy Ellision) can be applied here

    auto mx5_2 = f5(20);
    // output ->
    //  Myclass(int)
    //  ~Myclass() - destructor
    // NRVO (Copy Ellision) can be applied here

    // -----------------------------------------------------------------
  }
*/

/*
  struct force_nrvo{
  public:
    force_nrvo(int){
      std::cout << "force_nrvo(int)\n";
    }

    force_nrvo(const force_nrvo&);    // declared but not defined
    force_nrvo(force_nrvo&&);         // declared but not defined
  };

  // when copy ellision (NRVO) is applied,
  // copy or move constructor will not be called
  // LINK Time error will happen if NRVO is not applied
*/

/*
  #define MYCLASS_1
  #include "../myclass.hpp"

  Myclass foo(int x)
  {
    Myclass m1;
    Myclass m2;

    if (x > 10)
      return m1;
    else
      return m2;
  }
  // if there are 2 visible objects and
  // depends on the condition, 1 of them will be returned
  // NRVO can not be applied here

  int main(){
    auto mx1 = foo(20);
    // output ->
    //  Myclass() - default ctor
    //  Myclass() - default ctor
    //  Myclass(Myclass&&) - move ctor
    //  ~Myclass() - destructor
    //  ~Myclass() - destructor
    //  ~Myclass() - destructor

    // (L value to R value conversion) move semantics is applied
    // NRVO CAN NOT be applied here

    auto mx2 = foo(5);
    // output ->
    //  Myclass() - default ctor
    //  Myclass() - default ctor
    //  Myclass(Myclass&&) - move ctor
    //  ~Myclass() - destructor
    //  ~Myclass() - destructor
    //  ~Myclass() - destructor

    // (L value to R value conversion) move semantics is applied
    // NRVO CAN NOT be applied here
  }
*/