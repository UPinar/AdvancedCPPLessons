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

/*
  // sending more than one variant object to the visit function

  #include <variant>  // std::visit
  #include <string>

  struct VisClass{
    template <typename T, typename U>
    void operator() (const T& t, const U& u)
    {
      std::cout << "( " << t << ", " << u << " )\n";
    }
  };

  struct VisClass_2{
    // abbreviated template syntax (C++20)
    void operator() (const auto& t, const auto& u)
    {
      std::cout << "( " << t << ", " << u << " )\n";
    }
  };

  int main()
  {
    std::variant<int, double, std::string> vx(3.33);
    std::variant<long, int, std::string> vy(12);

    std::visit(VisClass{}, vx, vy);   // output -> ( 3.33, 12 )

    // --------------------------------------------------------

    vx = "hello world";
    vy = 45L;

    std::visit(VisClass{}, vx, vy);   // output -> ( hello world, 45 )

    // --------------------------------------------------------

    vx = 99;
    vy = 98;

    std::visit(VisClass_2{}, vx, vy); // output -> ( 99, 98 )

    // --------------------------------------------------------

    vx = 23.5;
    vy = "Istanbul";

    auto fn = [](const auto& t, const auto& u){
      std::cout << "( " << t << ", " << u << " )\n";
    };

    std::visit(fn, vx, vy);           // output -> ( 23.5, Istanbul )

    // --------------------------------------------------------
  }
*/

/*
  #include <variant>  // std::visit
  #include <string>

  struct VisClass{
    void operator() (double, std::string)
    {
      std::cout << "( double, std::string )\n";
    }

    void operator() (std::string, int)
    {
      std::cout << "( std::string, int )\n";
    }

    void operator() (double, long)
    {
      std::cout << "( double, long )\n";
    }

    void operator() (const auto&, const auto&)
    {
      std::cout << "( other, other )\n";
    }
  };

  int main()
  {
    std::variant<int, double, std::string> vx(3.33);
    std::variant<long, int, std::string> vy(12L);

    // --------------------------------------------------------

    std::visit(VisClass{}, vx, vy);   
    // output -> ( double, long )

    // --------------------------------------------------------

    vx = "hello world";
    vy = 45;

    std::visit(VisClass{}, vx, vy);
    // output -> ( std::string, int )

    // --------------------------------------------------------

    vx = 99;
    vy = 98;

    std::visit(VisClass{}, vx, vy);
    // output -> ( other, other )

    // --------------------------------------------------------
  }
*/

/*
  // union can not be a base class but std::variant can.

  #include <variant>
  #include <string>

  class Derived : public std::variant<int, std::string> {};

  int main()
  {
    using namespace std;

    Derived d = { "hello" };
    cout << "d.index() = " << d.index() << '\n';  
    // output -> d.index() = 1
    cout << "get<1>(d) = " << get<1>(d) << '\n';
    // output -> get<1>(d) = hello
    
    d.emplace<0>(12);
    cout << "get<0>(d) = " << get<0>(d) << '\n';
    // output -> get<0>(d) = 12
  }
*/

/*
                      --------------------
                      | overloader idiom |
                      --------------------
*/

/*
  struct A {
    void foo(int);
  };

  struct B {
    void foo(double);
  };

  struct Der : A, B {};

  int main()
  {
    Der dx;
    dx.foo(12);   // syntax error
    dx.foo(3.4);  // syntax error
    // error: request for member 'foo' is ambiguous

    dx.A::foo(12);   // VALID
    dx.B::foo(3.4);  // VALID
  }
*/

/*
  struct A {
    void foo(int) { std::cout << "A::foo(int)\n"; }
  };

  struct B {
    void foo(double) { std::cout << "B::foo(double)\n"; }
  };

  struct Der : A, B {
    using A::foo;   
    // "foo" identifier from struct A becomes visible in struct Der

    using B::foo;
    // "foo" identifier from struct B becomes visible in struct Der

    // Becuase of both "foo" identifiers(function tags)
    // are visible in struct Der(inside Der's scope) 
    // because of using declarations, "foo" functions become overloads.
  };

  int main()
  {
    Der dx;

    dx.foo(12);   // output -> A::foo(int)
    dx.foo(3.4);  // output -> B::foo(double)
  }
*/

