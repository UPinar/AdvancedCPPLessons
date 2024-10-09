#include <iostream>

/*
                    ============================
                    | Vocabulary Types (C++17) |
                    ============================
*/

/*
                        -----------------
                        | std::optional |
                        -----------------
*/

/*
  - bir değere sahip olabilir ya da olmayabilir kavramından 
    istifade edilebilecek her yerde kullanılabilir.
  - bir değer tutmuyorsa (tutacağı değer sınıf türünden ise)
    sınıf türünden nesne boş yere construct edilmeyecek.
    (nullable type)
  - storage, optional nesnesinin kendisinin içinde tutulur.

          <--- check where_to_use_optional.png --->
*/

/*
  #include <optional>
  #include <string>
  #include <vector>

  int main()
  {
    std::optional<int> o_x;
    std::optional<std::string> o_str;
    std::optional<std::vector<int>> o_ivec;
  }
*/

/*
  // Checking whether it has a value or not
  // "has_value" and "operator bool" member functions

  #include <optional>

  int main()
  {
    using namespace std;

    optional<int> o_x;    // default initialization
    optional<int> o_y{};  // value initialization

    // --------------------------------------------------------

    if (o_x)    // if(o_x.operator bool())
      cout << "o_x has a value\n";
    else
      cout << "o_x does not have a value\n";

    // output -> o_x does not have a value

    // --------------------------------------------------------

    cout << (o_y.has_value()  ? "o_y has a value\n"
                              : "o_y does not have a value\n");
    // output -> o_y does not have a value

    // --------------------------------------------------------
  }
*/

/*
  #include <optional>

  int main()
  {
    using namespace std;

    optional<int> o_x{ nullopt };

    if (o_x)
      cout << "o_x has a value\n";
    else
      cout << "o_x does not have a value\n";

    // output -> o_x does not have a value

    cout << boolalpha;
    cout << (o_x == nullopt) << '\n';  // output -> true
  }
*/

/*
  #include <optional>
  #include <string>

  int main()
  {
    using namespace std;

    optional<string> o_str{ "hello World" };

    // --------------------------------------------------------

    // operator* function for reaching the value that optional holds
    cout << *o_str << '\n';  // output -> hello world

    // operator* function returns a reference
    *o_str = "hello Universe";
    cout << *o_str << '\n';  // output -> hello Universe

    // --------------------------------------------------------

    // operator-> function for reaching the value that optional holds

    cout << o_str->size() << '\n';  // output -> 14

    // --------------------------------------------------------
  }
*/

/*
  #include <optional>
  #include <string>

  int main()
  {
    using namespace std;

    optional<string> o_str{ "hello" };
    cout << *o_str << '\n';  // output -> hello

    *o_str += " world";
    cout << *o_str << '\n';  // output -> hello world

    o_str->assign("hello galaxy");
    cout << *o_str << '\n';  // output -> hello galaxy
  }
*/

/*
  #include <optional>

  int main()
  {
    using namespace std;

    optional o_x = 45;  // CTAD(C++17)
  }
*/

/*
  #include <string>
  #include <optional>

  int main()
  {
    using namespace std;

    optional<string> o_str;

    try{
      auto val = *o_str;        // undefined behaviour(UB)  
      auto n = o_str->size();   // undefined behaviour(UB)
      // try to use the value that optional holds when it is empty
    }
    catch (const bad_optional_access& e){
      cout << e.what() << '\n';
    }
  }
*/

/*
  // "value" member function will reach the value that optional holds
  // if it is empty it will throw an exception 
  // of type "bad_optional_access"

  #include <optional>
  #include <string>

  int main()
  {
    using namespace std;

    optional<string> o_str;

    try{
      auto val = o_str.value();
    }
    catch (const std::exception& ex){
      cout << "exception caught: " << ex.what() << '\n';
    }
    // output -> exception caught: bad optional access
  }
*/

/*
  // "value_or" member function 
  // will return the value that optional holds if it is not empty
  // otherwise it will return the value that is passed as an argument

  // "value_or" member function is not returning a reference
  // it is returning a copy of the value that optional holds

  #include <string>
  #include <optional>

  int main()
  {
    using namespace std;

    optional<string> o_str{ nullopt };
    optional<string> o_str2{ "hello World" };

    cout << o_str.value_or("empty") << '\n';  
    // output -> empty

    cout << o_str2.value_or("empty") << '\n';
    // output -> hello World
  }
*/

/*
  // optional objects can not hold references
  // optional objects can not reference_wrapper objects

  #include <optional>
  #include <functional> // std::reference_wrapper
  #include <string>

  int main()
  {
    using namespace std;

    // --------------------------------------------------------

    string str{ "hello" };
    optional<reference_wrapper<string>> o_str{ std::ref(str) };
    o_str->get() += " Galaxy";

    cout << "str = " << str << '\n';
    // output -> str = hello Galaxy

    // --------------------------------------------------------

    optional<string&> o_str1;  // syntax error
    //  error: static assertion failed
    //  static_assert(is_object_v<_Tp> && !is_array_v<_Tp>);

    // --------------------------------------------------------
  }
*/

