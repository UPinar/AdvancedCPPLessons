#include <iostream>

/*
            ============================================
            | Class Template Argument Deduction (CTAD) |
            ============================================
*/

/*
  template <typename T>
  class Myclass{
  public:
    Myclass(const T&) {}
  };

  template <typename T>
  Myclass<T> make_myclass(const T& t)
  {
    return Myclass<T>(t);
  }

  int main()
  {
    Myclass<int> m1(10);
    auto m2 = make_myclass(20);

    Myclass m3(30); // CTAD   -> Myclass<int>
    Myclass m4(1.2); // CTAD  -> Myclass<double>
  }
*/

/*
  template <typename T>
  class Myclass{
  public:
    explicit Myclass(const T&) {}
  };

  int main()
  {
    Myclass m1 = 10;  // syntax error
    // error: class template argument deduction failed
  }
*/

/*
  template <typename T, typename U>
  class Myclass{
  public:
    Myclass(const T&, const U&) {
      std::cout << typeid(T).name() << " "; 
      std::cout << typeid(U).name() << '\n';
    }
  };

  int main()
  {
    Myclass m1{ 34, 6.7 };      // output -> int double
    Myclass m2{ 'A', "hello" }; // output -> char char[6]
  }
*/

/*
  template <typename T, std::size_t N>
  class Myclass{
  public:
    Myclass(T(&)[N])
    // constructor's parameter is a reference 
    // to an array of N elements of type T
    {
      std::cout << "type T -> " << typeid(T).name() << " ";
      std::cout << "constant N = " << N <<  '\n';
    }
  };

  int main()
  {
    // ---------------------------------------------------

    int arr[10]{};
    Myclass m1{ arr };
    // output -> type T -> int constant N = 10

    // ---------------------------------------------------

    double darr[] = { 1.2, 3.4, 5.6 };
    Myclass m2{ darr };
    // output -> type T -> double constant N = 3

    // ---------------------------------------------------
  }
*/

/*
  template <typename T, typename U, std::size_t N>
  class Myclass{
  public:
    Myclass(T(*)(U(&)[N]))
    {
      std::cout << "type T -> " << typeid(T).name() << " ";
      std::cout << "type U -> " << typeid(U).name() << " ";
      std::cout << "constant N = " << N <<  '\n';
    }
    // constructor's parameter is a pointer to a function 
    // that takes a reference to an array of N elements of type U
    // and returns a value of type T
  };

  int foo(double(&)[20]) { return 0; }
  // foo's parameter variable is a pointer to a function
  // that takes a reference to an array of 20 elements of type double
  // and returns an int

  int bar(long(&)[30]) { return 0; }

  int main()
  {
    // ---------------------------------------------------
    
    Myclass m1(foo);
    // output -> type T -> int type U -> double constant N = 20

    // ---------------------------------------------------

    Myclass m2(bar);
    // output -> type T -> int type U -> long constant N = 30

    // ---------------------------------------------------
  }
*/

/*
  template <typename T = double>  // default template argument
  struct Myclass{
    T m_val;
    Myclass() : m_val() {}
    Myclass(T val) : m_val(val) {}
  };

  int main()
  {
    Myclass m1{ 20 };   // Myclass<int>


    Myclass m2;         // Myclass<double>
    // default ctor will be called for "m2" object 
    // because there is default template argument
    // deduction will be applied.
  }
*/

/*
  template <typename T = int, typename U = double, typename W = long>
  class Myclass{
  public:
    Myclass(T = T{}, U = U{}, W = W{}) {
      std::cout << typeid(T).name() << " ";
      std::cout << typeid(U).name() << " ";
      std::cout << typeid(W).name() << '\n';
    }
  };

  int main()
  {
    Myclass m1;           // output -> int double long
    // Myclass<int, double, long>

    Myclass m2{ 9.9 };    // output -> double double long
    // Myclass<double, double, long>

    Myclass m3{ 10, 'A' }; // output -> int char long
    // Myclass<int, char, long>

    Myclass m4{ 10L, 9U, 'A' }; // output -> long unsigned char
    // Myclass<long, unsigned int, char>
  }
*/

