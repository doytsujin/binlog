//[variant
#include <binlog/adapt_stdvariant.hpp> // must be included to log std::variant types, requires C++17
//]

#include <binlog/binlog.hpp>

#include <iostream>
#include <stdexcept>
#include <variant>

struct Bad
{
  Bad() = default;
  ~Bad() = default;

  Bad(const Bad&) { throw std::runtime_error("copy ctor"); } // NOLINT
  Bad(Bad&&) { throw std::runtime_error("move ctor"); } // NOLINT

  void operator=(const Bad&) { throw std::runtime_error("copy op="); } // NOLINT
  void operator=(Bad&&) { throw std::runtime_error("move op="); } // NOLINT
};

BINLOG_ADAPT_STRUCT(Bad)

int main()
{
  // valueless_by_exception
  std::variant<int, Bad> valueless{0};
  try
  {
    valueless = Bad{};
  }
  catch (const std::runtime_error&)
  {
    BINLOG_INFO("valueless_by_exception: {}", valueless);
    // Outputs: valueless_by_exception: {null}
  }
  catch (const std::bad_variant_access&)
  {
    // avoid clang-tidy "an exception may be thrown in function 'main'"
    BINLOG_ERROR("Unexpected bad_variant_access was thrown");
  }

  //[variant

  std::variant<int, std::string, float> v{"works"};
  std::variant<std::monostate> mono;
  BINLOG_INFO("std::variant: {}, monostate: {}, valueless_by_exception: {}", v, mono, valueless);
  // Outputs: std::variant: works, monostate: {null}, valueless_by_exception: {null}
  //]

  // nested
  std::variant<std::pair<int, int>, std::variant<std::string, bool>> nested{true};
  BINLOG_INFO("nested: {}", nested);
  // Outputs: nested: true

  // TODO(benedek) this works, but the log statement takes ages to compile
  // and 5 time as long to run clang-tidy -- needs further investigation.
  //// limits
  //const std::variant<
  //  int, int, int, int, int, int, int, int, int, int, // 10
  //  int, int, int, int, int, int, int, int, int, int, // 20
  //  int, int, int, int, int, int, int, int, int, int, // 30
  //  int, int, int, int, int, int, int, int, int, int, // 40
  //  int, int, int, int, int, int, int, int, int, int, // 50
  //  int, int, int, int, int, int, int, int, int, int, // 60
  //  int, int, int, int, int, int, int, int, int, int, // 70
  //  int, int, int, int, int, int, int, int, int, int, // 80
  //  int, int, int, int, int, int, int, int, int, int, // 90
  //  int, int, int, int, int, int, int, int, int, int, // 100
  //  int, int, int, int, int, int, int, int, int, int, // 110
  //  int, int, int, int, int, int, int, int, int, int, // 120
  //  int, int, int, int, int, int, int, int, int, int, // 130
  //  int, int, int, int, int, int, int, int, int, int, // 140
  //  int, int, int, int, int, int, int, int, int, int, // 150
  //  int, int, int, int, int, int, int, int, int, int, // 160
  //  int, int, int, int, int, int, int, int, int, int, // 170
  //  int, int, int, int, int, int, int, int, int, int, // 180
  //  int, int, int, int, int, int, int, int, int, int, // 190
  //  int, int, int, int, int, int, int, int, int, int, // 200
  //  int, int, int, int, int, int, int, int, int, int, // 210
  //  int, int, int, int, int, int, int, int, int, int, // 220
  //  int, int, int, int, int, int, int, int, int, int, // 230
  //  int, int, int, int, int, int, int, int, int, int, // 240
  //  int, int, int, int, int, int, int, int, int, int, // 250
  //  int, int, int, int, int                           // 255
  //> large(std::in_place_index<254>, 254);
  //BINLOG_INFO("large: {}", large);
  //// Outputs: large: 254

  binlog::consume(std::cout);
}
