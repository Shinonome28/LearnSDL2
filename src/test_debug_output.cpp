#include "common_utils.h"

int main(int argc, char** argv) {
  debug() << "test "
          << "hello,world!" << DebugOutputStream::End();
  return 0;
}