/*
              ---------------------------------------
              | std::in_place_t | std::in_place tag |
              ---------------------------------------
*/

/*
  #include <optional>
  #include <utility>  // std::in_place

  class Myclass{
  public:
    Myclass()
    {
      std::cout << "Myclass default ctor\n";
    }

    Myclass(int, int, int)
    {
      std::cout << "Myclass(int, int, int) ctor\n";
    }

    Myclass(const Myclass&)
    {
      std::cout << "Myclass copy ctor\n";
    }

    Myclass(Myclass&&)
    {
      std::cout << "Myclass move ctor\n";
    }

    ~Myclass()
    {
      std::cout << "Myclass destructor\n";
    }
  };

  int main()
  {
    using namespace std;

    // --------------------------------------------------------

    optional<Myclass> o_x;  // empty optional object
    // Myclass object is not default constructed

    // --------------------------------------------------------

    // how to create an optional object that holds  
    // default constructed Myclass object inside it ?

    optional<Myclass> o_y{ Myclass{} };
    // output ->
    //  Myclass default ctor  
    //  Myclass move ctor
    //  Myclass destructor
    //  Myclass destructor

    // temporary Myclass object is default contrusted and
    // then it has been moved to the optional object's storage

    // --------------------------------------------------------

    optional<Myclass> o_z{ std::in_place };
    // output -> 
    //  Myclass default ctor
    //  Myclass destructor

    // if optional's in_place_t overload is being used to construct
    // the arguments will be perfect forwarded to optional's
    // storage object's constructor

    // --------------------------------------------------------

    optional<Myclass> o_w{ std::in_place , 1, 2, 3 };
    // output ->
    //  Myclass(int, int, int) ctor
    //  Myclass destructor

    // arguments will be perfect forwarded to the Myclass's 
    // Myclass(int, int, int) ctor

    // --------------------------------------------------------
  }
*/

/*
  #include <optional> // std::make_optional
  #include <utility>  // std::in_place

  class Myclass{
  public:
    Myclass()
    {
      std::cout << "Myclass default ctor\n";
    }

    Myclass(int, int, int)
    {
      std::cout << "Myclass(int, int, int) ctor\n";
    }

    ~Myclass()
    {
      std::cout << "Myclass destructor\n";
    }
  };

  int main()
  {
    // --------------------------------------------------------

    auto op = std::make_optional<Myclass>();
    auto op2 = std::optional<Myclass>(std::in_place);
    // Those 2 lines are equivalent.

    // both output ->
    //  Myclass default ctor
    //  Myclass destructor  

    // --------------------------------------------------------

    auto op3 = std::make_optional<Myclass>(1, 2, 3);
    auto op4 = std::optional<Myclass>(std::in_place, 1, 2, 3);
    // Those 2 lines are equivalent.

    // both output ->
    //  Myclass(int, int, int) ctor
    //  Myclass destructor

    // --------------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <string> 
  #include <optional>   // std::make_optional
  #include "../nutility.h"

  int main()
  {
    using namespace std;

    // --------------------------------------------------------

    optional<string> op1(in_place, 10, 'A');
    // string(10, 'A');   // fill constructor

    optional<vector<int>> op2(in_place, { 1, 2, 3, 4, 5 });
    // initializer list parameter constructor

    cout << *op1 << '\n';   // output -> AAAAAAAAAA
    print(*op2);            // output -> 1 2 3 4 5

    // --------------------------------------------------------

    auto op3 = make_optional<string>(10, 'B');
    auto op4 = make_optional<vector<int>>({ 6, 7, 8, 9, 10 });

    cout << *op3 << '\n';   // output -> BBBBBBBBBB
    print(*op4);            // output -> 6 7 8 9 10

    // --------------------------------------------------------
  } 
*/

/*
  // ways to make an optional object empty

  #include <optional>
  #include <string>

  int main()
  {
    using namespace std;
    cout << boolalpha;

    optional<string> o_str;

    // --------------------------------------------------------

    cout << "has value? : " << o_str.has_value() << '\n';
    // output -> has value? : false

    o_str = "hello";
    cout << "has value? : " << o_str.has_value() << '\n';
    // output -> has value? : true

    o_str.reset();  // string objects destructor will be called
    cout << "has value? : " << o_str.has_value() << '\n';
    // output -> has value? : false
    
    // --------------------------------------------------------

    o_str = "world";
    cout << "has value? : " << o_str.has_value() << '\n';
    // output -> has value? : true

    o_str = nullopt;  // string objects destructor will be called
    cout << "has value? : " << o_str.has_value() << '\n';
    // output -> has value? : false

    // --------------------------------------------------------

    o_str = "hello";
    cout << "has value? : " << o_str.has_value() << '\n';
    // output -> has value? : true

    o_str = optional<string>{};
    cout << "has value? : " << o_str.has_value() << '\n';
    // output -> has value? : false

    // --------------------------------------------------------
    
    o_str = "world";
    cout << "has value? : " << o_str.has_value() << '\n';
    // output -> has value? : true

    o_str = {};
    cout << "has value? : " << o_str.has_value() << '\n';
    // output -> has value? : false

    // --------------------------------------------------------
  }
*/