/*
  template <typename ...Args>
  struct Der : Args... {
    using Args::foo...;
    // using A::foo, using B::foo, using C::foo;
  };

  struct A{
    void f_a(){}
    void foo(int){ std::cout << "A::foo(int)\n"; }
  };
  struct B{
    void f_b(){}
    void foo(double){ std::cout << "B::foo(double)\n"; }
  };
  struct C{
    void f_c(){}
    void foo(long){ std::cout << "C::foo(long)\n"; }
  };

  int main()
  {

    Der<A, B, C> d;    // A, B, C are base classes of Der

    // --------------------------------------------------------

    d.f_a(); // VALID
    d.f_b(); // VALID
    d.f_c(); // VALID

    // --------------------------------------------------------
    d.foo(12);   // output -> A::foo(int)
    d.foo(3.4);  // output -> B::foo(double)
    d.foo(45L);  // output -> C::foo(long)

    // --------------------------------------------------------
  }
*/

/*
  #include <string>

  struct A{
    A(int);
  };
  struct B{
    B(double);
  };
  struct C{
    C(long);
  };

  struct D : A, B, C{
  };

  int main()
  {
    D dx = { 12, 3.4, 33L };  // aggregate initialization
  }
*/

/*
  int main()
  {
    auto fn = [](int x){ return x * 5; };  // lambda expression

    // --------------------------------------------------------

    decltype(fn) fx1;  // syntax error before C++20
    // error: use of deleted function 'main()::<lambda(int)>::<lambda>()'
    // deleted default constructor

    decltype(fn) fx2;  // VALID since C++20

    // --------------------------------------------------------

    decltype(fn) fx3(fn);  // copy constructor
    // VALID in also C++17

    // --------------------------------------------------------

    int y = 0;
    auto fn2 = [y](int x){ return x * y; }; 
    // not a stateless lambda expression (capturing "y")

    decltype(fn2) fy1;  // syntax error
    // error: use of deleted function 'main()::<lambda(int)>::<lambda>()'

    // --------------------------------------------------------
  }
*/

/*
  auto fn1 = [](int x){ return x * 2; };
  auto fn2 = [](int x){ return x * 4; };
  auto fn3 = [](int x){ return x * 6; };

  struct A : decltype(fn1), decltype(fn2), decltype(fn3) {};
*/

/*
  auto fn2 = [](int x){ return x * 4; };
  auto fn3 = [](int x){ return x * 6; };

  struct A : decltype([](int x){ return x * 2; }), 
                                    decltype(fn2),
                                    decltype(fn3) 
  {}; 
  // syntax error before C++20
  // error: lambda-expression in unevaluated context 
  // only available with '-std=c++20' or '-std=gnu++20'
*/

/*
  #include <cmath>  // std::abs
  #include <set>

  int main()
  {
    using namespace std;

    auto fn = [](int x, int y){ return std::abs(x) < std::abs(y); };

    // --------------------------------------------------------

    set<int, decltype(fn)> myset;  // syntax error before C++20
    // default initialization will defult construct the lambda object
    // because of lambda object is not default constructible before
    // C++20, it will cause a syntax error.

    // --------------------------------------------------------

    set<int, decltype(fn)> myset_2(fn);  // VALID before C++20

    // --------------------------------------------------------

    set<int, decltype([](int x, int y){ 
                  return std::abs(x) < std::abs(y);})> myset_3;

    // VALID since C++20

    // --------------------------------------------------------
  }
*/

/*
  struct A{};

  struct B : A, A, A{}; // syntax error
  // error: duplicate base type 'A' invalid
*/

/*
  #include <variant>
  #include <type_traits>

  template <typename ...Args>
  struct AStruct : Args... {};

  int main()
  {
    using namespace std;

    auto f1 = [](){};
    auto f2 = [](){};
    auto f3 = [](){};

    auto b1 = is_same_v<decltype(f1), decltype(f2)>;  
    cout << boolalpha << b1 << '\n';  // output -> false

    auto b2 = is_same_v<decltype(f1), decltype(f3)>;  
    cout << boolalpha << b2 << '\n';  // output -> false
    // same lambda expression but different closure types

    AStruct<decltype(f1), decltype(f2), decltype(f3)> as;
  }
*/

/*
  template <typename ...Args>
  struct Overload : Args...{

  };

  int main()
  {
    Overload ox{
      [](int a){ return a * 2; },
      [](int a){ return a * 3; },
      [](int a){ return a * 4; },
      [](int a){ return a * 5; }
    };

    // template argümanlarının belirtilmemiş olmalarına karşın
    // argümanların, lambda ifadelerinde elde edilen closure
    // typelar olduğu çıkarım yoluyla elde edilir.
    // CTAD(Class Template Argument Deduction) mekanizması

    // syntax error before C++20
    // C++ 20 öncesi bu kodun sentaks hatası olmaması için
    // deduction guide(çıkarım kılavuzu) oluşturulması gerekiyordu.
  }
*/

