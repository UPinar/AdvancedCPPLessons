# AdvancedCPPLessons

### [Value Categories](https://github.com/UPinar/AdvancedCPPLessons/tree/main/value_categories)
  - `decltype` specifier
  - `std::move` <algorithm> <utility>
  - universal(forwarding) reference
  - reference collapsing
  - special member functions
  - `noexcept` specifier |  `noexcept` operator

### [Copy Elision](https://github.com/UPinar/AdvancedCPPLessons/tree/main/copy_elision)
  - Mandatory Copy Elision
    - Unmaterialize object passing
    - (Unnamed) Return Value Optimization (RVO) 
  - Named Return Value Optimization (NRVO) [Optimization - Copy Elision]
  - No NRVO Scenarios

### [Move Semantics](https://github.com/UPinar/AdvancedCPPLessons/tree/main/move_semantics)
  - Rule of Zero
  - Bad Scenarios
  - `noexcept`
  - `override` contextual keyword(bağlamsal anahtar sözcük)
  - move sematics and `noexcept` specifier relationship
  - moved from state

### [Perfect Forwarding](https://github.com/UPinar/AdvancedCPPLessons/tree/main/perfect_forwarding)
  - universal(forwarding) reference
  - perfect forwarding
    - `auto&&`
    - `decltype(auto)`
  - ref qualifier
  
### [Move Semantics and Perfect Forwarding in STL](https://github.com/UPinar/AdvancedCPPLessons/tree/main/move_semantic_perfect_forwarding_in_standart_library)
  - move-only types
  - `std::initializer_list` class template
  - `std::remove` && `std::remove_if` algorithms
  - `std::move` && `std::move_backward` algorithms
  - `std::move_iterator` iterator adaptor
  
### [Literal Operator Functions](https://github.com/UPinar/AdvancedCPPLessons/tree/main/literal_operator_functions)
  - cooked literal operator functions
  - uncooked literal operator functions
  - raw string literal
  - `std::quoted`

### [`std::string_view` (C++17)](https://github.com/UPinar/AdvancedCPPLessons/tree/main/string_view)


### [Vocabulary Types (C++17)](https://github.com/UPinar/AdvancedCPPLessons/tree/main/vocabulary_types)
  - `std::optional` class template
  - `std::variant` variadic class template
    - `std::monostate` class
    - `std::visit` variadic function templace
    - overloader idiom
  - `std::any` class
    - `std::any_cast` function template