#include <iostream>

/*
                      ==================
                      | Move Semantics |
                      ==================
*/

/*
  - if an object will no longer be used, 
    rather than copying it(deep copy) 
    it is better to steal its resources.
*/

/*
  - Move constructor and move assignment operator 
    will be generated(implicitly declared defaulted) by the compiler 
    if they were running as expected, 
    do not need to be defined explicitly.
    1. if classes data member is not a handle 
      - pointer(s) to resource
      - reference(s) to resource
    no need to explicitly define move members.
  
    2. if compiler generated move members are breaking
      moved-from state objects invariants.
    we need to explicitly define move members.
*/

/*
  #include <vector>
  #include <string>

  class Myclass{
  public:

    // ---- compiler generated(defaulted) copy constructor ----
    Myclass(const Myclass& other) : 
      m_vec(other.m_vec), m_str(other.m_str) {}

    // in compiler generated copy constructor,
    // if data members are class types, their copy constructors 
    // will be called. 

    // ---- compiler generated(defaulted) move constructor ----
    Myclass(Myclass&& other) : 
      m_vec(std::move(other.m_vec)), m_str(std::move(other.m_str)) {}

    // in compiler generated move constructor,
    // if data members are class types, their move constructors
    // will be called.

    // ---- compiler generated(defaulted) copy assignment operator ----
    Myclass& operator=(const Myclass& other){
      m_vec = other.m_vec;
      m_str = other.m_str;
      return *this;
    }

    // ---- compiler generated(defaulted) move assignment operator ----
    Myclass& operator=(Myclass&& other){
      m_vec = std::move(other.m_vec);
      m_str = std::move(other.m_str);
      return *this;
    }

  private:
    std::vector<int>  m_vec;
    std::string       m_str;
  };

  // for Myclass it is better applying (Rule of 0)
  // not defining any of the special member functions

  class BetterMyclass{
  private:
    std::vector<int>  m_vec;
    std::string       m_str;
  };
*/

/*
  Myclass x;
  x = y; 
  // "y" expression is LValue expression, its type is Myclass
  // copy assignment operator will be called.

  x = foo();  
  // "foo()" expression is RValue expression, its type is Myclass
  // move assignment operator will be called.

  x = Myclass{};
  // Myclass{} is a temporary object PRValue expression
  // move assignment operator will be called.
*/


/*
                        -----------------
                        | Bad Scenarios |
                        -----------------
        1. do not use const in functions return type
        2. do not use std::move in functions return expression
*/

/*
  -----------------------------------------------------------------------
  |               |   T&     |   const T&     |   T&&     |  const T&&  |
  -----------------------------------------------------------------------
  | LValue        |    1     |       2        |    X      |      X      |
  -----------------------------------------------------------------------
  | const LValue  |    X     |       1        |    X      |      X      |
  -----------------------------------------------------------------------
  | RValue        |    X     |       3        |    1      |      2      |
  -----------------------------------------------------------------------
  | const RValue  |    X     |       2        |    X      |      1      |
  -----------------------------------------------------------------------
*/

/*
  #define MYCLASS_1
  #include "../myclass.hpp"

  // using `const` in a functions return type
  // prevents the compiler from move semantics
  const Myclass foo(int x)
  {
    Myclass m(x);
    return m;
    // L value to R value conversion (move eligible)
  }

  Myclass bar(int x){
    Myclass m(x);
    return m;
  }

  int main(){
    // because of we are not creating a new object NO NRVO

    Myclass mx1;
    mx1 = foo(23);
    // "foo(23)" is const RValue expression
    // - const RValue can not bind to Myclass&& inside move assignment
    // - const RValue expression can bind to const Myclass& 
    //   inside copy assignment

    // output ->
    //  Myclass() - default ctor
    //  Myclass(int)
    //  copy assignment     --------------->
    //  ~Myclass() - destructor
    //  ~Myclass() - destructor

    Myclass mx2;
    mx2 = bar(23);
    // "bar(23)" is RValue expression
    // - RValue expression can bind to Myclass&& inside move assignment

    // output ->
    //  Myclass() - default ctor
    //  Myclass(int)
    //  move assignment    --------------->
    //  ~Myclass() - destructor
    //  ~Myclass() - destructor
  }
*/

