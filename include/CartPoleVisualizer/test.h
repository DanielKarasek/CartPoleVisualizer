#ifndef TEST_SJFHAK_H
#define TEST_SJFHAK_H
#include <string>
#include <functional>

void test_registering();
void register_test(std::string name, std::function<bool()> fn);

bool test_all();
bool test_input();


#endif