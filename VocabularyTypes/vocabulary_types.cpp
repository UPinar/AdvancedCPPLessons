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
*/