/*
  template <typename T, typename U>
  struct Pair{
    Pair(const T& t, const U& u) : m_first(t), m_second(u) {}

  private:
    T m_first;
    U m_second;
  };

  // deduction guide
  template <typename T, typename U>
  Pair(T, U) -> Pair<T, U>;

  int main()
  {
    int a[10]{};
    double b[20]{};

    Pair p(a, b);

    // --------------------------------------------------------

    // constructor'ın parametresi referans olduğu için 
    // T türü için çıkarım(deduction) int* olmuyor.
    // T ==> int[10]
    // U ==> double[20]

    // Pair(const int(&)[10], const double(&)[20])
    // initializing int[10] m_first variable with int[10]
    // syntax error
    // error: array used as initializer

    // --------------------------------------------------------

    // before deduction guide "p"'s type is Pair<int[10], double[10]>
    // after deduction guide "p"'s type is Pair<int*, double*>

    // --------------------------------------------------------
  }
*/

/*
  #include <string>

  template <typename T>
  class Myclass {
  public:
    Myclass(const T& t);
  };

  Myclass(const char*)-> Myclass<std::string>;
  // Myclass'ın constructor'ına const char* türünden bir argüman
  // gönderildiğinde, deduction std::string türüne yapılacak.

  int main()
  {
    // --------------------------------------------------------
    
    // before deduction guide
    Myclass mx{"hello world"};
    Myclass<char[12]> my{"hello world"};
    // Those 2 lines are equivalent.

    // --------------------------------------------------------

    // after deduction guide
    Myclass mx{"hello world"};
    Myclass<std::string> my{"hello world"};
    // Those 2 lines are equivalent.

    // --------------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <list>
  #include <iterator>

  template <typename T>
  class Vector{};

  template <typename Iter>
  Vector(Iter, Iter) -> 
      Vector<typename std::iterator_traits<Iter>::value_type>;

  int main()
  {
    using namespace std;

    list<int> mylist = { 1, 2, 3, 4, 5, 6 };
    vector<int> myvec(mylist.begin(), mylist.end());  // range ctor

    vector myvec_2(mylist.begin(), mylist.end());
    // "myvec_2" data type is vector<int> (CTAD)
    // because of deduction guide has been used 
    // T will deduce to int not an iterator type
  }
*/

/*
  #include <variant>  // std::visit
  #include <string>

  template <typename ...Args>
  struct Overload : Args...{
    using Args::operator()...;
  };

  // before C++20, we need to define a deduction guide
  template <typename ...Args>
  Overload(Args...) -> Overload<Args...>;

  int main()
  {
    using namespace std;

    // --------------------------------------------------------

    variant<int, double, long, string, char, float> vx{ "hello" };

    visit(Overload{
      [](int)           { cout << "int\n"; },
      [](double)        { cout << "double\n"; },
      [](long)          { cout << "long\n"; },
      [](string)        { cout << "string\n";},
      [](auto)          { cout << "other\n";}
    }, vx);

    // output -> string 

    // --------------------------------------------------------

    vx = 3.45f;

    visit(Overload{
      [](int)           { cout << "int\n"; },
      [](double)        { cout << "double\n"; },
      [](long)          { cout << "long\n"; },
      [](string)        { cout << "string\n";},
      [](auto)          { cout << "other\n";}
    }, vx);

    // output -> other

    // --------------------------------------------------------
  }
*/

/*
  #include <variant>
  #include <string>

  template <typename ...Ts>
  struct overload : Ts...{
    using Ts::operator()...;
  };

  // before C++20 we need to define a deduction guide
  template <typename ...Ts>
  overload(Ts...) -> overload<Ts...>;

  int main()
  {
    using namespace std;

    // --------------------------------------------------------

    variant<int, string> vx(99);

    visit(overload{
      [](int ival) { cout << "int : " << ival << '\n'; },
      [](const string& str) { cout << "string : " << str << '\n'; }
    }, vx);
    // output -> int : 99

    auto twice = overload{
      [](string& s) { s += s; },
      [](auto& i) { i *= 2; },
    };

    visit(twice, vx);
    std::cout << get<0>(vx) << '\n';
    // output -> 198

    // --------------------------------------------------------

    vx = "hello world";

    visit(overload{
      [](int ival) { cout << "int : " << ival << '\n'; },
      [](const string& str) { cout << "string : " << str << '\n'; }
    }, vx);
    // output -> string : hello world

    visit(twice, vx);
    std::cout << get<string>(vx) << '\n';
    // output -> hello worldhello world

    // --------------------------------------------------------
  }
*/

