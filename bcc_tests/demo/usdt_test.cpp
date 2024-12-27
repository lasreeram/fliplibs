#include <sys/sdt.h>

int main() {
  DTRACE_PROBE("hello_usdt", "enter");
  int reval = 0;
  DTRACE_PROBE1("hello_usdt", "exit", reval);
}

