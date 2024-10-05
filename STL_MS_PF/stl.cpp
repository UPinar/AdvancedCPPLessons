#include <iostream>

/*
          ------------------------------------------------
          | Move Semantics and Perfect Forwarding in STL |
          ------------------------------------------------
*/

/*
  // how to choose parameter type in functions

  #include <string>
  #include <algorithm>  // std::reverse

  // will use std::string class for read-only operations
  // but inside the function, we need to copy the string
  void foo(const std::string& str)
  {
    auto str_temp = str; // copied the string

    reverse(str_temp.begin(), str_temp.end());
    // not using qualified name(std::) for reverse function
    // because ADL(argument dependent lookup) is being used.
    // "begin" and "end" member function's types are 
    // std::string::iterator  
    // so "reverse" identifier will be searched in std namespace
  }

  // better making parameter variable as non-reference type
  void foo(std::string str)
  {
    reverse(str.begin(), str.end());
  }

  // ADVANTAGE: when PRValue expression is passed to 
  //  non-reference type parameter as an argument, 
  //  mandatory copy ellision will happen.
*/

/*
  // move-only types

  class MoveOnly{
  public:
    MoveOnly() = default;

    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;

    MoveOnly(MoveOnly&&) = default;
    MoveOnly& operator=(MoveOnly&&) = default;
  };
  // when one of move members are user declared,
  // copy members will implicitly declared deleted.
  // but it can also be explicitly declared deleted

  // move members can be, 
  //  - user declared defaulted
  //  - user declared to be defined.


  void func(MoveOnly);

  int main()
  {
    MoveOnly mx;

    func(mx); // syntax error
    // "mx" is an LValue expression, so it will be copied

    func(MoveOnly{});
    // MoveOnly{} is a PRValue expression
    // mandatory copy ellision will happen

    func(std::move(mx));
    // std::move(mx) is an XValue expression
    // "mx" has been move to parameter variable 
  }
*/

/*
  // std::initializer_list class reminder

  #include <initializer_list>

  void func(std::initializer_list<int> param)
  {
    std::cout << "&param           = " << &param << '\n';
    std::cout << "address of array = " << param.begin() << '\n';
  }

  void foo(int a, int b, int c)
  {
    for (auto val : { a, b, c })
      std::cout << val << ' ';
    std::cout << '\n';
  }

  void bar(std::initializer_list<int> param)
  {
    for (auto val : param)
      std::cout << val << ' ';
    std::cout << '\n';
  }

  int main()
  {
    // -------------------------------------------------------

    std::initializer_list<int> my_list = { 1, 2, 3, 4, 5 };
    // compiler will generate an array of integers
    // integers inside initializer_list will be COPIED to array

    // -------------------------------------------------------

    // initializer_list object holds only 2 pointers
    // first pointer holds the address of the first element
    // second pointer holds the address of the end of the array

    std::cout << "sizeof(my_list) = " << sizeof(my_list) << '\n';
    // output -> sizeof(my_list) = 16(2 * 8byte)

    // -------------------------------------------------------

    std::cout << "&my_list         = " << &my_list << '\n';
    std::cout << "address of array = " << my_list.begin() << '\n';
    // output ->
    //  &my_list         = 0xfa64dff940
    //  address of array = 0x7ff6532250b0

    func(my_list);  
    // initializer_list object will be copied to parameter variable
    // output ->
    //  &param           = 0xfa64dff8e0
    //  address of array = 0x7ff6532250b0


    // &my_list and &param will be different 
    // but param.begin() and my_list.begin() will be same
    // (copied data member pointers)

    // -------------------------------------------------------

    int x = 10, y = 20, z = 30;

    foo(x, y, z);       // output -> 10 20 30
    // parameter variables used as an initializer_list

    bar( { x, y, z} );  // output -> 10 20 30
    // parameter variables sent as an initializer_list

    // -------------------------------------------------------
  }
*/

// continue from Lesson-6 00:30:20