/*
  #include <array>

  template <typename T, std::size_t N>
  struct Array{};

  int main()
  {
    // ---------------------------------------------------

    std::array a1{ 1, 2, 3, 4, 5 };       // VALID
    // std::array<int, 5>
    // ---------------------------------------------------

    std::array<int> a2{ 1, 2, 3, 4, 5 };  // syntax error
    // error: wrong number of template arguments (1, should be 2)

    // ---------------------------------------------------

    std::array<> a3{ 1, 2, 3, 4, 5 };     // syntax error
    // error: wrong number of template arguments (0, should be 2)

    // ---------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <list>
  #include <set>

  int main()
  {
    using namespace std;

    vector vec{ 1, 2, 3, 4 };         
    // vector<int>

    list lst{ 1.2, 2.4, 3.6, 4.8 };  
    // list<double>

    set s1{ 2, 4, 6, 8 };         
    // set<int, std::less<int>>

    set s2{ { 2, 4, 6, 8 }, [](int a, int b){ return b < a; } };
    // set<int, lambda>
  }
*/

/*
  template <typename T>
  class Myclass{
  public:
    Myclass(T) 
    {
      std::cout << "type of T -> " << typeid(T).name() << '\n';
    }
  };

  int main()
  {
    Myclass m1(std::less<int>{}); 
    // output -> type of T -> struct std::less<int>

    Myclass m2([](int x){ return x * x; });
    // output -> type of T -> class <lambda_...> (closure type)
  }
*/

/*
  #include <utility>    // std::forward
  #include <algorithm>  // std::sort
  #include <functional> // std::ref
  #include <string>

  #include "../nutility.h"

  template <typename F>
  class CountCalls{
  public:
    CountCalls(F f) : m_f(f) {}

    template <typename... Args>   // member template
    auto operator()(Args&&... args)
    {
      ++m_count;
      return m_f(std::forward<Args>(args)...);
    }

    int count() const { return m_count; }
  private:
    F m_f;
    int m_count{};
  };

  int main()
  {
    using namespace std;

    vector<string> msvec;
    rfill(msvec, 1'000'000, rname);

    auto f = CountCalls{ [](const string& str_1, const string& str_2){
                              return str_1.size() < str_2.size();
                            }};

    sort(msvec.begin(), msvec.end(), ref(f));
    // if we pass "f" instead of "ref(f)" as the third argument
    // it will copy the "f" object and the count that our "f"
    // object will be 0 the count that the copied object will be
    // incremented.

    cout << "Number of calls: " << f.count() << '\n';
    // output -> Number of calls: 18'052'530
  }
*/

/*
  #include <algorithm>  // std::find_if
  #include <functional> // std::ref
  #include <vector>

  class Pred{
  public:
    bool operator()(int) const;
  private:
    int m_a[1024];
  };

  int main()
  {
    using namespace std;

    vector<int> ivec(100'000);

    find_if(ivec.begin(), ivec.end(), Pred{});
    // "find_if" function's 3rd parameter is a predicate
    // and it is not a reference type.
    // So, the predicate object will be copied.
    // but our "Pred{}" object is 1024 * sizeof(int) bytes long.
    // it is not a good idea to copy it.

    Pred pred;
    auto iter = find_if(ivec.begin(), ivec.end(), std::ref(pred));
    // better using "std::ref" to avoid copying the predicate object.
  }
*/

/*
  #include <vector>
  #include <algorithm>  // std::for_each

  #include "../nutility.h"

  class Functor{
  public:
    Functor(int val) : m_val(val) {}

    void operator()(int x)
    {
      if (x > m_val)
        ++m_count;
    }

    int get_count() const { return m_count; }

  private:
    int m_val;
    int m_count{};
  };

  int main()
  {
    std::vector<int> ivec;
    rfill(ivec, 200'00, Irand{ 0, 100'000 });

    auto f = for_each(ivec.begin(), ivec.end(), Functor{ 90'000 });
    // std::for_each algorithm is returning the callable 
    // that we passed as the third argument.

    std::cout << f.get_count() << '\n'; // output -> 2001
  }
*/

/*
  #include <vector>

  int main()
  {
    using namespace std;

    vector<int> v1(1000);

    vector v2{ v1 };      // copy constructor      
    // "v2"s type is vector<int>

    vector v3{ v1, v1 };  // initializer_list constructor 
    // "v3"s type is vector<vector<int>>

    vector v4(v1, v1);  // syntax error
    // error: no matching function for call to 
    // 'vector(std::vector<int>&, std::vector<int>&)'
  }
*/

/*
  #include <list>
  #include <string>
  #include <vector>
  #include <utility> // std::pair

  int main()
  {
    using namespace std;

    list<pair<string, double>> mylist{
      { "A", 1.2 },
      { "B", 3.4 },
      { "C", 5.6 },
      { "D", 7.8 }
    };

    // ---------------------------------------------------

    vector myvec( mylist.rbegin(), mylist.rend() ); // ( not braces )
    // range constructor will be called

    for (const auto& [str, val] : myvec)
      cout << str << " -> " << val << '\n';

    // output ->
    //  D -> 7.8
    //  C -> 5.6
    //  B -> 3.4
    //  A -> 1.2

    // ---------------------------------------------------

    vector myvec2{ mylist.rbegin(), mylist.rend() }; // { braces }
    // initializer_list constructor will be called

    // "myvec2"'s type is  vector<reverse_iterator<...>>

    // ---------------------------------------------------
  }
*/

