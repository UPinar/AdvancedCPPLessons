#include <iostream>

/*
                      ===================
                      | `format` module |
                      ===================
*/

/*
  - <iostream> kütüphanesinde bulunan her stream nesnesi 
    bir format state tutuyor.
  - Bir çıkış işlemi yapılmak istendiği zaman, çıkış işlemini 
    yapan fonksiyon stream(akım) nesnesinin format state'ini
    kullanarak çıkış işlemini gerçekleştirir.
*/

/*
  <cstdio> 
  ----------------
  int printf(const char* p_format, ...);
  int sprintf(char* p_buffer, const char* p_format, ...);
  int fprintf(FILE* p_file, const char* p_format, ...);

  Disadvantages: 
  - Not type safe (variadic functions)
  - Not extendable (can not be used with user defined types)
  Advantages:
  - Faster than <iostream> 
  - Splitting arguments and conversion specifiers

  <iostream>
  ----------------
  Disadvantages:
  - Slower than <cstdio>
  - Complex usage of formatting states
  Advantages:
  - Type safe (using operator overloading)
  - Extendable (can be used with user defined types)

  <format>
  ----------------
  - Type safe
  - Extendable
  - Faster than <iostream> <cstdio>
*/

/*
                    -----------------------
                    | <iostream> REMINDER |
                    -----------------------
*/

/*
  int main()
  {
    using namespace std;

    // -------------------------------------------------------

    ios_base::fmtflags;
    // Her stream nesnesi fmtflags türünden bir veri elemanı tutuyor.

    // -------------------------------------------------------

    cout.setf(ios::boolalpha);  // ios::boolalpha is a bitmask

    // for cout object's default format state 
    // ios::boolalpha is cleared
    // when cout.setf(ios::boolalpha) is called
    // ios::boolalpha is set

    cout.unsetf(ios::boolalpha); 
    // ios::boolalpha is cleared

    // -------------------------------------------------------

    // cout.flags() have 2 overloads(get and set)

    cout.flags();   // get overload
    // returns ios_base::fmtflags object that holds 
    // the current format state of the cout object

    cout.flags( cout.flags() | ios::boolalpha );  // set overload
    cout.setf(ios::boolalpha);
    // Those 2 lines are equivalent.


    cout.flags( cout.flags() & ~ios::boolalpha ); // set overload
    cout.unsetf(ios::boolalpha);
    // Those 2 lines are equivalent.

    // -------------------------------------------------------
  }
*/

/*
  int main()
  {
    std::ostream os(std::cout.rdbuf());

    // cout object's format state is not changed

    os.setf(std::ios::boolalpha);

    // not all format states are on/off flags like boolalpha
    // i.e 2 parameter setf function
    os.setf(std::ios::left, std::ios::adjustfield); 
    os.setf(std::ios::fixed, std::ios::floatfield);
  }
*/

/*
  int main()
  {
    using namespace std;

    int ival{ 123123 };
    double dval{ 546.877 };

    // -------------------------------------------------------

    cout << ival << " " << dval << '\n';
    // when format state has been changed
    // chaining will be broken because of both ival and dval
    // will pass to standart output stream
    // with the same format state

    // -------------------------------------------------------

    cout.setf(ios::hex, ios::basefield);
    cout << ival << ' ';
    cout.setf(ios::uppercase);
    cout.setf(ios::scientific, ios::floatfield);
    cout << dval << '\n';

    // -------------------------------------------------------
  }
*/

/*
  // ostream manipulator
  std::ostream& Hex(std::ostream& os)
  {
    os.setf(std::ios::hex, std::ios::basefield);
    return os;
  }

  std::ostream& Boolalpha(std::ostream& os)
  {
    os.setf(std::ios::boolalpha);
    return os;
  }

  std::ostream& Endl(std::ostream& os)
  {
    os.put('\n');
    os.flush();
    return os;
  }

  std::ostream& dash_line(std::ostream& os)
  {
    os << "\n----------------------------------------\n";
    return os;
  }

  int main()
  {
    std::cout << 47802 << Hex << 47802 << '\n';
    // output -> 47802baba

    std::cout << "hello" << dash_line << "world" << '\n';
    // output ->
    //  hello
    //  ----------------------------------------
    //  world
  }
*/