/*
  #include <variant>
  #include <string>

  template <typename ...Ts>
  struct overload : Ts...{
    using Ts::operator()...;
  };

  int main()
  {
    using namespace std;

    // --------------------------------------------------------

    variant<int, double, string> vx(99);
    variant<int, long, string> vy("hello");

    visit(overload{
      [](int, int){ cout << "(int, int)\n";},
      [](int, string) { cout << "(int, string)\n";},
      [](double, long) { cout << "(double, long)\n";},
      [](auto, auto) { cout << "(auto, auto)\n";}
    }, vx, vy);
    // output -> (int, string)

    // --------------------------------------------------------

    vx = 3.45;
    vy = 45L;

    visit(overload{
      [](int, int){ cout << "(int, int)\n";},
      [](int, string) { cout << "(int, string)\n";},
      [](double, long) { cout << "(double, long)\n";},
      [](auto, auto) { cout << "(auto, auto)\n";}
    }, vx, vy);
    // output -> (double, long)

    // --------------------------------------------------------

    vx = "Istanbul";
    vy = 45L;

    visit(overload{
      [](int, int){ cout << "(int, int)\n";},
      [](int, string) { cout << "(int, string)\n";},
      [](double, long) { cout << "(double, long)\n";},
      [](auto, auto) { cout << "(auto, auto)\n";}
    }, vx, vy);
    // output -> (auto, auto)

    // --------------------------------------------------------
  }
*/

/*
  - when new alternative wanted to add to the variant object
    old alternative needs to be destroyed then new alternative
    will be constructed.
  
  - if constructor of the new alternative throws an exception
    variant object will be in a state that does not hold 
    any alternative.

  - to find out the variant object is in this kind of state
    valueless_by_exception() member function can be used.

https://en.cppreference.com/w/cpp/utility/variant/valueless_by_exception

  A variant may become valueless in the following situations:
    -> (guaranteed) an exception is thrown during the initialization 
        of the contained value during `move assignment`
    -> (optionally) an exception is thrown during the initialization 
        of the contained value during `copy assignment`
    -> (optionally) an exception is thrown when initializing 
        the contained value during a `type-changing assignment`
    -> (optionally) an exception is thrown when initializing 
        the contained value during a `type-changing emplace` 
*/

/*
  #include <stdexcept>
  #include <variant>
  #include <string>

  struct Demo {
      Demo(int) {}
      Demo(const Demo&) { throw std::runtime_error("copy ctor"); }
      Demo& operator= (const Demo&) = default;
  };

  int main()
  {
    using namespace std;
    cout << boolalpha;

    variant<std::string, Demo> var{ "hello world" };

    try{
      var = Demo{ 12 };
      // "var" is valueless which is an invalid state caused
      // by an exception raised in the process of 
      // `type-changing assignment`.
    }
    catch(const std::exception& ex){
      cout << "exception caught... " << ex.what() << '\n';
      cout << var.valueless_by_exception() << '\n';
      cout << "var.index() = " << var.index() << '\n';
      cout << (var.index() == variant_npos) << '\n';
    }
    // output ->
    //  exception caught... copy ctor
    //  true
    //  var.index() = 18446744073709551615
    //  true
  }
*/

/*
  // std::variant in generic programming 

  #include <variant> 

  using vtype = std::variant<int, double, long, char, float>;

  int main()
  {
    using namespace std;

    // --------------------------------------------------------

    // "variant_size" is a meta function

    constexpr auto total_elem_1 = variant_size<vtype>::value;  
    // total_elem_1 = 5

    constexpr auto total_elem_2 = variant_size_v<vtype>;  
    // total_elem_2 = 5

    // --------------------------------------------------------

    // "variant_alternative" is a meta function

    variant_alternative<1, vtype>::type x;
    // "x"'s data type is double

    variant_alternative_t<1, vtype> y;
    // "y"'s data type is double

    // --------------------------------------------------------
  }
*/