/*
  #define MYCLASS_1
  #include "../myclass.hpp"

  // using std::move in functions return expression
  // pessimistic move
  Myclass foo()
  {
    Myclass x;
    // code
    return std::move(x);
  }

  // if NRVO(Copy Elision) is not applied 
  // because of L value to R value conversion(move eligible) will be done
  // move semantics will already be applied
  // no need to use std::move in return expression

  // if NRVO(Copy Elision) wants to be applied 
  // because of the return expression's type is Myclass&& 
  // NRVO could not be applied


  Myclass bar(){
    Myclass x;
    return x;
  }

  int main(){
    // creating a new object and want NRVO to be applied
    Myclass m1 = foo();
    // output ->
    //  Myclass() - default ctor
    //  Myclass(Myclass&&) - move ctor
    //  ~Myclass() - destructor
    //  ~Myclass() - destructor

    // creating a new object and want NRVO to be applied
    Myclass m2 = bar();
    // output ->
    //  Myclass() - default ctor
    //  ~Myclass() - destructor
    // NRVO(Copy Elision) is applied
  }
*/

/*
  // normally copy assignment or copy constructor will be called
  // but we want specifically move members to be called

  #define MYCLASS_1
  #include "../myclass.hpp"

  int main(){
    Myclass m1{ 34 };
    Myclass m2{ 35 };

    Myclass x = m1;                           // copy ctor
    m1 = m2;                                  // copy assignment

    Myclass y = std::move(m1);                // move ctor
    Myclass z = static_cast<Myclass&&>(m1);   // move ctor
    m1 = std::move(m2);                       // move assignment
    m1 = static_cast<Myclass&&>(m2);          // move assignment
  }
*/

/*
  #define MYCLASS_1
  #include "../myclass.hpp"
  #include <utility>  // std::move

  class AClass{
  public:
    AClass(const Myclass& m) : mx{m} {}
  private:
    Myclass mx;
  };

  class SClass{
  public:
    SClass(Myclass m) : mx{std::move(m)} {}
  private:
    Myclass mx;
  };

  int main(){
    // -----------------------------------------------------
    AClass ac{ Myclass{ 23 } };
    // output ->
    //  Myclass(int)
    //  Myclass(const Myclass&) - copy ctor
    //  ~Myclass() - destructor
    //  ~Myclass() - destructor

    // -----------------------------------------------------
    SClass sc{ Myclass{ 24 } };
    // output ->
    //  Myclass(int)
    //  Myclass(Myclass&&) - move ctor
    //  ~Myclass() - destructor
    //  ~Myclass() - destructor

    // -----------------------------------------------------
    Myclass m;
    SClass sc2{ m };
    // output ->
    //  Myclass() - default ctor
    //  Myclass(const Myclass&) - copy ctor
    //  Myclass(Myclass&&) - move ctor
    //  ~Myclass() - destructor
    //  ~Myclass() - destructor
    //  ~Myclass() - destructor

    // -----------------------------------------------------
    Myclass m2;
    SClass sc3{ std::move(m2) };
    // output ->
    //  Myclass() - default ctor
    //  Myclass(Myclass&&) - move ctor
    //  Myclass(Myclass&&) - move ctor
    //  ~Myclass() - destructor
    //  ~Myclass() - destructor
    //  ~Myclass() - destructor
  }
*/

/*
  #define MYCLASS_1
  #include "../myclass.hpp"
  #include <utility>  // std::move

  // -------------------- SCENARIO_1 --------------------
  class AClass{
  public:
    AClass(const Myclass& m) : mx{m} {}
  private:
    Myclass mx;
  };

  // -------------------- SCENARIO_2 --------------------
  class BClass{
  public:
    BClass(Myclass m) : mx{std::move(m)} {}
  private:
    Myclass mx;
  };

  // -------------------- SCENARIO_3 --------------------
  class CClass{
  public:
    CClass(const Myclass& m) : mx{m} {}
    CClass(Myclass&& m) : mx{std::move(m)} {}
  private:
    Myclass mx;
  };
*/