/*
  #include <string>

  class Space {
  public:
    Space(std::size_t n) : m_n{ n } {}

    // hidden friend ostream manipulator
    friend std::ostream& operator<<(std::ostream& os, const Space& sp)
    {
      std::size_t space_count = sp.m_n;
      while (space_count--)
        os.put(' ');

      return os;
    }
  private:
    std::size_t m_n{};
  };

  int main()
  {
    using namespace std;

    int ival{ 12 };
    double dval{ 3.14 };
    string str{ "hello" };

    std::cout << ival << Space(3) 
              << dval << Space(3) 
              << str << '\n';
    // output -> 12   3.14   hello
  }
*/

/*
  #include <format>

  int main()
  {
    // std::format function is being used 
    // for compile time format string

    auto str = std::format("hello world");
    // "std::format("hello world")" expression's type is std::string

    std::cout << std::format("hello");
    operator<<(std::cout, std::format("hello"));
    // Those 2 lines are equivalent.
  }
*/

/*
  #include <format>
  #include <string>

  int main()
  {
    using namespace std;

    string city{ "Istanbul" };
    int plate_number{ 34 };

    cout << format("{} plate number is {}\n", city, plate_number);
    // output -> Istanbul plate number is 34
  }
*/

/*
  #include <format>
  #include <string>

  int main()
  {
    std::string city{ "Istanbul" };
    int plate_number{ 34 };

    std::cout << std::format("{1} - {0}\n", city, plate_number);
    // output -> 34 - Istanbul

    std::cout << std::format("{0} - {1}\n", city, plate_number);
    // output -> Istanbul - 34

    std::cout << std::format("{0} - {1} - {0}\n", city, plate_number);
    // output -> Istanbul - 34 - Istanbul

    std::cout << std::format("{1} - {0} - {1}\n", city, plate_number);
    // output -> 34 - Istanbul - 34
  }
*/

/*
  #include <cctype>

  int main()
  {
    for (int i = 0; i < 128; ++i){
      if (std::isprint(i))
        std::cout << std::format("{0:d} - {0:#X} - {0:c}\n", i);
    }

    // output ->
    //  32 - 0X20 -
    //  33 - 0X21 - !
    //  34 - 0X22 - "
    //  35 - 0X23 - #
    //  36 - 0X24 - $
    //  37 - 0X25 - %
    //  38 - 0X26 - &
    //  39 - 0X27 - '
    // ...
    //  122 - 0X7A - z
    //  123 - 0X7B - {
    //  124 - 0X7C - |
    //  125 - 0X7D - }
    //  126 - 0X7E - ~
  }
*/

/*
  [[fill]align] [sign][#][0][width][.precision][type]
*/

/*
  #include <format>

  int main()
  {
    using namespace std; 

    int x = 1273;

    cout << format("|{{{}}}|", x);  // output -> |{1273}|
  }
*/

/*
  // for integral types 
  // - default alignment is right
  // - defualt type is decimal

  #include <format>

  int main()
  {
    using namespace std; 

    int x = 1234;

    cout << format("|{:12}|\n", x);
    cout << format("|{:>12}|\n", x);  
    // output -> |        1234|
    // Those 2 lines are equivalent.

    cout << format("|{:<12}|\n", x);  
    // output -> |1234        |

    cout << format("|{:^12}|\n", x);  
    // output -> |    1234    |

  }
*/

/*
  #include <format>

  int main()
  {
    using namespace std; 

    int x = 1234;

    cout << format("|{:_>12}|\n", x);  
    // output -> |________1234|

    cout << format("|{:_<12}|\n", x);  
    // output -> |1234________|

    cout << format("|{:_^12}|\n", x);  
    // output -> |____1234____|


    cout << format("|{:.>12}|\n", x);  
    // output -> |........1234|

    cout << format("|{:.<12}|\n", x);  
    // output -> |1234........|

    cout << format("|{:.^12}|\n", x);  
    // output -> |....1234....|
  }
*/