/*
  std::variant kullanım alanları
  ------------------------------

  - komut satırı argümanlarının işlenmesinde argümanların 
    türlerini variant nesnesi içinde tutmak.

  - bir fonksiyonda Data veya ErrorType türünden bir değer
    döndürmek.

  - kalıtıma(runtime polymorphism) virtual dispatch mekanizmasına
    alternatif olarak std::variant kullanmak.
    ->  her polimorfik nesne için fazladan 1 pointer ve her sınıf için 
        bir sanal fonksiyon veri yapısı bellek alanı maliyeti.
    -> her polimorfik nesne için bir allocation yapılması 
        (dinamik ömürlü nesne oluşturulması ve yok edilmesi) maliyeti.
*/

/*
  #include <variant>

  struct Data{};

  enum ErrorType { system, archive, log };

  std::variant<Data, ErrorType> foo();
*/

/*
  // It is guaranteed that NO OTHER ANIMAL will be added.
  // (Closed Hierarchy)

  #include <string>
  #include <variant>    // std::visit
  #include <list>
  #include <utility>    // std::move
  #include <algorithm>  // std::count_if

  class Cat {
  public:
    Cat(std::string name) : m_name{ std::move(name) } {}
    void meow() const { std::cout << m_name << " meow\n"; }
  private:
    std::string m_name;
  };

  class Dog {
  public:
    Dog(std::string name) : m_name{ std::move(name) } {}
    void woof() const { std::cout << m_name << " woof\n"; }
  private:
    std::string m_name;
  };

  class Lamb {
  public:
    Lamb(std::string name) : m_name{ std::move(name) } {}
    void bleat() const { std::cout << m_name << " bleat\n"; }
  private:
    std::string m_name;
  };

  using Animal = std::variant<Dog, Cat, Lamb>;

  template <typename T>
  bool is_type(const Animal& a)
  {
    return std::holds_alternative<T>(a);
  }

  // Visitor (functor class)
  struct AnimalVoice {
    void operator()(const Cat& c) const { c.meow(); }
    void operator()(const Dog& d) const { d.woof(); }
    void operator()(const Lamb& l) const { l.bleat(); }
  };

  int main()
  {
    using namespace std;

    list<Animal> animal_farm { Cat{ "c1" }, Dog{ "d1" }, Lamb{ "l1" },
                              Cat{ "c2" }, Lamb{ "l2" } };

    // --------------------------------------------------------

    for (const Animal& a : animal_farm){
      switch(a.index()){
      case 0:
        get<Dog>(a).woof();
        break;
      case 1:
        get<Cat>(a).meow();
        break;
      case 2:
        get<Lamb>(a).bleat();
        break;
      }
    }
    // output ->
    //  c1 meow
    //  d1 woof
    //  l1 bleat
    //  c2 meow
    //  l2 bleat

    // --------------------------------------------------------

    cout << "--------------------------------\n";

    for (const Animal& a : animal_farm){
      if (const auto dog_ptr = get_if<Dog>(&a))
        dog_ptr->woof();
      else if (const auto cat_ptr = get_if<Cat>(&a))
        cat_ptr->meow();
      else if (const auto lamb_ptr = get_if<Lamb>(&a))
        lamb_ptr->bleat();
    }

    // output ->
    //  c1 meow
    //  d1 woof
    //  l1 bleat
    //  c2 meow
    //  l2 bleat

    // --------------------------------------------------------

    cout << "--------------------------------\n";

    for (const Animal& a : animal_farm){
      visit(AnimalVoice{}, a);
    }

    // output ->
    //  c1 meow
    //  d1 woof
    //  l1 bleat
    //  c2 meow
    //  l2 bleat

    // --------------------------------------------------------

    cout << "--------------------------------\n";

    cout  << "inside animal farm " 
      << count_if(animal_farm.begin(), animal_farm.end(), is_type<Cat>)
      << " cats, "
      << count_if(animal_farm.begin(), animal_farm.end(), is_type<Dog>)
      << " dogs and"
      << count_if(animal_farm.begin(), animal_farm.end(), is_type<Lamb>)
      << " lambs are living.\n";
    // output -> inside animal farm 2 cats, 1 dogs and 2 lambs are living.

    // --------------------------------------------------------
  }
*/

/*
                      ------------------
                      | std::any class |
                      ------------------
*/