/*
  #include <optional>

  class Myclass{
  public:
    Myclass()
    {
      std::cout << "Myclass default ctor\n";
    }

    Myclass(int x)
    {
      std::cout << "Myclass(int) ctor, x = " << x << '\n';
    }

    ~Myclass()
    {
      std::cout << "Myclass destructor\n";
    }
  };

  int main()
  {
    using namespace std;

    optional<Myclass> o_x;

    // --------------------------------------------------------
    
    o_x.emplace();
    cout << "[1] main continues\n";
    o_x.emplace(10);
    cout << "[2] main continues\n";

    // output ->
    //  Myclass default ctor
    //  [1] main continues
    //  Myclass destructor
    //  Myclass(int) ctor, x = 10
    //  [2] main continues

    // --------------------------------------------------------

    for(int i = 0; i < 3; ++i)
      o_x.emplace(i);

    // output ->
    //  Myclass destructor
    //  Myclass(int) ctor, x = 0
    //  Myclass destructor
    //  Myclass(int) ctor, x = 1
    //  Myclass destructor
    //  Myclass(int) ctor, x = 2
    //  Myclass destructor

    // --------------------------------------------------------
  }
*/

/*
  // std::optional usage scenarios
  // <--- check where_to_use_optional.png --->

  #include <optional>
  #include <string>

  std::optional<std::string> get_person_nick(int id);
  void add_record(std::optional<std::string> nick);

  class UserRecord{
  private:
    std::optional<std::string> m_nick;
  };
*/

/*
  #include <optional>
  #include <string>

  int main()
  {
    using namespace std;
    cout << boolalpha;

    optional<int> o_x = 10;
    optional<int> o_y = 10;

    // --------------------------------------------------------

    cout << (o_x == o_y) << '\n';  // output -> true
    cout << (o_x < o_y) << '\n';   // output -> false

    cout << (o_x == 10) << '\n';   // output -> true
    cout << (o_x < 10) << '\n';    // output -> false

    // --------------------------------------------------------

    optional<int> o_z = nullopt;
    optional<int> o_v = nullopt;
    optional<int> o_w = -1282;

    cout << (o_z == o_v) << '\n';  // output -> true
    cout << (o_w < o_z) << '\n';   // output -> false

    // --------------------------------------------------------

    using namespace std::literals;
    optional<string> o_str1 = "hello"s;
    optional<string> o_str2;

    cout << (o_str2 < o_str1 ) << '\n';  // output -> true

    // --------------------------------------------------------

    optional<bool> o_b1;
    optional<bool> o_b2{ false };
    optional<bool> o_b3{ true };

    cout << (o_b1 == o_b2) << '\n';   // output -> false
    cout << (o_b1 == o_b3) << '\n';   // output -> false
    cout << (o_b2 == o_b3) << '\n';   // output -> false

    cout << (o_b1 < o_b2) << '\n';    // output -> true
    cout << (o_b1 < o_b3) << '\n';    // output -> true

    // --------------------------------------------------------
  }
*/

/*
  #include <optional>
  #include <string>

  bool has_nick(int);

  std::optional<std::string> get_person_nick_1(int id)
  {
    if (has_nick(id))
      return "nick";        // returning not an empty optional object
    else
      return std::nullopt;  // returning an empty optional object
      // return {};
      // return std::optional<std::string>{};
      // Those 3 lines are equivalent.
  }

  std::optional<std::string> get_person_nick_2(int id)
  {
    std::optional<std::string> op;

    if (has_nick(id))
      op = "nick";

    return op;
  }

  int main()
  {
    using namespace std;
    
    // -- to prevent scope leakage
    if (auto op = get_person_nick_1(10); op) {}
    if (auto op = get_person_nick_1(10); op.operator bool()) {}
    // Those 2 lines are equivalent.

    if (auto op = get_person_nick_1(10); op->length() > 10) {}
  }
*/

/*
  #include <optional>
  #include <algorithm>  // std::find_if
  #include <utility>    // std::forward

  template<typename Con, typename Pred>
  auto Find_if(Con&& con, Pred&& pred)
  {
    using std::begin, std::end; 
    // using declaration for if std::begin and std::end customized,
    // customized versions will be used.
    // else standart versions will be used.

    auto beg_iter = begin(con);
    auto end_iter = end(con);
    auto result = std::find_if(beg_iter, end_iter, pred);

    using iterator = decltype(result);

    if (result == end_iter)
      return std::optional<iterator>();
    
    return std::optional<iterator>(result);
  }

  template<typename Con, typename T>
  auto Find(Con&& con, const T& val)
  {
    return Find_if(std::forward<Con>(con), 
                    [&val](auto&& x) { return x == val; });
  }
*/