/*
  // dynamic value can be used for width

  int main()
  {
    using namespace std;
    
    int ival = 1234;

    cout << "Enter a width: ";
    int runtime_width;
    cin >> runtime_width;

    cout << format("|{:_<{}}|\n", ival, runtime_width);

    // input -> Enter a width: 20
    // output -> |1234________________|

    // input -> Enter a width: 2
    // output -> |1234|


    cout << format("|{1:_<{0}}|\n", runtime_width, ival );
    // using positional indexes
  }
*/

/*
  //   [[fill]align] [sign][#][0][width][.precision][type]
  // signs -> ' ', '+', '-'

  int main()
  {
    int ival = 345;
    int n_ival = -345;

    // -------------------------------------------------------

    std::cout << std::format("|{:_>10}|\n", ival);    // no sign
    // output -> |_______345|

    std::cout << std::format("|{:_> 10}|\n", ival);   // sign ' ' 
    // output -> |______ 345|

    std::cout << std::format("|{:_>+10}|\n", ival);   // sign '+'
    // output -> |______+345|

    std::cout << std::format("|{:_>-10}|\n", ival);   // sign '-'
    // output -> |_______345|
    
    // -------------------------------------------------------

    std::cout << std::format("|{:_>10}|\n", n_ival);  // no sign
    // output -> |______-345|

    std::cout << std::format("|{:_> 10}|\n", n_ival); // sign ' '
    // output -> |______-345|

    std::cout << std::format("|{:_>+10}|\n", n_ival); // sign '+'
    // output -> |______-345|

    std::cout << std::format("|{:_>-10}|\n", n_ival); // sign '-'
    // output -> |_______-345|

    // -------------------------------------------------------
  }
*/

/*
  // [[fill]align] [sign][#][0][width][.precision][type]

  // padding 0

  #include <format>

  int main()
  {
    using namespace std;

    int ival = 255;
    int n_ival = -255;
    
    cout << format("|{:07d}|", ival) << '\n';     
    // output -> |0000255|

    cout << format("|{:+07d}|", ival) << '\n';    
    // output -> |+000255|

    cout << format("|{: 07d}|", n_ival) << '\n';  
    // output -> |-000255|

    cout << format("|{:-07d}|", n_ival) << '\n';  
    // output -> |-000255|

    cout << format("|{:07X}|", ival) << '\n';     
    // output -> |00000FF|

    cout << format("|{:#07X}|", ival) << '\n';    
    // output -> |0X000FF|
  }
*/

/*
  // <---- check format_specifiers.png ---->
*/

/*
  // general        -> fixed and scietific are cleared
  //  - can be fixed or scientific depends on the value
  // fixed          -> fixed set, scientific cleared
  // scientific     -> scientific set, fixed cleared
  // hexfloat       -> scientific set, fixed set (hexfloat set)  

  int main()
  {
    using namespace std;

    double dval_1 = 8273.128342;
    double dval_2 = 4547623093.128342;

    // ----------------------------------------------------

    cout << "| " << dval_1 << " |\n";
    // output -> | 8273.13 | 
    // (because of general(default) written as fixed)

    cout << "| " << dval_2 << " |\n";
    // output -> | 4.54762e+09 |
    // (because of general(default) written as scientific)

    // ----------------------------------------------------

    cout << hexfloat;

    cout << "| " << dval_1 << " |\n";
    // output -> | 0x1.028906d82ba5ap+13 |

    cout << "| " << dval_2 << " |\n";
    // output -> | 0x1.0f0f38b520dbp+32 |

    // ----------------------------------------------------
  }
*/

/*
  // check <---- format_type.png ---->

  #include <format>

  int main()
  {
    using namespace std;

    double dval = 8273.128342;

    cout << format("|{}|", dval) << '\n';     // general
    // output -> |8273.128342|
    cout << format("|{:g}|", dval) << '\n';   // general
    // output -> |8273.13|

    cout << format("|{:e}|", dval) << '\n';  // scientific(e)
    // output -> |8.273128e+03|
    cout << format("|{:E}|", dval) << '\n';  // scientific(E)
    // output -> |8.273128E+03|

    cout << format("|{:f}|", dval) << '\n';  // fixed
    // output -> |8273.128342|
    cout << format("|{:F}|", dval) << '\n';  // fixed
    // output -> |8273.128342|

    cout << format("|{:a}|", dval) << '\n';  // hexfloat(a)
    // output -> |1.028906d82ba5ap+13|
    cout << format("|{:A}|", dval) << '\n';  // hexfloat(A)
    // output -> |1.028906D82BA5AP+13|
  }
*/