/*
                    -------------------------
                    | `noexcept` (REMINDER) |
                    -------------------------
*/

/*
  void func();            
  // func function can throw an exception

  void foo() noexcept;    
  // foo function is giving no-throw guarantee

  void bar() noexcept(true);  
  // bar function is giving no-throw guarantee

  void baz() noexcept(false); 
  // baz function can throw an exception
*/

/*
  void foo() noexcept(sizeof(int) == 4); 
  // foo function is giving no-throw guarantee if sizeof(int) == 4
*/

/*
  template <typename T>
  void foo(T) noexcept(std::is_nothrow_copy_constructible_v<T>);

  // if T type's copy constructor is giving no-throw guarantee
  // foo function also is giving no-throw guarantee
*/

/*
  - noexcept is a part of function signature 
  - but we can not use it in function overload

  void foo(int);
  void foo(int) noexcept;
  // error: declaration of 'void foo(int) noexcept' 
  // has a different exception specifier
*/

/*
  void foo(int) noexcept; 
  // foo function is giving no-throw guarantee
  void bar(int);
  // bar function can throw an exception

  int main(){
    //--------------------------------------------------------
    void (*fptr)(int) = foo;
    constexpr auto b1 = noexcept(fptr(23));   // b1 is false

    void (*fptr2)(int) noexcept = foo;
    constexpr auto b2 = noexcept(fptr2(23));  // b2 is true
    // fptr2 function pointer is giving no-throw guarantee

    //--------------------------------------------------------
    void (*fptr3)(int) noexcept = bar;  // syntax error
    // error: invalid conversion from 'void (*)(int)' to 
    // 'void (*)(int) noexcept' 

    // fptr3 function pointer is giving no-throw guarantee
    // but bar function is not giving no-throw guarantee
    // it will cause syntax error

    //--------------------------------------------------------
    auto fptr4 = foo;
    constexpr auto b3 = noexcept(fptr4(23));  // b3 is true
    // fptr4 function pointer's type is `void (*)(int) noexcept`

    //--------------------------------------------------------
  }
*/

/*
  // overriding virtual functions in inheritance
  //---------------------------------------------------------
  class Base{
  public:
    virtual void func(int);
    // func function can throw an exception
  };

  class Der : public Base{
  public:
    void func(int) noexcept override; // no syntax error
  };

  //---------------------------------------------------------
  class Base2{
  public:
    virtual void foo(int) noexcept;
    // foo function is giving no-throw guarantee
  };

  class Der2 : public Base2{
  public:
    void foo(int) override; // syntax error
  };
  // error: looser exception specification on overriding 
  // virtual function 'virtual void Der2::foo(int)'

  // User will trust the Base classes no-throw guarantee
  // but if a Derived class comes, it is saying that 'I can throw'
  // this will violate Base classes no-throw guarantee

  //---------------------------------------------------------
*/

/*
  // overriding virtual functions 
  class Base{
  public:
    virtual void foo() noexcept (sizeof(int) < 8);
  };

  class Der : public Base{
  public:
    virtual void foo() noexcept (sizeof(int) < 4) override; 
    // syntax error

    // error: looser exception specification 
    // on overriding virtual function
    // 'virtual void Der::foo() noexcept (false)'

    // because of sizeof(int) = 4, derived class function is not giving
    // no-throw guarantee that base class function is giving
    // this will cause syntax error
  };
*/

/*
  class Base{
  public:
    void foo(int) noexcept;
  };

  class Der : public Base{
  public:
    void foo(int); // VALID
  };
  // Der class's foo function is not a virtual function 
  // that is overriding Base class's foo function
*/

/*
  classes 
    - move constructor
    - move assignment
    - swap functions 
    - memory deallocation functions
  should give no-throw guarantee

  class destructor are implicitly noexcept

  when compiler, generated special member functions 
  it will undertand if they are noexcept or not statically 
  (by looking at the code)
*/