/*
  #include <optional>
  #include <string>     // std::stoi

  std::optional<int> to_int(const std::string& str)
  {
    try {
      return std::stoi(str);
    }
    catch (...){
      return std::nullopt;
    }
  }

  std::optional <int> to_int_2(const std::string& str)
  {
    std::optional<int> ret;
    try {
      ret = std::stoi(str);
    }
    catch (...) {}

    return ret;
  }

  int main()
  {
    for (auto s : { "42", "077", "hello", "0x42" })
    {
      std::optional<int> op = to_int(s);

      if (op)
        std::cout << s << " converted to int: " << *op << '\n';
      else
        std::cout << "(" << s << ") can not be converted to int\n";
    }

    // output ->
    //  42 converted to int: 42
    //  077 converted to int: 77
    //  (hello) can not be converted to int
    //  0x42 converted to int: 0 --> because of first character is '0'
  }
*/

/*
  #include <optional>
  #include <string>
  #include <utility>  // std::move

  int main()
  {
    using namespace std;

    // --------------------------------------------------------

    optional<string> op1{ "hello world" };
    cout << "op1->length() = " << op1->length() << '\n';
    // output -> op1->length() = 11

    auto op2 = op1; // copy semantics

    cout << "op1->length() = " << op1->length() << '\n';
    // output -> op1->length() = 11

    // --------------------------------------------------------

    auto op3 = std::move(op1); // move semantics
    cout << "op1->length() = " << op1->length() << '\n';
    // output -> op1->length() = 0

    // --------------------------------------------------------

    op1 = "hello universe";
    cout << "op1->length() = " << op1->length() << '\n';
    // output -> op1->length() = 14

    auto op4 = std::move(*op1); // also move semantics
    cout << "op1->length() = " << op1->length() << '\n';
    // output -> op1->length() = 0

    // --------------------------------------------------------
  }
*/

/*
  #include <optional>
  #include <string>
  #include <utility>  // std::move

  int main()
  {
    using namespace std;
    cout << boolalpha;

    optional<string> op1{ "hello world" };

    optional<string> op2 = std::move(*op1);
    // std::string object inside the std::optional object
    // has not been destroyed has been moved,
    // std::string object is now in moved-from state

    cout << op1.has_value() << '\n';  // output -> true
    cout << op2.has_value() << '\n';  // output -> true

    *op1 = "hello universe";      // VALID
    cout << op1.value() << '\n';  // output -> hello universe
  }
*/

/*
  #include <optional>

  struct Mystruct{
    std::optional<double> mopt_d1;
    std::optional<double> mopt_d2;
  };
  // std::optional<double> has a double and a bool data members
  // but because of std::optional has been using aligned storage
  // size will be increased

  struct Mystruct2{
    bool has_mx;
    bool has_my;
    double mx;
    double my;
  };

  int main()
  {
    using namespace std;

    constexpr auto size_Mystruct = sizeof(Mystruct);
    // output -> size_Mystruct = 32

    constexpr auto size_Mystruct2 = sizeof(Mystruct2);
    // output -> size_Mystruct2 = 24
  }
*/

/*
            ------------------------------------------
            | `std::variant` variadic class template |
            ------------------------------------------
*/

/*
  value type : 
    - Bir değişkene, başka bir değişkenin değeri atandığında
    kendisine atama yapılan değişken ile diğer değişken 
    birbirlerinden tamamen bağımsızlar. 
    - Farklı nesneler farklı değerlere sahipler.
    - Paylaşılan bir varlık yok. (value semantics)
    Örneğin iki std::optional nesne birbirlerine atandığında
    ikisi arasında bir kaynak paylaşımı söz konusu değil.
*/

/*
  variant<int, double, std::string> v;
  - std::variant object can hold int or double or std::string
  - those types are called alternative types of std::variant object
*/

/*
  #include <variant>
  #include <string>

  template <typename ...Args>
  class Variant{};

  int main()
  {
    Variant<int, double, std::string> v1;
    // T zamanında "v1" std::variant objesi 
    // int, double veya std::string tutuyor olabilir.

    Variant<int, double, int> v2;

    Variant<std::monostate, int, double> v3;
    // if v3 holds std::monostate object, 
    // it can be used as an empty object
  }
*/

/*
  #include <variant>
  #include <string>

  class Myclass{
  public:
    Myclass(int);
  };

  int main()
  {
    using namespace std;

    // --------------------------------------------------------

    variant<int, double, long, string> v1;    // default initialized
    variant<int, double, long, string> v2{};  // value initialized

    // default constructed variant object holds 
    // the first alternative type which is int for "v1" and "v2"

    // --------------------------------------------------------

    // if the first alternative type is not default constructible
    // variant object can not be default initialized
    // it will cause a syntax error.

    variant<Myclass, int, double> v3;  // syntax error
    // error: no matching function for call to 
    // 'std::variant<Myclass, int, double>::variant()'

    // --------------------------------------------------------
  }
*/ 