/*
  - programın çalışma zamanında any'nin tuttuğu türü değiştirebiliriz.
  - void* tutulan türe vakıf değil. any nesnesi tür bilgisini tutar.

  - any'nin construct edeceği nesneyi kendi içindeki bufferda tutma
    garantisi yok. sizeof değeri büyük nesneler için dinamik bellek
    tahsisi yapabilir.
*/

/*
  #include <any>
  #include <vector>

  int main()
  {
    using namespace std;
    boolalpha(cout);

    // --------------------------------------------------------

    any x1;     // default initialize
    any x2{};   // value initialize
    // both x1 and x2 objects are empty

    cout << x1.has_value() << '\n';  // output -> false
    cout << x2.has_value() << '\n';  // output -> false

    // --------------------------------------------------------

    any a1(12);
    any a2{ 3.4 };      
    any a3{ "hello world" };
    any a4{ "hello world"s };
    any a5(vector<int>{ 1, 2, 3, 4, 5 });

    // --------------------------------------------------------
  }
*/

/*
  #include <typeinfo>

  int main()
  {
    // --------------------------------------------------------

    std::type_info x; // syntax error
    // error: no matching function for call to 
    // 'std::type_info::type_info()'
    // std::type_info class's default ctor is not declared.

    // --------------------------------------------------------

    typeid(int);
    // "typeid(int)" expressions data type is "const std::type_info&"

    // --------------------------------------------------------

    auto t = typeid(int); // syntax error
    // error: use of deleted function 
    // 'std::type_info::type_info(const std::type_info&)'
    // std::type_info class's copy ctor is deleted.

    // --------------------------------------------------------
  }
*/

/*
  #include <string>
  #include <typeinfo>

  int main()
  {
    using namespace std;

    type_info& r = typeid(std::string);   // syntax error
    // error: binding reference of type 'std::type_info&' to 
    // 'const std::type_info' discards qualifiers

    const type_info& cr = typeid(std::string);  // VALID
  }
*/

/*
  #include <string>
  #include <typeinfo>

  int main()
  {
    using namespace std;
    cout << boolalpha;

    const type_info& cr = typeid(std::string);

    // --------------------------------------------------------

    cr.operator==(typeid(std::string)); 
    // operator==(const std::type_info&) const

    int x = 0;

    bool b1 = typeid(int) == typeid(x);
    cout << b1 << '\n';  // output -> true

    bool b2 = typeid(int).operator==(typeid(x));
    cout <<  b2 << '\n';  // output -> true

    // --------------------------------------------------------
  }
*/

/*
  #include <any>

  // global operator new() function overload
  void* operator new(std::size_t sz)
  {
    std::cout << "operator new called for the size of : " 
              << sz << '\n';

    if (sz == 0)
      ++sz;

    if (void* p = std::malloc(sz))
      return p;

    throw std::bad_alloc();
  }

  struct AStruct{
    char buf[256]{};
  };

  struct BStruct{
    char buf[8]{};
  };

  struct CStruct{
    char buf[9]{};
  };

  int main()
  {
    // --------------------------------------------------------

    std::cout << "sizeof(std::any) = " << sizeof(std::any) << '\n';
    // output -> sizeof(std::any) = 16
    // std::any object is holding type_info& (8 byte) inside of it.
    
    // --------------------------------------------------------

    auto p = new AStruct;
    // output -> operator new called for the size of : 256

    // --------------------------------------------------------

    std::any ax = 345;  
    // no dynamic allocation for int object

    std::any ay = BStruct{}; 
    // no dynamic allocation for BStruct object

    std::any az = CStruct{};
    // output -> operator new called for the size of : 9

    // for gcc compiler the buffer of std::any seems like 8 bytes.
    // dependent on the compiler it may be different.

    // --------------------------------------------------------
  }
*/

/*
  #include <any>
  #include <string>

  int main()
  {
    using namespace std;

    any a1(45);
    
    a1 = 3.4;
    a1 = "hello world";
    a1 = string("Istanbul");
  }
*/

/*
  #include <any>
  #include <string>

  int main()
  {
    using namespace std;

    // --------------------------------------------------------

    any{}.type();    
    // type member function's return type is const type_info&

    // --------------------------------------------------------

    any a1(45);             // output -> a1 is holding an int
    any a1(3.4);            // output -> a1 is holding double
    any a1("hello world"s); // output -> a1 is holding a string

    if (a1.type() == typeid(int))
      cout << "a1 is holding an int\n";
    else if (a1.type() == typeid(double))
      cout << "a1 is holding a double\n";
    else if (a1.type() == typeid(string))
      cout << "a1 is holding a string\n";

    // --------------------------------------------------------
  }
*/