/*
  #include <type_traits>

  class Myclass{};

  int main(){
    using namespace std;
    cout.setf(ios::boolalpha);

    cout  << "is default constructor noexcept ? " 
          << is_nothrow_default_constructible_v<Myclass> << '\n';
    cout  << "is destructor noexcept ? " 
          << is_nothrow_destructible_v<Myclass> << '\n';
    cout  << "is copy constructor noexcept ? " 
          << is_nothrow_copy_constructible_v<Myclass> << '\n';
    cout  << "is move constructor noexcept ? " 
          << is_nothrow_move_constructible_v<Myclass> << '\n';
    cout  << "is copy assignment noexcept ? " 
          << is_nothrow_copy_assignable_v<Myclass> << '\n';
    cout  << "is move assignment noexcept ? " 
          << is_nothrow_move_assignable_v<Myclass> << '\n';

    // output ->
    //  is default constructor noexcept ? true
    //  is destructor noexcept ? true
    //  is copy constructor noexcept ? true
    //  is move constructor noexcept ? true
    //  is copy assignment noexcept ? true
    //  is move assignment noexcept ? true
  }
*/

/*
  #include <type_traits>

  class Myclass{
    std::string mstr{};
  };

  int main(){
    using namespace std;
    cout.setf(ios::boolalpha);

    cout  << "is default constructor noexcept ? " 
          << is_nothrow_default_constructible_v<Myclass> << '\n';
    cout  << "is destructor noexcept ? " 
          << is_nothrow_destructible_v<Myclass> << '\n';
    cout  << "is copy constructor noexcept ? " 
          << is_nothrow_copy_constructible_v<Myclass> << '\n';
    cout  << "is move constructor noexcept ? " 
          << is_nothrow_move_constructible_v<Myclass> << '\n';
    cout  << "is copy assignment noexcept ? " 
          << is_nothrow_copy_assignable_v<Myclass> << '\n';
    cout  << "is move assignment noexcept ? " 
          << is_nothrow_move_assignable_v<Myclass> << '\n';

    // output ->
    //  is default constructor noexcept ? true
    //  is destructor noexcept ? true
    //  is copy constructor noexcept ? false
    //  is move constructor noexcept ? true
    //  is copy assignment noexcept ? false
    //  is move assignment noexcept ? true
  }

  // copy members are not giving no-throw guarantee
  // std::string class's(data member) 
  // copy members are not giving no-throw guarantee
  // so Myclass's copy members are not giving no-throw guarantee
*/

/*
  #include <type_traits>

  class AClass{};

  class Myclass{
    AClass m_ac;
  };

  int main(){
    using namespace std;
    cout.setf(ios::boolalpha);

    cout  << "is default constructor noexcept ? " 
          << is_nothrow_default_constructible_v<Myclass> << '\n';
    cout  << "is destructor noexcept ? " 
          << is_nothrow_destructible_v<Myclass> << '\n';
    cout  << "is copy constructor noexcept ? " 
          << is_nothrow_copy_constructible_v<Myclass> << '\n';
    cout  << "is move constructor noexcept ? " 
          << is_nothrow_move_constructible_v<Myclass> << '\n';
    cout  << "is copy assignment noexcept ? " 
          << is_nothrow_copy_assignable_v<Myclass> << '\n';
    cout  << "is move assignment noexcept ? " 
          << is_nothrow_move_assignable_v<Myclass> << '\n';

    // output ->
    //  is default constructor noexcept ? true
    //  is destructor noexcept ? true
    //  is copy constructor noexcept ? true
    //  is move constructor noexcept ? true
    //  is copy assignment noexcept ? true
    //  is move assignment noexcept ? true
  }

  // all special member functions are giving no-throw guarantee
  // becuase AClass's(data member) special member functions are also
  // giving no-throw guarantee
*/