/*
  // boolean integral types

  #include <format>

  int main()
  {
    using namespace std;

    cout << format("|{}|", true) << '\n';       // output -> |true|
    cout << format("|{}|", 10 > 56) << '\n';    // output -> |false|

    cout << format("|{:d}|", true) << '\n';     // output -> |1|
    cout << format("|{:d}|", 10 > 56) << '\n';  // output -> |0|

    cout << format("|{:#x}|", true) << '\n';    // output -> |0x1|
    cout << format("|{:#x}|", 10 > 56) << '\n'; // output -> |0x0|

    cout << format("|{:#b}|", true) << '\n';    // output -> |0b1|
    cout << format("|{:#B}|", 10 > 56) << '\n'; // output -> |0B0|
  }
*/

/*
  // pointer types

  #include <format>

  int main()
  {
    using namespace std;

    int x = 1234;

    // ---------------------------------------------------

    cout << format("|{}|", &x) << '\n'; // syntax error

    // ---------------------------------------------------

    cout << format("|{}|", (void*)&x) << '\n'; 
    // output -> |0xc20a5ff82c|
    cout << format("|{}|", static_cast<void*>(&x)) << '\n'; 
    // output -> |0xc20a5ff82c|
    cout << format("|{:p}|", (void*)&x) << '\n';       
    // output -> |0xc20a5ff82c|
    cout << format("|{:p}|", nullptr) << '\n';  
    // output -> |0x0|

    // ---------------------------------------------------  
  }
*/

/*
  int main()
  {
    int x = 65;
    
    std::cout << std::format("|{}|\n", x);      // output -> |65|
    std::cout << std::format("|{:c}|\n", x);    // output -> |A|
    std::cout << std::format("|{:d}|\n", x);    // output -> |65|
    std::cout << std::format("|{:x}|\n", x);    // output -> |41|
    std::cout << std::format("|{:#x}|\n", x);   // output -> |0x41|
    std::cout << std::format("|{:X}|\n", x);    // output -> |41|
    std::cout << std::format("|{:#X}|\n", x);   // output -> |0X41|
    std::cout << std::format("|{:o}|\n", x);    // output -> |101|
    std::cout << std::format("|{:#o}|\n", x);   // output -> |0101|
  }
*/

/*
  #include <format>
  #include <string>

  int main()
  {
    using namespace std;

    string city{ "Istanbul" };

    // -----------------------------------------------------

    cout << format("|{}|\n", city);   
    // output -> |Istanbul|

    // -----------------------------------------------------

    cout << format("|{:24}|\n", city);   
    // output -> |Istanbul                |
    // default alignment is left in std::string

    // -----------------------------------------------------

    cout << format("|{:_>24}|\n", city);   
    // output -> |________________Istanbul|

    // -----------------------------------------------------
  }
*/

/*
  // [[fill]align] [sign][#][0][width][.precision][type]

  #include <format>
  #include <string>

  int main()
  {
    using namespace std;

    string city{ "Istanbul" };

    cout << format("|{:_>24.3}|\n", city);
    // output -> |_____________________Ist|

    // [fill(_)] - [align(>)] - [width(24)] - [precision(.3)] 
    // is being used explicitly
  }
*/

/*
  // [[fill]align] [sign][#][0][width][.precision][type]

  #include <format>     // std::format_to
  #include <iterator>

  int main()
  {
    int ival = 2'454'234;

    std::format_to( std::ostream_iterator<char>{ std::cout },
                    "|{:^#16X}|\n", ival);
    // output -> |    0X2572DA    |


    // [align(^)] - [#] - [width(16)] - [type(X)]
  }
*/

/*
  // [[fill]align] [sign][#][0][width][.precision][type]

  #include <format>     // std::format_to
  #include <iterator>

  int main()
  {
    int ival = 2'454'234;

    std::string str;

    std::format_to( std::back_inserter(str),
                    "|{:^#16X}|", ival);

    std::cout << "str.length() = " << str.length() << '\n';
    // str.length() = 18
    std::cout << str << '\n';
    // output -> |    0X2572DA    |


    // [align(^)] - [#] - [width(16)] - [type(X)]
  }
*/

