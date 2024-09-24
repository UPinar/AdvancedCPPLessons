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
  #include <string>
  #include <utility>  // std::move
  #include <vector>
  #include <chrono>

  class Mystr{
  public:
    Mystr() : m_str(1000, 'A') {} 
    Mystr(const Mystr& other) : m_str(other.m_str) {}
    Mystr(Mystr&& other) noexcept : m_str(std::move(other.m_str)) {}
  private:
    std::string m_str;
  };

  int main()
  {
    using namespace std;
    using namespace std::chrono;

    vector<Mystr> vec(1'000'000);
    cout << "vec.capacity(): " << vec.capacity() << '\n';

    auto tp_start = steady_clock::now();

    vec.reserve(vec.capacity() + 1);

    auto tp_end = steady_clock::now();

    cout << duration<double, std::milli>(tp_end - tp_start).count() 
        << " ms" << '\n';
    cout << "vec.capacity(): " << vec.capacity() << '\n';
  }
*/
