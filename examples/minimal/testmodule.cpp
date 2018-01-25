#include "testmodule.hpp"

void print_keys(pammap::PamMap p) {
  for (auto& kv : p) {
    std::cout << kv.key() << std::endl;
  }
}