/*
  #include <string>
  #include <list>
  #include <format>   // std::format_to
  #include <iterator> // std::back_inserter, std::front_inserter

  int main()
  {
    using namespace std;

    string str{"Hello World"};
    int ival = 28373;
    double dval = 5463.1238;

    // ------------------------------------------------------

    string str_2;
    format_to(back_inserter(str_2), "|{} {} {:.2f}|", ival, str, dval);

    std::cout << str_2 << '\n';
    // output -> |28373 Hello World 5463.12|

    // ------------------------------------------------------

    list<char> clist;
    format_to(front_inserter(clist), "|{} {} {:.2f}|", ival, str, dval);

    for (auto c : clist)
      cout << c;
    // output -> |21.3645 dlroW olleH 37382|

    // ------------------------------------------------------
  }
*/

/*
  #include <string>
  #include <format>     // std::format_to
  #include <iterator>   // std::back_inserter

  int main()
  {
    using namespace std;

    string str;

    for (char c = 'A'; c <= 'Z'; ++c){
      format_to(back_inserter(str), "{{ {0}, {0:d} }}\n", c);
    }

    cout << str;
    // output ->
    //  { A, 65 }
    //  { B, 66 }
    //  { C, 67 }
    //  { D, 68 }
    //  { E, 69 }
    //  { F, 70 }
    //  { G, 71 }
    //  { H, 72 }
    //  { I, 73 }
    //  { J, 74 }
    //  { K, 75 }
    //  { L, 76 }
    //  { M, 77 }
    //  { N, 78 }
    //  { O, 79 }
    //  { P, 80 }
    //  { Q, 81 }
    //  { R, 82 }
    //  { S, 83 }
    //  { T, 84 }
    //  { U, 85 }
    //  { V, 86 }
    //  { W, 87 }
    //  { X, 88 }
    //  { Y, 89 }
    //  { Z, 90 }
  }
*/

/*
  #include <format>

  int main()
  {
    using namespace std;

    int ival = 1234;

    cout << format("{:#X}\n", ival);  // output -> 0X4D2

    auto len = formatted_size("{:#x}", ival);
    cout << "len = " << len << '\n';  // output -> len = 5

    // if a buffer needed for string 
    // formatted_size function can be used.
  }
*/

/*
  #include <array>
  #include <format>
  #include <string>

  int main()
  {
    using namespace std;

    array<char, 10> arr{};  // value initialized

    string str_1{"12345"};
    string str_2{"67890"};

    format_to_n(arr.data(), arr.size() - 1, "|{}{}|\n", str_1, str_2);

    for (char c : arr)
      cout << c;
    // output -> |12345678   
    // (arr.size() - 1 = 9) characters
  }
*/

/*
  #include <array>
  #include <format>
  #include <string>

  int main()
  {
    using namespace std;

    array<char, 40> arr{};

    string str_1{"hello"};
    string str_2{"world"};

    auto x = format_to_n( arr.data(), 
                          arr.size() - 1, 
                          "{} {}", str_1, str_2);

    cout << "x.size = " << x.size << '\n';  
    // output -> x.size = 11
    // "hello"(5) + ' '(1) + "world"(5) --> 11

    cout << "index = " << x.out - arr.data() << '\n';
    // output -> index = 11
    // p_last - p_beg = length of str
  } 
*/

/*
  #include <format>

  class Always_40 {
  public:
    int get_value() const { return 40; }
  };

  template <>
  struct std::formatter<Always_40> {
    constexpr auto parse(std::format_parse_context& ctx) {
      return ctx.begin(); // return the position of '}' character
    }

    auto format(const Always_40& obj, std::format_context& ctx) const {
      return std::format_to(ctx.out(), "{}", obj.get_value());
    }
  };

  // {}   --> ctx.begin() will return '}' character
  // {:b} --> ctx.begin() will return 'b' character

  int main()
  {
    std::cout << std::format("|{}|\n", Always_40{});  
    // output -> |40|

    std::cout << std::format("|{0} {0} {0}|\n", Always_40{});
    // output -> |40 40 40|
  }
*/

