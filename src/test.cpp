#include "test.h"
#include "auxilaries.h"
#include <iostream>
#include <string>
#include <sstream>
#include <streambuf>
#include <functional>
#include <vector>

struct TestPair
{
  std::string text;
  std::function<bool()> fn;
};

static std::vector<TestPair> test_functions;

class Cout2String
{
  public: 
    Cout2String(std::streambuf *new_buf):
    old_buf{std::cout.rdbuf(new_buf)}
    {}
    ~Cout2String()
    {
      std::cout.rdbuf(old_buf);
    }
  private:
    std::streambuf *old_buf;
};


class Cin2String
{
  public: 
    Cin2String(std::streambuf *new_buf):
    old_buf{std::cin.rdbuf(new_buf)}
    {}
    ~Cin2String()
    {
      std::cin.rdbuf(old_buf);
    }
  private:
    std::streambuf *old_buf;
};

void register_test(std::string name, std::function<bool()> fn)
{
  test_functions.push_back({name, fn});
}

void test_registering()
{
  register_test("Conditional input integer test ", test_input);
}

bool test_all()
{ 
  bool all_passed{true};
  int total{0};
  int passed_total{0};
  for (const auto &pair: test_functions)
  {
    ++total;
    bool this_passed = pair.fn();
    
    if (this_passed) ++passed_total;
    else all_passed = false;
    
    std::string passed = this_passed ? "\033[1;32mpassed\033[0m" : "\033[1;31mfailed\033[0m"; 
    std::cout << "Test " << pair.text << " " << passed << '\n';
  }
  std::cout << "Tests passed \033[1m" << passed_total << "/" << total << "\033[0m\n";
  return all_passed;

}


bool test_string_stream()
{
  std::string res;
  {
    std::stringstream st;
    Cout2String tmp{st.rdbuf()};
    std::cout << "ahoj" << std::flush;
    res = st.str();  
  }
  std::cout << res;
  {
    std::stringstream st("Ahoj2");
    Cin2String tmp{st.rdbuf()};
    std::cin >> res;
  }
  std::cout << res;
  
  return true;
}

bool test_input()
{
  std::stringstream in("Ahoj2\n-7\n4\n");
  std::stringstream out;
  
  Cin2String tmp_in{in.rdbuf()};
  Cout2String tmp_out{out.rdbuf()};
  
  get_integral_value<int>("Test get >0: ", [](int a){return a>0;});

  std::string res = out.str();
  return !res.compare("Test get >0: Input failed due to invalid value\nTest get >0: "
                      "Input failed due to value not passing condition\nTest get >0: ");
}