/*
  #include <type_traits>

  class AClass{
  public:
    AClass();
  };

  class Myclass{
    AClass m_ac;
  };

  int main(){
    using namespace std;
    cout.setf(ios::boolalpha);

    cout  << "is default constructor noexcept ? " 
          << is_nothrow_default_constructible_v<Myclass> << '\n';
    cout  << "is destructor noexcept ? " 
          << is_nothrow_destructible_v<Myclass> << '\n';
    cout  << "is copy constructor noexcept ? " 
          << is_nothrow_copy_constructible_v<Myclass> << '\n';
    cout  << "is move constructor noexcept ? " 
          << is_nothrow_move_constructible_v<Myclass> << '\n';
    cout  << "is copy assignment noexcept ? " 
          << is_nothrow_copy_assignable_v<Myclass> << '\n';
    cout  << "is move assignment noexcept ? " 
          << is_nothrow_move_assignable_v<Myclass> << '\n';

    // output ->
    //  is default constructor noexcept ? false
    //  is destructor noexcept ? true
    //  is copy constructor noexcept ? true
    //  is move constructor noexcept ? true
    //  is copy assignment noexcept ? true
    //  is move assignment noexcept ? true
  }

  // only default constructor is not giving no-throw guarantee
  // because AClass's default constructor is user declared to be defined
  // and it is not giving no-throw guarantee
*/


/*
  // if we want to make sure compiler generated default constructor
  // to be noexcept, we can use static_assert declaration

  #include <type_traits>

  class AClass{
  public:
    AClass();
  };

  class Myclass{
    AClass m_ac;
  };

  static_assert(std::is_nothrow_default_constructible_v<Myclass>, 
                "Myclass default constructor should be noexcept");
  // output -> 
  // error: static assertion failed: 
  // Myclass default constructor should be noexcept
*/

/*
  #include <type_traits>

  class AClass{
  public:
    AClass();
  };

  class Myclass{
  public:
    Myclass() noexcept = default;
  private:
    AClass m_ac;
  };

  int main(){
    using namespace std;
    cout.setf(ios::boolalpha);

    cout  << "is default constructor noexcept ? " 
          << is_nothrow_default_constructible_v<Myclass> << '\n';
    cout  << "is destructor noexcept ? " 
          << is_nothrow_destructible_v<Myclass> << '\n';
    cout  << "is copy constructor noexcept ? " 
          << is_nothrow_copy_constructible_v<Myclass> << '\n';
    cout  << "is move constructor noexcept ? " 
          << is_nothrow_move_constructible_v<Myclass> << '\n';
    cout  << "is copy assignment noexcept ? " 
          << is_nothrow_copy_assignable_v<Myclass> << '\n';
    cout  << "is move assignment noexcept ? " 
          << is_nothrow_move_assignable_v<Myclass> << '\n';

    // output ->
    //  is default constructor noexcept ? true
    //  is destructor noexcept ? true
    //  is copy constructor noexcept ? true
    //  is move constructor noexcept ? true
    //  is copy assignment noexcept ? true
    //  is move assignment noexcept ? true
  }
*/

/*
  #include <type_traits>  // std::is_nothrow_destructible_v

  class Myclass{
  public:
    ~Myclass();   // user declared to be defined
  };

  int main()
  {
    std::cout << std::boolalpha;
    std::cout << std::is_nothrow_destructible_v<Myclass> << '\n';
    // output -> true

    // if we declare a destructor without noexcept specifier
    // it will be implicitly noexcept
  }
*/

/*
  #include <type_traits>  // std::is_nothrow_destructible_v

  class Myclass{
  public:
    ~Myclass() noexcept(false);   // C++20
  };

  int main()
  {
    std::cout << std::boolalpha;
    std::cout << std::is_nothrow_destructible_v<Myclass> << '\n';
    // output -> false
  }
*/

/*
  1. basic exception guarantee : 
    - if a function will throw an exception,
      object will not be in invalid state and should be usable.
    - will not leak resources.

  2. strong exception guarantee : (commit or rollback guarantee)
    - will give all basic exception guarantees
    - if an exception thrown, program state WILL NOT be changed.

    (ya işi yaparım ya da işi yapamazsam, 
      iş yapılmadan önceki program state'i korurum.)

    - copy and swap idiom is used to give strong exception guarantee.

  3. no-throw guarantee : (noexcept)
    - if this function or functions that it calls throws an exception,
    program will terminate(std::terminate will be called).
*/