/*
  #include <format>
  #include <string>
  #include <utility>  // std::move

  class Person {
  public:
    Person(std::string name, int id) 
      : m_name{ std::move(name) }, m_id{ id } {}

    std::string get_name() const { return m_name; }
    int get_id() const { return m_id; }
  private:
    std::string m_name;
    int m_id;
  };

  template <>   // explicit(full) specialization
  class std::formatter<Person> {
  public:
    constexpr auto parse(std::format_parse_context& ctx)
    {
      auto iter{ ctx.begin() };
      const auto iter_end{ ctx.end() };

      if (iter == iter_end || *iter == '}')   // "{}" 
      {
        m_ftype = FormatType::All;
        return iter;
      }

      switch(*iter){
      case 'n': m_ftype = FormatType::Name; break;
      case 'i': m_ftype = FormatType::Id; break;
      case 'a': m_ftype = FormatType::All; break;
      default: throw std::format_error("Invalid format specifier");
      }

      ++iter;
      if (iter != iter_end && *iter != '}') // "{}n"
        throw std::format_error("Invalid format specifier");

      return iter;
    }

    // abbreviated template syntax (C++20)
    auto format(const Person& per, std::format_context& ctx) const
    {
      using enum FormatType;

      switch(m_ftype){
      case Name:
        return std::format_to(ctx.out(), "{}", per.get_name());
      case Id:
        return std::format_to(ctx.out(), "{}", per.get_id());
      case All:
        return std::format_to(ctx.out(), "[{} - {}]", 
                              per.get_id(), 
                              per.get_name());
      default:
        throw std::format_error("Invalid format type");
      }
    }
  private:
    enum class FormatType { Name, Id, All };
    FormatType m_ftype;
  };

  // 'n' format specifier ===> name
  // 'i' format specifier ===> id
  // 'a' format specifier ===> [name - id]

  int main()
  {
    using namespace std;

    Person p1{ "World", 123 };
    Person p2{ "Galaxy", 345 };
    Person p3{ "Universe", 678 };

    cout << format("{:n}\n{:i}\n{:a}", p1, p2, p3) << '\n';

    // output ->
    //  World
    //  345
    // [678 - Universe]
  }
*/

/*
  #include <format>

  struct Point{
    int m_x, m_y;
  };

  template <>
  class std::formatter<Point> {
  public:
    constexpr auto parse(std::format_parse_context& ctx)
    {
      auto iter{ ctx.begin() };

      for(; iter != ctx.end() && *iter != '}'; ++iter)
      {
        if (*iter == '#')
          cb_ = true;
        else
          throw std::format_error("Invalid format character");
      }
      return iter;
    }

    auto format(const Point& p, std::format_context& ctx) const
    {
      return std::format_to(ctx.out(), "{}{} - {}{}", 
                            (cb_ ? '{' : '('),
                            p.m_x, 
                            p.m_y,
                            (cb_ ? '}' : ')'));
    }
  private:
    bool cb_{};
  };

  int main()
  {
    using namespace std;

    Point p1{ 1, 4 }, p2{ 3, 5 };
    cout << format("{:#} {}", p1, p2) << '\n';
    // output -> {1 - 4} (3 - 5)

    Point p3{ 11, 22 };
    cout << format("{0:#} - {0}", p3) << '\n';
    // output -> {11 - 22} - (11 - 22)
  }
*/

/*
  #include <format>

  class Mint {
  public:
    Mint() = default;
    explicit Mint(int x) : m_x{ x } {}
    int get() const { return m_x; }

  private:
    int m_x{};
  };

  template <>
  class std::formatter<Mint> {
  public:
    // when no formatting is provided "{}"
    constexpr auto parse(std::format_parse_context& ctx)
    {
      auto iter = ctx.begin();
      return iter;
      // "{}"     --> ctx.begin will return '}' character's position
      // "{}"     --> ctx.end will return '}' character's position
    }

    auto format(const Mint& m, std::format_context& ctx) const
    {
      return std::format_to(ctx.out(), "{}", m.get());
    }
  };

  int main()
  {
    Mint m1{ 123 };

    std::cout << std::format("|{}|", m1); // output -> |123|
  }
*/

