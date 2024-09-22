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

// Lesson 3 -> 02:20:00