/*
  #include <string>

  void foo()
  {
    auto p = new std::string{"hello world"};

    // code 1
    // code 2
    // code 3

    delete p;
  }

  // if code1, code2, code3 throws an exception
  // foo() function will leak resource
  // so this function is not giving basic exception guarantee
*/

/*
  --------------------------------------------------------------------
  | override contextual keyword(bağlamsal anahtar sözcük) [REMINDER] |
  --------------------------------------------------------------------
*/

/*
  int main()
  {
    int override = 10;    
    // because of override is a contextual keyword
    // it can be used as an identifier
  }
*/

/*
  class Base{
  public:
    virtual void foo(int);
    virtual void func(unsigned int);
    void bar(int);  // not a virtual function
  };

  class Der : public Base{
  public:
    void foo(int);
    void func(int);
    void bar(int);
  };

  // for a function to be override function
  // functions signature and return type should be same

  // Der::foo function is overriding Base::foo function
  // Der::func function is not overriding Base::func function(hiding)
  // Der::bar function is not overriding Base::bar function
*/

/*
  // when override contextual keyword is used

  class Base{
  public:
    virtual void func(unsigned int);
    void bar(int);
  };

  class Der : public Base{
  public:
    void func(int) override; // syntax error
    // Base::foo function's signature is not same

    void bar(int) override; // syntax error
    // Base::bar function is not a virtual function
  };
*/

/*
        -----------------------------------------------------
        | move sematics and noexcept specifier relationship |
        -----------------------------------------------------
*/

/*
  - compiler will optimize better when it knows that 
    a function will not throw an exception.(no-throw guarantee)
*/

/*
  #include <vector>
  #include <string>

  class Student{
  public:
    Student(const char* name) : name_{ name } {}

    Student(const Student& other) : name_{ other.name_ } 
    {
      std::cout << "COPY CTOR " << name_ << '\n';
    }

    Student(Student&& other) : name_{ std::move(other.name_) }
    {
      std::cout << "MOVE CTOR " << name_ << '\n';
    }
  private:
    std::string name_;
  };

  int main(){
    std::vector<Student> vec{
      "STUDENT_A",
      "STUDENT_B",
      "STUDENT_C"
    };
    // output ->
    //  COPY CTOR STUDENT_A
    //  COPY CTOR STUDENT_B
    //  COPY CTOR STUDENT_C

    std::cout << "capacity: " << vec.capacity() << '\n';
    // output -> capacity: 3

    // becuase of the capacity is 3, reallocation needs to be done
    // when push_back is called with "STUDENT_D"

    vec.push_back("STUDENT_D");
    // output ->
    //  MOVE CTOR STUDENT_D
    //  COPY CTOR STUDENT_A   ---------------> 
    //  COPY CTOR STUDENT_B   --------------->
    //  COPY CTOR STUDENT_C   --------------->

    // for std::string class, 
    // move constructor is much more cheaper than copy constructor
  }
*/

/*
  #include <vector>
  #include <string>

  class Student{
  public:
    Student(const char* name) : name_{ name } {}

    Student(const Student& other) : name_{ other.name_ } 
    {
      std::cout << "COPY CTOR " << name_ << '\n';
    }

    Student(Student&& other) noexcept: name_{ std::move(other.name_) }
    {
      std::cout << "MOVE CTOR " << name_ << '\n';
    }
  private:
    std::string name_;
  };

  int main(){
    std::vector<Student> vec{
      "STUDENT_A",
      "STUDENT_B",
      "STUDENT_C"
    };
    // output ->
    //  COPY CTOR STUDENT_A
    //  COPY CTOR STUDENT_B
    //  COPY CTOR STUDENT_C

    std::cout << "capacity: " << vec.capacity() << '\n';
    // output -> capacity: 3

    vec.push_back("STUDENT_D");
    // output ->
    //  MOVE CTOR STUDENT_D
    //  MOVE CTOR STUDENT_A   ---------------> 
    //  MOVE CTOR STUDENT_B   --------------->
    //  MOVE CTOR STUDENT_C   --------------->

    // push_back function is giving strong exception guarantee
    // when copy constructor called because of we are not 
    // changing the state of the elements(not steling their resources)
    // if an exception throw in the middle of the operation
    // easy to rollback the operation
    // copy constructor is giving strong exception guarantee

    // but move constructor is not giving strong exception guarantee
    // because of we are stealing the resources 
    // if an exception throw in the middle of the operation
    // we can not rollback the operation

    // if move constructor is `noexcept`, because of push_back function
    // has a strong exception guarantee, it can choose move constructor
    // instead of copy constructor, because it is giving 
    // no-throw guarantee
  }
*/