/*
  #include <variant>
  #include <string>
  #include <string_view>

  int main()
  {
    using namespace std;

    // --------------------------------------------------------

    variant<int, double, char> vx{ 12 };
    // vx holds int alternative

    variant<int, double, char> vy{ 3.4 };
    // vy holds double alternative

    variant<int, double, char> vz{ 'A' };
    // vz holds char alternative

    // --------------------------------------------------------

    variant<int, float, char> v1{ 3.4 };  
    // syntax error - ambiguity

    variant<int, double, char> v2{ 5.3f };  // VALID
    // float ==> double (promotion)
    // double alternative will be selected

    // --------------------------------------------------------

    variant<int, string, string_view> v3{ "hello" };    
    // syntax error - ambiguity

    variant<int, string, string_view> v4{ "hello"s };   // VALID

    variant<int, string, string_view> v5{ "hello"sv };  // VALID

    // --------------------------------------------------------
  }
*/

/*
  // how to learn which alternative that variant object holds
  // - index() member function
  // - holds_alternative<T>() global function template

  #include <variant>

  int main()
  {
    using namespace std;

    // --------------------------------------------------------

    variant<int, double, char> vx;
    cout << vx.index() << '\n';  // output -> 0

    vx = 12;
    cout << vx.index() << '\n';  // output -> 0

    vx = 5.6;
    cout << vx.index() << '\n';  // output -> 1

    vx = 'A';
    cout << vx.index() << '\n';  // output -> 2

    // --------------------------------------------------------

    cout << boolalpha;

    variant<int, double, char> vy;
    cout << holds_alternative<int>(vy) << '\n';    // output -> true

    vy = 12;
    cout << holds_alternative<int>(vy) << '\n';    // output -> true
    cout << holds_alternative<double>(vy) << '\n'; // output -> false

    vy = 5.6;
    cout << holds_alternative<double>(vy) << '\n';  // output -> true
    cout << holds_alternative<char>(vy) << '\n';    // output -> false

    vy = 'A';
    cout << holds_alternative<char>(vy) << '\n';    // output -> true
    cout << holds_alternative<int>(vy) << '\n';     // output -> false

    // --------------------------------------------------------
  }
*/

/*
  #include <variant>

  int main()
  {
    std::variant<int, double, long> vx;

    auto b = std::holds_alternative<char>(vx);  // syntax error
    // error: static assertion failed: 
    // T must occur exactly once in alternatives
    // compile time error
  }
*/

/*
  // std::in_place_index_t  => in_place_index<> constexpr variable
  // std::in_place_type_t   => in_place_type<> constexpr variable

  #include <variant>

  class Date{
  public:
    Date(int, int, int) {}
  };

  int main()
  {
    using namespace std;

    // --------------------------------------------------------

    variant<int, string, double> v1{ in_place_index<1>, 10, 'A' };
    variant<int, string, Date> v2{ in_place_index<2>, 1, 1, 2001 };

    // arguments will be perfect forwarded to the constructor of the
    // alternative type that has been selected by in_place_index<>

    // --------------------------------------------------------

    variant<int, string, double> v3{ in_place_type<string>, 10, 'B' };
    variant<int, string, Date> v3{ in_place_type<Date>, 2, 2, 2002 };

    // --------------------------------------------------------
  }
*/

/*
  // how to default construct Myclass object in a variant

  #include <variant>

  class Myclass{
  public:
    Myclass() { std::cout << "Myclass default ctor\n"; }
    Myclass(const Myclass&) { std::cout << "Myclass copy ctor\n"; }
  };

  int main()
  {
    // --------------------------------------------------------

    std::variant<Myclass, int, double> v1;
    // output -> Myclass default ctor

    // --------------------------------------------------------

    std::variant<int, Myclass, double> v2{ Myclass{} };
    // output -> 
    //  Myclass default ctor
    //  Myclass copy ctor

    std::variant<int, Myclass, double> v3{ std::in_place_type<Myclass> };
    // output -> Myclass default ctor

    std::variant<int, Myclass, double> v4{ std::in_place_index<1> };
    // output -> Myclass default ctor

    // --------------------------------------------------------
  }
*/

/*
  #include <variant>

  struct AClass{
    int m_x, m_y;
  };

  struct BClass{
    double m_x, m_y;
  };

  struct CClass{
    unsigned char m_buffer[256];
  };

  using var_type = std::variant<AClass, BClass, CClass>;

  int main()
  {
    std::cout << "sizeof(AClass) = " << sizeof(AClass) << '\n';
    // output -> sizeof(AClass) = 8
    std::cout << "sizeof(BClass) = " << sizeof(BClass) << '\n';
    // output -> sizeof(BClass) = 16
    std::cout << "sizeof(CClass) = " << sizeof(CClass) << '\n';
    // output -> sizeof(CClass) = 256

    std::cout << "sizeof(var_type) = " << sizeof(var_type) << '\n';
    // output -> sizeof(var_type) = 264

    // variant object's size will be the size of the 
    // largest alternative + an integer to hold the index 
    // of the alternative and some alignment same as std::optional
  }
*/

