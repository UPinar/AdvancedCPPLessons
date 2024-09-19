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