/*
  ---------------------------------------------------------------
  Question :  how much new storage created when reallocation 
              is done in the std::vector? 
  Answer : implementation defined, compiler dependent(2x or 1.5x)
  ---------------------------------------------------------------
  Question :  Created a std::vector with 1'000'000 elements,
              deleted 999'999 elements, only 1 left
              what is the capacity of the std::vector?
  Answer : capacity is not changed, it is still 1'000'000

  shrink_to_fit function can be used to shrink the capacity
  ---------------------------------------------------------------
*/

/*
  #include <vector>
  #include <string>
  #include <utility>      // std::move
  #include <chrono>       // std::steady_clock, std::duration
  #include <type_traits>  // std::is_nothrow_move_constructible_v

  class AClass{
  public:
    AClass() : m_str(500, 'A') {} 
    AClass(const AClass& other) = default;
    AClass(AClass&& other) noexcept(false) = default;
  private:
    std::string m_str;
  };

  class BClass{
  public:
    BClass() : m_str(500, 'A') {} 
    BClass(const BClass& other) = default;
    BClass(BClass&& other) = default;
  private:
    std::string m_str;
  };

  int main()
  {
    using namespace std;
    using namespace std::chrono;

    std::cout << std::boolalpha;

    vector<AClass> vecA(1'000'000);
    cout << "AClass move ctor is noexcept ? " 
         << is_nothrow_move_constructible_v<AClass> << '\n';

    cout << "vecA.capacity(): " << vecA.capacity() << '\n';

    auto tp_start = steady_clock::now();
    vecA.reserve(vecA.capacity() + 1);
    auto tp_end = steady_clock::now();

    cout << duration<double, std::milli>(tp_end - tp_start) << '\n';
    cout << "vecA.capacity(): " << vecA.capacity() << '\n';

    cout << "------------------------------------\n";

    cout << "BClass move ctor is noexcept ? " 
         << is_nothrow_move_constructible_v<BClass> << '\n';

    vector<BClass> vecB(1'000'000);
    cout << "vecB.capacity(): " << vecB.capacity() << '\n';

    tp_start = steady_clock::now();
    vecB.reserve(vecB.capacity() + 1);
    tp_end = steady_clock::now();

    cout << duration<double, std::milli>(tp_end - tp_start) << '\n';
    cout << "vecB.capacity(): " << vecB.capacity() << '\n';

    // output ->
    //  AClass move ctor is noexcept ? false
    //  vecA.capacity(): 1000000
    //  1027.76ms
    //  vecA.capacity(): 1000001
    //  ------------------------------------
    //  BClass move ctor is noexcept ? true
    //  vecB.capacity(): 1000000
    //  63.9915ms
    //  vecB.capacity(): 1000001
  }
*/

/*
                    --------------------
                    | moved from state |
                    --------------------
*/

/*
  STL's classes are giving some guarantees about moved from state
    - destructable
    - unspecified but in a valid state
    - invariants are not broken
*/

/*
  #include <utility> // std::move

  template <typename T>
  void Swap(T& x, T& y)
  {
    T temp = std::move(x);
    x = std::move(y);     // x is in moved from state
    y = std::move(temp);  // y is in moved from state
  }
  // using x and y, when they are in move from state
*/

