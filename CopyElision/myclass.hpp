#ifndef MYCLASS_HPP
#define MYCLASS_HPP

#include <iostream>

#ifdef SCENARIO_1
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
    std::cout << "Myclass& operator=(const Myclass&) - copy assignment\n";
    return *this;
  }

  Myclass& operator=(Myclass&&){
    std::cout << "Myclass& operator=(Myclass&&) - move assignment\n";
    return *this;
  }

  int foo(){
    std::cout << "Myclass::foo()\n";
    return 5;
  }
};
#endif // SCENARIO_1

#ifdef SCENARIO_2
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

  int foo(){
    std::cout << "Myclass::foo()\n";
    return 5;
  }

};
#endif // SCENARIO_2

#endif // MYCLASS_HPP