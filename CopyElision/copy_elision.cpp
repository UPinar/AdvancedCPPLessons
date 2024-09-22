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
  #define SCENARIO_1
  #include "myclass.hpp"

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
  #define SCENARIO_1
  #include "myclass.hpp"

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
  #define SCENARIO_1
  #include "myclass.hpp"

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
  #define SCENARIO_1
  #include "myclass.hpp"

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
  #define SCENARIO_2
  #include "myclass.hpp" 


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
  #define SCENARIO_2
  #include "myclass.hpp"

  void foo(Myclass){}

  int main(){
    foo(Myclass{});
    // output ->
    //  Myclass() - default ctor
    //  ~Myclass() - destructor

    // "Myclass{}" 
    // the expression that is argument of foo function
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
  #define SCENARIO_1
  #include "myclass.hpp"

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
  #define SCENARIO_1
  #include "myclass.hpp"

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
  #define SCENARIO_2
  #include "myclass.hpp"

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
                -----------------------------
                | Materialization Scenarios |
                -----------------------------
*/

/*
  #define SCENARIO_2
  #include "myclass.hpp"

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
  // Lesson 3 : 01:01:52
*/