/*
  #include <string>
  #include <utility>  // std::swap

  int main()
  {
    std::string s(30, 'A');
    auto& r1 = s;
    auto& r2 = s;

    std::cout << s << '\n';
    // output -> AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

    std::swap(r1, r2);

    std::cout << s << '\n';
    // output -> AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
  }
*/

/*
  - invariants are not broken

  i.e : Think about a linked_list implementation
    - head_pointer -> data member

    invariant : 
      - if list is empty, head_pointer should be nullptr
      - if minimum 1 element in the list, 
        head_pointer should point to the first Node
*/

/*
  class Myclass{

  private:
    int mx; 
    // Invariant : this data member should always be positive
  };
*/

/*
  Question :  Why compiler generated move members can create 
              a problem ?
    - data member's restraints can broke invariants
      i.e : some data members should always be in some range
      but in compiler generated move members, when an object
      becomes moved from state, it will set those data members 
      not in that range.

    - dependency between classes non-static data members 
    can broke invariants
      i.e : we have a Triangle class which have 3 double data 
      members that holding vertexes of the triangle and 1 double
      for the area of the triangle. When one of the vertexes changed
      area should also be changed.

    - pointer-like data members(iterators, smart pointers) 
    state broke the invariants
*/

/*
  #include <string>

  // Card class's move members are implicitly declared defaulted
  class Card{
  public:
    Card(const std::string& val) : m_val{ val }{}

    std::string get_value() const { return m_val; }
  private:
    std::string m_val;  // rank + "_of_" + suit
  };

  // Card class's invariant : even if it has been moved
  // it should always hold its value(m_val)
  int main()
  {
    Card c1{ "king_of_hearts" };
    auto c2 = std::move(c1);
    // c1 is in moved from state, it is data member(std::string)
    // becomes an empty string -> invariant is broken

    std::cout << c1.get_value() << '\n';
    // output -> ""
  }
*/

/*
  #include <string>   // std::to_string

  // IntString class's move members are implicitly declared defaulted
  class IntString{
  public:
    IntString(int val = 0) : 
        m_val{ val }, m_sval{ std::to_string(val) } {}

    void set_value(int i)
    {
      m_val = i;
      m_sval = std::to_string(i);
    }

    void print()const 
    {
      std::cout << "[" << m_val << "/'" << m_sval << "']\n";
    }  
  private:
    int m_val;          // value
    std::string m_sval; // cached string representation of the value
  };

  int main()
  {
    IntString isx{ 23 };
    IntString isy;
    
    std::cout << "isx and isy before move\n";
    isx.print();
    isy.print();

    isy = std::move(isx); // isx is in moved from state

    std::cout << "isx and isy after move\n";
    isx.print();
    isy.print();

    // output ->
    //  isx and isy before move
    //  [23/'23']
    //  [0/'0']
    //  isx and isy after move
    //  [23/'']    -> m_sval becomes empty string but m_val is still 23
    //  [23/'23']

    // IntString class's invariants becomes broken
  }
*/

/*
  #include <memory>   // std::shared_ptr, std::make_shared
  #include <string>   // std::to_string

  class SharedInt {
  public:
    explicit SharedInt(int val) : 
        m_sp{ std::make_shared<int>(val) } {}

    std::string as_string() const
    {
      return std::to_string(*m_sp);
      // if m_sp is nullptr, undefined behavior(ub)
    }
  private:
    std::shared_ptr<int> m_sp;
  };

  int main()
  {
    SharedInt x{ 22 };
    SharedInt y{ x }; // x and y share the value of 22

    std::cout << x.as_string() << '\n';   // output -> 22
    std::cout << y.as_string() << '\n';   // output -> 22

    std::cout << "-----------------\n";

    SharedInt a{ 44 };
    SharedInt b{ std::move(a) };
    // a's reference count becomes 0 (1 -> 0)

    std::cout << a.as_string() << '\n';  // undefined behavior(ub)
    // a is in moved from state, m_sp is nullptr

    // SharedInt class's invariants becomes broken
  }
*/