/*
  // global get<T>() function template

  #include <variant>

  int main()
  {
    using namespace std;

    variant<int, double, long> vx(45);

    // --------------------------------------------------------

    cout << get<0>(vx) << '\n';  // output -> 45
    
    // --------------------------------------------------------

    try{
      cout << get<1>(vx) << '\n';
    }
    // catch (const std::exception& ex){
    catch (const std::bad_variant_access& ex)
    {
      cout << "exception caught : " <<ex.what() << '\n';
    }
    // output -> exception caught : std::get: wrong index for variant
    // run-time exception will be thrown

    // --------------------------------------------------------

    cout << get<3>(vx) << '\n';  // syntax error
    // error: static assertion failed
    // static_assert(_Np < sizeof...(_Types));

    // compile-time error -> out of range index

    // --------------------------------------------------------
  }
*/

/*
  #include <variant>

  int main()
  {
    using namespace std;

    variant<int, double, long> vx(4.53);

    get<1>(vx) = 8.978;   // reference semantics

    cout << "get<1>(vx) = " << get<1>(vx) << '\n';
    // output -> get<1>(vx) = 8.978
  }
*/

/*
  #include <variant>
  #include <string>

  int main()
  {
    using namespace std;

    variant<int, double, string> vx("hello world");

    cout << get<2>(vx) << '\n';       // output -> hello world
    cout << get<string>(vx) << '\n';  // output -> hello world

    get<string>(vx) = "hello universe";
    cout << get<string>(vx) << '\n';  // output -> hello universe
  }
*/

/*
  // get_if<T>() global function template (pointer semantics)

  #include <variant>
  #include <string>

  int main()
  {
    using namespace std;

    // --------------------------------------------------------

    variant<int, double, string> vx("hello world");

    auto p_str = get_if<string>(&vx);
    // "get_if<string>(&vx)" expression's data type is std::string*

    cout << *p_str << '\n';  // output -> hello world

    // --------------------------------------------------------

    // if the variant object does not hold the alternative type
    // that is passed to get_if<T>() function template
    // it will return nullptr

    auto p_d = get_if<double>(&vx);

    if (p_d == nullptr)
      cout << "vx does not hold double\n";
    else
      cout << "vx holds double, value = " << *p_d << '\n';

    // vx does not hold double

    // --------------------------------------------------------
  }
*/

/*
  // ways to reach the value that variant object holds

  #include <variant>
  #include <string>

  int main()
  {
    using namespace std;

    variant<int, double, string> vx("hello world");

    if (vx.index() == 2)
      cout << "alternative string : " << get<2>(vx) << '\n';
    // output -> alternative string : hello world

    if (holds_alternative<string>(vx))
      cout << "alternative string : " << get<string>(vx) << '\n';
    // output -> alternative string : hello world

    if (auto p_str = get_if<string>(&vx))
      cout << "alternative string : " << *p_str << '\n';
    // output -> alternative string : hello world
  }
*/

/*
  #include <variant>
  #include <string>

  int main()
  {
    using namespace std;

    // --------------------------------------------------------

    variant<int, double, string> vx(34.567);

    if (holds_alternative<int>(vx))
      cout << "alternative int : " << get<int>(vx) << '\n';
    else if(holds_alternative<double>(vx))
      cout << "alternative double : " << get<double>(vx) << '\n';
    else if(holds_alternative<string>(vx))
      cout << "alternative string : " << get<string>(vx) << '\n';
    // output -> alternative double : 34.567

    // --------------------------------------------------------

    vx = 12;

    if (vx.index() == 0)
      cout << "alternative int : " << get<0>(vx) << '\n';
    else if(vx.index() == 1)
      cout << "alternative double : " << get<1>(vx) << '\n';
    else if(vx.index() == 2)
      cout << "alternative string : " << get<2>(vx) << '\n';
    // output -> alternative int : 12

    // --------------------------------------------------------

    vx = "hello world";

    if (auto p_i = get_if<int>(&vx))
      cout << "alternative int : " << *p_i << '\n';
    else if (auto p_d = get_if<double>(&vx))
      cout << "alternative double : " << *p_d << '\n';
    else if (auto p_str = get_if<string>(&vx))
      cout << "alternative string : " << *p_str << '\n';
    // output -> alternative string : hello world

    // --------------------------------------------------------
  }
*/

/*
  #include <variant>
  #include <string>

  int main()
  {
    enum idx : size_t { age, salary, name };

    using Age = int;
    using Salary = double;
    using Name = std::string;

    std::variant<Age, Salary, Name> vx(45); 

    std::cout << get<age>(vx) << '\n';  // get<index>
    // output -> 45
    std::cout << get<Age>(vx) << '\n';  // get<type>
    // output -> 45

    // --------------------------------------------------------

    vx = "hello world";

    std::cout << get<name>(vx) << '\n';  // get<index>
    // output -> hello world
    std::cout << get<Name>(vx) << '\n';  // get<type>
    // output -> hello world
  }
*/

