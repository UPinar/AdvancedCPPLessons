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

// Lesson_8 : 01:43:30