#ifndef MYCLASS_HPP
#define MYCLASS_HPP

#include <iostream>

#ifdef MYCLASS_1
class Myclass{
public:
  Myclass(){
    std::cout << "Myclass() - default ctor\n";
  }

  ~Myclass(){
    std::cout << "~Myclass() - destructor\n";
  }

  Myclass(int){
    std::cout << "Myclass(int)\n";
  }

  Myclass(const Myclass&){
    std::cout << "Myclass(const Myclass&) - copy ctor\n";
  }

  Myclass(Myclass&&){
    std::cout << "Myclass(Myclass&&) - move ctor\n";
  }

  Myclass& operator=(const Myclass&){
    std::cout << "copy assignment\n";
    return *this;
  }

  Myclass& operator=(Myclass&&){
    std::cout << "move assignment\n";
    return *this;
  }
};
#elif defined(MYCLASS_2)
  class Myclass{
  public:
    Myclass(){
      std::cout << "Myclass() - default ctor\n";
    }

    ~Myclass(){
      std::cout << "~Myclass() - destructor\n";
    }

    Myclass(int){
      std::cout << "Myclass(int)\n";
    }

    Myclass(const Myclass&) = delete;
    Myclass& operator=(const Myclass&) = delete;
    Myclass(Myclass&&) = delete;
    Myclass& operator=(Myclass&&) = delete; 

    // int foo(){
    //   std::cout << "Myclass::foo()\n";
    //   return 5;
    // }
  };

#elif defined(MYCLASS_3)  
class Myclass{
public:
  Myclass(){
    std::cout << "Myclass() - default ctor\n";
  }

  ~Myclass(){
    std::cout << "~Myclass() - destructor\n";
  }

  Myclass(int){
    std::cout << "Myclass(int)\n";
  }

  Myclass(Myclass&&){
    std::cout << "Myclass(Myclass&&) - move ctor\n";
  }

  Myclass& operator=(Myclass&&){
    std::cout << "move assignment\n";
    return *this;
  }

  Myclass(const Myclass&) = delete;
  Myclass& operator=(const Myclass&) = delete;
};
#endif

#endif // MYCLASS_HPP