/*
  // ways to assign a value to a variant object

  #include <variant>
  #include <string>

  int main()
  {
    std::variant<int, double, std::string> vx;

    // --------------------------------------------------------

    vx = 12;
    vx = 3.3;
    vx = "hello world"; 

    vx = std::string(10, 'A');  // VALID

    // --------------------------------------------------------

    vx.emplace<int>(13);  
    vx.emplace<double>(4.4);
    vx.emplace<std::string>("hello galaxy");

    std::cout << get<std::string>(vx) << '\n';
    // output -> hello galaxy

    vx.emplace<0>(14);  
    vx.emplace<1>(5.5);
    vx.emplace<2>("hello universe");

    std::cout << get<2>(vx) << '\n';
    // output -> hello universe

    // --------------------------------------------------------
  }
*/

/*
  #include <variant>

  class AClass{
  public:
    AClass() { std::cout << "AClass default ctor\n"; }
    ~AClass() { std::cout << "AClass destructor\n"; }
  };

  class BClass{
  public:
    BClass() { std::cout << "BClass default ctor\n"; }
    ~BClass() { std::cout << "BClass destructor\n"; }
  };

  int main()
  {
    using namespace std;

    variant<int, AClass, BClass> vx;

    std::cout << "[1] main continues\n";
    vx.emplace<1>();
    std::cout << "[2] main continues\n";
    vx.emplace<2>();
    std::cout << "[3] main continues\n";

    // output ->
    //  [1] main continues
    //  AClass default ctor
    //  [2] main continues
    //  AClass destructor
    //  BClass default ctor
    //  [3] main continues
    //  BClass destructor

    // old variant objects destructor will be called when 
    // emplace member function is called
  }
*/

/*
                ------------------------
                | std::monostate class |
                ------------------------
*/

/*
  - when a class's all data members and member functions are static
    all of the objects from this class will have a same state.
  - static member functions will be called in the same way.
  
  those kind of classes are called monostate classes.
*/

/*
  advantages of using std::monostate as an alternative type
  in a variant object :
    - because of std::monostate have only one state, 
      can be used in comparison oparetions and 
      can be used in checking the alternative type
      (it is like empty alternative type[empty variant object])

    - some of the alternatives in variant might not be 
      default constructible, but we want to create a variant
      using those types. In this case std::monostate can be used
      as a first alternative.
*/

/*
  #include <variant>

  struct AClass{
    AClass(int);
  };

  struct BClass{
    BClass(double);
  };

  int main()
  {
    using namespace std;
    variant<AClass, BClass> v_1;   // syntax error
    variant<BClass, AClass> v_2;   // syntax error

    variant<monostate, AClass, BClass> v_3; // VALID
  }
*/

/*
  #include <variant>
  #include <string>

  int main()
  {
    using namespace std;

    variant<monostate, int, double, string> vx;

    // --------------------------------------------------------

    if (vx.index() == 0)
      cout << "monostate\n";
    // output -> monostate

    if (holds_alternative<monostate>(vx))
      cout << "monostate\n";
    // output -> monostate

    if (get_if<monostate>(&vx))
      cout << "monostate\n";
    // output -> monostate

    // --------------------------------------------------------

    variant<monostate, int, double, string> vy;

    vy = monostate{};
    vy = {};  
    // default constructed variant object 
    // holds monostate alternative in this scenario

    vy.emplace<monostate>();
    vy.emplace<0>();

    // --------------------------------------------------------
  }
*/

/*
            ---------------------------------------------
            | `std::visit()` variadic function template |
            ---------------------------------------------
*/

/*
  template <class Visitor, class ...Variants>
  constexpr visit(Visitor&& vis, Variants&&... vars);

  - callable will sent to this function can be L value or R value
  - variants will sent to this function can be L value or R value

  -> visit function will sent the variant's(2nd parameter) alternative 
    to the callable object(1st parameter) that has been sent to the 
    visit function.
*/

/*
  #include <variant>  // std::visit
  #include <string>

  struct PrintVisitor{
    // operator call function for variant's int alternative
    void operator()(int x) const
    {
      std::cout << "int : " << x << '\n';
    }

    // operator call function for variant's double alternative
    void operator()(double x) const  
    {
      std::cout << "double : " << x << '\n';
    }

    // operator call function for variant's string alternative
    void operator()(const std::string& s) const
    {
      std::cout << "string : " << s << '\n';
    }
  };

  int main()
  {
    using namespace std;

    variant<int, double, string> vx("hello world");

    PrintVisitor pv;
    std::visit(pv, vx);
    // output -> string : hello world

    vx = 12;
    std::visit(pv, vx);
    // output -> int : 12

    vx = 3.4;
    std::visit(PrintVisitor{}, vx);
    // output -> double : 3.4

    // first parameter(callable) can be L value or R value
  }
*/

