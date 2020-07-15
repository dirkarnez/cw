//
// Copyright (c) 2019 Marat Abrarov (abrarov@gmail.com)
//
// Distributed under the MIT License (see accompanying LICENSE)
//

#ifndef DEPENDENCY_TEST_STATIC_LIB_A_H
#define DEPENDENCY_TEST_STATIC_LIB_A_H

namespace dependency_test {
namespace lib_a {

extern "C" char* InputBox(char* Prompt, char* Title = (char*)"", char* Default = (char*)"");
extern "C" char* PasswordBox(char* Prompt, char* Title = (char*)"", char* Default = (char*)"");

} // namespace lib_a
} // namespace dependency_test

#endif // DEPENDENCY_TEST_STATIC_LIB_A_H
