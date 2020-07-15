//
// Copyright (c) 2019 Marat Abrarov (abrarov@gmail.com)
//
// Distributed under the MIT License (see accompanying LICENSE)
//

#include <iostream>
#include "foo_bar.h"

void dependency_test::app::foo()
{
  std::cout << "dependency_test::app::foo" << std::endl;
}

void dependency_test::app::bar()
{
  std::cout << "dependency_test::app::bar" << std::endl;
}