/*
  #include <complex>


  template <typename T>
  class Complex{
  public:
    Complex(const T& re = T(), const T& im = T());
  private:
    T m_re;
    T m_im;
  };

  int main()
  {
    std::complex c1{ 2.3, 4.5 };  
    // "c1" data type is std::complex<double>

    // ---------------------------------------------------

    std::complex c2{ 2.3 };
    // "c2" data type is std::complex<double>

    // ---------------------------------------------------

    std::complex c3; // syntax error
    // error: no matching function for call to 'complex()'

    // ---------------------------------------------------

    std::complex<double> c4;  // VALID
    // "c4" data type is std::complex<double>

    // ---------------------------------------------------

    std::complex c5{ 2.3f, 4.5 }; // syntax error
    // error: no matching function for call to 'complex(float, double)'

    // ---------------------------------------------------
  }
*/

/*
  #include <functional>   // std::function

  int foo(int);
  double bar(double, double);

  struct Functor{
    void operator()(int);
  };

  int main()
  {
    std::function f1{ foo };
    // std::function<int(int)>

    std::function f2{ &foo };
    // std::function<int(int)>

    std::function f3{ bar };
    // std::function<double(double, double)>

    std::function f4{ Functor{} };
    // std::function<void(int)>

    std::function f5{ [](int x, int y) { return x + y; } };
    // std::function<int(int, int)>
  }
*/

/*
  #include <utility> // std::pair
  #include <vector>

  int main()
  {
    using namespace std;

    // ---------------------------------------------------

    vector<pair> vec1{ { 1, 2 }, { 3, 4 }, { 5, 6 } };  // syntax error
    // can not deduce template arguments for "pair"

    // ---------------------------------------------------

    vector<pair<int, double>> vec2{ { 1, 2.3 }, { 4, 5.6 }};  // VALID
    // "vec2"s type is vector<pair<int, double>>

    // ---------------------------------------------------

    vector vec3 { pair{ 1, 2.3 }, pair{ 4, 5.6 } };  // VALID
    // initializer_list constructor will be called
    // vec3's type is vector<pair<int, double>>

    // ---------------------------------------------------
  }
*/

/*
  // specialization usage is not affecting the CTAD

  template <typename T>
  struct MyStruct{
    MyStruct(T){ std::cout << "Primary template\n"; }
  };

  // explicit(full) specialization
  template <>
  struct MyStruct<int>{
    MyStruct(const double& x){ 
      std::cout << "MyStruct<int> specialization\n"; }
  };

  int main()
  {
    MyStruct m{ 10 }; 
    // output -> MyStruct<int> specialization

    MyStruct m2{ 10.5 };
    // output -> Primary template
  }
*/

/*
  // CTAD from standart library

  #include <optional>
  #include <atomic>
  #include <tuple>
  #include <mutex>
  #include <complex>

  std::mutex mtx;

  int main()
  {
    using namespace std;

    optional opt{ 10 };
    // optional<int>

    atomic atom_1{ 10 };
    // atomic<int> -> std::atomic_int

    tuple tx{ 10, 20.5, "hello" };
    // tuple<int, double, const char*>

    lock_guard guard{ mtx };
    // lock_guard<std::mutex>

    complex c{ 2.3, 4.5 };
    // complex<double>
  }
*/

/*
  #include <utility> // std::pair

  int main()
  {
    using namespace std::literals;

    std::pair x{ 10, 4.5 }; 
    // pair<int, double>

    std::pair y{ "hello"s, "world" };
    // pair<std::string, const char*>

    std::pair z{ 7u, 3.5f };
    // pair<unsigned int, float>
  }
*/

/*
  #include <complex>

  template <typename T>
  class Complex{
    constexpr Complex(const T& re = T(), const T& im = T()) {}
  };

  int main()
  {
    std::complex c1{ 1.3f, 2.6f };  // std::complex<float>
    std::complex c2( 1.3, 2.6 );    // std::complex<double>
    std::complex c3 = 1.5;          // std::complex<double>
    std::complex c4 = { 5.3 };      // std::complex<double>

    std::complex c5{5, 3.3};  // syntax error
    // error: no matching function for call to 'complex(int, double)'
  }
*/

// Lesson_10 : 01:37:00