/*
  #include <any>

  int main()
  {
    using namespace std;

    any a1;

    bool b = (a1.type() == typeid(void));
    cout << boolalpha << b << '\n';  // output -> true
  }
*/

/*
  #include <any>

  int main()
  {
    using namespace std;
    cout << boolalpha;

    int arr[10]{};
    any a1 = arr;

    cout << (a1.type() == typeid(int[10])) << '\n'; // output -> false
    cout << (a1.type() == typeid(int*)) << '\n';    // output -> true
  }
*/

/*
            -----------------------------------
            | std::any_cast function template |
            -----------------------------------
*/

/*
  #include <any>  // std::any_cast

  int main()
  {
    using namespace std;

    // --------------------------------------------------------

    any a{ 34 };
    cout << any_cast<int>(a) << '\n';  // output -> 34

    // --------------------------------------------------------
    
    try{
      a = 3.4;
      cout << any_cast<int>(a) << '\n';
      // holding double but trying to get int value,
      // it will throw an exception
    }
    // catch(const std::bad_any_cast& ex)
    // catch(const std::bad_cast& ex)
    catch(const std::exception& ex){
      cout << "exception caught... " << ex.what() << '\n';
    }
    // output -> exception caught... bad any_cast

    // --------------------------------------------------------
  }
*/

/*
  #include <any>

  int main()
  {
    using namespace std;

    any a{ 34 };

    // --------------------------------------------------------

    any_cast<int>(a) = 64;
    //  error: lvalue required as left operand of assignment
    // "any_cast<int>(a)" is PRValue expression

    // --------------------------------------------------------

    any_cast<int&>(a) = 64; // VALID
    // "any_cast<int&>(a)" is LValue expression

    cout << any_cast<int>(a) << '\n';  // output -> 64

    // --------------------------------------------------------
  }
*/

/*
  #include <any>
  #include <string>

  class Date{
  public:
    Date(int d, int m, int y) : 
        m_day{ d }, m_month{ m }, m_year{ y } {}

    friend std::ostream& operator<<(std::ostream& os, const Date& d)
    {
      return os << d.m_day 
                << '/' << d.m_month 
                << '/' << d.m_year;
    }
  private:
    int m_day;
    int m_month;
    int m_year;
  };

  int main()
  {
    using namespace std;

    // --------------------------------------------------------

    any a1{ in_place_type<Date>, 1, 1, 2001 };
    cout << any_cast<Date>(a1) << '\n';  // output -> 1/1/2001

    // --------------------------------------------------------

    auto a2 = make_any<Date>(2, 2, 2002);
    cout << any_cast<Date>(a2) << '\n';  // output -> 2/2/2002

    // --------------------------------------------------------

    auto a3 = make_any<string>(20, 'X');
    cout << any_cast<string>(a3) << '\n';  
    // output -> XXXXXXXXXXXXXXXXXXXX

    // --------------------------------------------------------
  }
*/

/*
  #include <any>

  int main()
  {
    using namespace std;
    cout << boolalpha;

    any a{ 34 };
    cout << a.has_value() << '\n';  // output -> true

    a.reset();
    cout << a.has_value() << '\n';  // output -> false
  }
*/

/*
  #include <any>

  class Date{
  public:
    Date(int d, int m, int y) : 
        m_day{ d }, m_month{ m }, m_year{ y } {}

    friend std::ostream& operator<<(std::ostream& os, const Date& d)
    {
      return os << d.m_day 
                << '/' << d.m_month 
                << '/' << d.m_year;
    }
  private:
    int m_day;
    int m_month;
    int m_year;
  };

  int main()
  {
    using namespace std;
    cout << boolalpha;

    // --------------------------------------------------------

    any a{ 34 };
    cout << a.has_value() << '\n';  // output -> true

    // --------------------------------------------------------

    a.reset();
    cout << a.has_value() << '\n';  // output -> false

    // --------------------------------------------------------
    a.emplace<Date>(1, 1, 2001);
    cout << a.has_value() << '\n';      // output -> true
    cout << any_cast<Date>(a) << '\n';  // output -> 1/1/2001
    
    // --------------------------------------------------------
  }
*/

