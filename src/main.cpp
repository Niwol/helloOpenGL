#include <iostream>

#include "Application.hpp"

int main(int, char**) {

  std::cout << "Hello OpenGL!" << std::endl;

  Application app;

  if(app.construct(800, 600))
    app.run();

  return EXIT_SUCCESS;
}