/*
  #include <variant>  // std::visit
  #include <string>

  struct PrintVisitor{
    // member template
    template <typename T>
    void operator()(const T& t) const
    {
      std::cout << "[" << t << "]\n";
    }
  };

  int main()
  {
    using namespace std;

    variant<int, double, string> vx("hello world");
    PrintVisitor pv;

    std::visit(pv, vx);
    // output -> [hello world]

    vx = 12;
    std::visit(pv, vx);
    // output -> [12]

    vx = 3.4;
    std::visit(pv, vx);
    // output -> [3.4]
  }
*/

/*
  #include <variant>  // std::visit
  #include <string>

  struct PrintVisitor{
    // abbreviated template syntax (C++20) [member template]
    void operator()(const auto& t) const
    {
      std::cout << "[" << t << "]\n";
    }
  };

  int main()
  {
    using namespace std;

    variant<int, double, string> vx("hello world");
    PrintVisitor pv;

    std::visit(pv, vx);
    // output -> [hello world]

    vx = 12;
    std::visit(pv, vx);
    // output -> [12]

    vx = 3.4;
    std::visit(pv, vx);
    // output -> [3.4]
  }
*/

/*
  #include <variant>  // std::visit
  #include <string>
  #include <type_traits>

  struct PrintVisitor{
    template <typename T>
    void operator()(const T& t) const
    {
      if constexpr(std::is_same_v<T, int>)
        std::cout << "int : " << t << '\n';
      else if constexpr(std::is_same_v<T, double>)
        std::cout << "double : " << t << '\n';
      else if constexpr(std::is_same_v<T, std::string>)
        std::cout << "string : " << t << '\n';

      // compile time (static if)
    }
  };

  int main()
  {
    using namespace std;

    variant<int, double, string> vx("hello world");
    PrintVisitor pv;

    std::visit(pv, vx);

    vx = 12;
    std::visit(pv, vx);

    vx = 3.4;
    std::visit(pv, vx);

    // output ->
    //  string : hello world
    //  int : 12
    //  double : 3.4
  }
*/

/*
  #include <variant>  // std::visit
  #include <string>

  struct ChangeVisitor{
    template <typename T>
    void operator()(T& t) const
    {
      if constexpr(std::is_same_v<T, int>)
        ++t;
      else if constexpr(std::is_same_v<T, double>)
        t *= t;
      else if constexpr(std::is_same_v<T, std::string>)
        t += t;
    }
  };

  struct PrintVisitor{
    template <typename T>
    void operator()(const T& t) const
    {
      if constexpr(std::is_same_v<T, int>)
        std::cout << "int : " << t << '\n';
      else if constexpr(std::is_same_v<T, double>)
        std::cout << "double : " << t << '\n';
      else if constexpr(std::is_same_v<T, std::string>)
        std::cout << "string : " << t << '\n';
    }
  };

  int main()
  {
    using namespace std;

    variant<int, double, string> vx("hello world");
    PrintVisitor pv;
    ChangeVisitor cv;

    // --------------------------------------------------------

    std::visit(pv, vx);
    // output -> string : hello world

    std::visit(cv, vx);
    std::visit(pv, vx);
    // output -> string : hello worldhello world

    // --------------------------------------------------------

    vx = 12;
    std::visit(pv, vx);
    // output -> int : 12

    std::visit(cv, vx);
    std::visit(pv, vx);
    // output -> int : 13

    // --------------------------------------------------------

    vx = 3.4;
    std::visit(pv, vx);
    // output -> double : 3.4

    std::visit(cv, vx);
    std::visit(pv, vx);
    // output -> double : 11.56

    // --------------------------------------------------------
  }
*/

/*
  #include <variant>  // std::visit
  #include <string>

  int main()
  {
    using namespace std;

    variant<int, double, string> vx(3.33);

    auto pr = [](const auto& t) { cout << "[" << t << "]\n"; };

    visit(pr, vx);  // output -> [3.33]

    vx =12;
    visit(pr, vx);  // output -> [12]

    vx = "hello world";
    visit([](const auto& t) { cout << "[" << t << "]\n"; }, vx);
    // output -> [hello world]
  }
*/

/*
  #include <variant>  // std::visit
  #include <string>

  struct AlternativeVisitor{
    void operator()(int) { std::cout << "int\n";}
    void operator()(double) { std::cout << "double\n";}
    void operator()(const std::string&) { std::cout << "string\n";}

    void operator()(auto) { std::cout << "other alternatives\n";}
  };

  int main()
  {
    using namespace std;

    variant<int, double, string, long long, char> vx(12);
    AlternativeVisitor vis;

    visit(vis, vx);
    // output -> int

    vx = 3.4;
    visit(vis, vx);
    // output -> double

    vx = "hello world";
    visit(vis, vx);
    // output -> string

    vx = 12LL;
    visit(vis, vx);
    // output -> other alternatives

    vx = 'A';
    visit(vis, vx);
    // output -> other alternatives
  }
*/