/*
  #include <any>
  #include <string>
  #include <type_traits>    // std::is_same_v
  #include <utility>        // std::move

  int main()
  {
    using namespace std;

    any a1{ "istanbul"s };

    // --------------------------------------------------------

    auto& ra = any_cast<string&>(a1);
    ra[0] = 'I';
    ra.at(0) = 'I';
    ra.front() = 'I';

    cout << any_cast<const string&>(a1) << '\n';  
    // output -> Istanbul

    // --------------------------------------------------------

    auto str = any_cast<string&&>(move(a1));
    static_assert(is_same_v<decltype(str), string>);

    cout  << "a1.size() = " 
          << any_cast<const string&>(a1).size() << '\n';
    // output -> a1.size() = 0

    cout << "str : " << str << '\n';
    // output -> str : Istanbul

    // --------------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <any>
  #include <string>

  int main()
  {
    using namespace std;

    vector<any> avec{ 12, 3.4, "hello", 45L, "Istanbul"s};
    // initializer list constructor of vector class

    for (int cnt = 0; const auto& a : avec)
    {
      cout << "avec[" << cnt++ << "] = ";

      if (a.type() == typeid(int))
        cout << any_cast<int>(a) << '\n';
      else if (a.type() == typeid(double))
        cout << any_cast<double>(a) << '\n';
      else if(a.type() == typeid(const char*))
        cout << any_cast<const char*>(a) << '\n';
      else if (a.type() == typeid(long))
        cout << any_cast<long>(a) << '\n';
      else if (a.type() == typeid(string))
        cout << any_cast<string>(a) << '\n';
      }

      // output ->
      //  avec[0] = 12
      //  avec[1] = 3.4
      //  avec[2] = hello
      //  avec[3] = 45
      //  avec[4] = Istanbul
  }
*/

/*
  #include <any>

  int main()
  {
    using namespace std;
    
    // --------------------------------------------------------

    any ax{ 34.56 };

    if (any_cast<double>(&ax))
      cout << "ax is holding a double\n";
    else
      cout << "ax is not holding a double\n";
    // output -> ax is holding a double

    // --------------------------------------------------------

    ax = 45;
    if (any_cast<double>(&ax))
      cout << "ax is holding a double\n";
    else
      cout << "ax is not holding a double\n";
    // output -> ax is not holding a double

    // --------------------------------------------------------
    
    ax = 12.35;
    if (auto ptr = any_cast<double>(&ax)){
      cout << "ax is holding a double\n";
      cout << "value = " << *ptr << '\n';
    }
    // output ->
    //  ax is holding a double
    //  value = 12.35

    // --------------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <any>
  #include <string>

  int main()
  {
    using namespace std;

    vector<any> avec{ 12, 3.4, "hello", 45L, "Istanbul"s};

    for (int cnt = 0; const auto& a : avec)
    {
      if (auto pi = any_cast<int>(&a))
        cout << "int = " << *pi << '\n';
      else if(auto pd = any_cast<double>(&a))
        cout << "double = " << *pd << '\n';
      else if(auto pc = any_cast<const char*>(&a))
        cout << "const char* = " << *pc << '\n';
      else if(auto ps = any_cast<string>(&a))
        cout << "string = " << *ps << '\n';
      else if(auto pl = any_cast<long>(&a))
        cout << "long = " << *pl << '\n';
    }

    // output ->
    //  int = 12
    //  double = 3.4
    //  const char* = hello
    //  long = 45
    //  string = Istanbul
  }
*/

/*
  #include <any>
  #include <string>
  #include <utility>  // std::pair
  #include <vector>
  #include <iomanip>  // std::setw

  using tv_pair = std::pair<std::string, std::any>;

  int main()
  {
    using namespace std;

    vector<tv_pair> vec;
    vec.emplace_back("name", "hello world"s);
    vec.emplace_back("year", 2000);
    vec.emplace_back("month", 12);
    vec.emplace_back("day", 31);
    vec.emplace_back("wage", 3.14);
    vec.emplace_back("town", "Istanbul"s);

    cout << left;

    for (const auto& [property, value] : vec){
      if (value.type() == typeid(int))
        cout << setw(16) << property << any_cast<int>(value) << '\n';
      else if (value.type() == typeid(double))
        cout << setw(16) << property << any_cast<double>(value) << '\n';
      else if (value.type() == typeid(string))
        cout << setw(16) << property << any_cast<string>(value) << '\n';
    }
    // output ->
    //  name            hello world
    //  year            2000
    //  month           12
    //  day             31
    //  wage            3.14
    //  town            Istanbul
  }
*/