/*
  #include <format>

  class Mint {
  public:
    Mint() = default;
    explicit Mint(int x) : m_x{ x } {}
    int get() const { return m_x; }

  private:
    int m_x{};
  };

  template <>
  class std::formatter<Mint> {
  public:
    // when formatting is provided "{:12}"
    constexpr auto parse(std::format_parse_context& ctx)
    {
      auto iter = ctx.begin();

      while (iter != ctx.end() && *iter != '}')
      {
        if(*iter < '0' || *iter > '9')
          throw std::format_error("Invalid width character");

        m_width = m_width * 10 + *iter - '0';
        ++iter;
      }

      return iter;

      // "{:12}"  --> ctx.begin will return '1' character's position
    }

    auto format(const Mint& m, std::format_context& ctx) const
    {
      return std::format_to(ctx.out(), "{:{}}", m.get(), m_width);
    }

  private:
    int m_width{};
  };

  int main()
  {
    Mint m1{ 123 };

    std::cout << std::format("|{:15}|", m1); 
    // output -> |            123|
  }
*/

/*
  // -------------------- FIRST WAY --------------------

  #include <format>

  class Mint {
  public:
    Mint() = default;
    explicit Mint(int x) : m_x{ x } {}
    int get() const { return m_x; }

  private:
    int m_x{};
  };

  template <>
  class std::formatter<Mint> {
  public:
    // when formatting is provided "{:12}"
    constexpr auto parse(std::format_parse_context& ctx)
    {
      return m_int_f.parse(ctx);
    }

    auto format(const Mint& m, std::format_context& ctx) const
    {
      return m_int_f.format(m.get(), ctx);
    }
  private:
    std::formatter<int> m_int_f;
    // using std::formatter's int specialization to
    // format Mint objects
  };

  // [[fill]align] [sign][#][0][width][.precision][type]

  int main()
  {
    Mint m1{ 123 };

    std::cout << std::format("|{0:_>15}| |{0:_<15}|\n", m1); 
    // output -> |____________123| |123____________|
  }
*/

/*
  // -------------------- SECOND WAY --------------------

  #include <format>

  class Mint {
  public:
    Mint() = default;
    explicit Mint(int x) : m_x{ x } {}
    int get() const { return m_x; }

  private:
    int m_x{};
  };

  // std::formatter<Mint> is public inherited from std::formatter<int>
  template <>
  struct std::formatter<Mint> : std::formatter<int> {
  public:
    // parse member function is not need to be written
    auto format(const Mint& m, std::format_context& ctx) const
    {
      return std::formatter<int>::format(m.get(), ctx);
    }
  };

  // [[fill]align] [sign][#][0][width][.precision][type]

  int main()
  {
    Mint m1{ 345 };

    std::cout << std::format("|{0:_>15}| |{0:_<15}|\n", m1); 
    // output -> |____________345| |345____________|
  }
*/

/*
  #include <string>
  #include <format>

  enum class Fruit { Apple, Orange, Pear };

  template <>
  class std::formatter<Fruit> : public std::formatter<std::string> {
  public:
    auto format(const Fruit& fruit, std::format_context& ctx) const
    {
      std::string str;

      switch(fruit)
      {
        using enum Fruit;

        case Apple:   str  = "Apple"; break;
        case Orange:  str  = "Orange"; break;
        case Pear:    str  = "Pear"; break;
      }
      return std::formatter<std::string>::format(str, ctx);
    }
  };

  int main() 
  {
    Fruit f1 = Fruit::Apple;
    Fruit f2 = Fruit::Orange;

    std::cout << std::format("|{:_<16}|\n", f1);
    // output -> |Apple___________|

    std::cout << std::format("|{:.>24}|\n", f2);
    // output -> |..................Orange|
  }
*/

/*
  #include <format>

  int main()
  {
    using namespace std;

    const char* p = "|{:<12}|";
    int x = 234;

    cout << format(p, x);   // syntax error
    //  error: the value of 'p' is not usable in a constant expression
  }
*/

/*
  // runtime formatting

  #include <format>

  int main()
  {
    char str_arr[20] = "";

    std::cout << "write format line : ";
    std::cin >> str_arr;  

    int x = 465;

    auto s = std::vformat(str_arr, std::make_format_args(x));
    std::cout << s << '\n';

    // input -> write format line : |{:_>20}|
    // output -> |_________________465|
  }
*/