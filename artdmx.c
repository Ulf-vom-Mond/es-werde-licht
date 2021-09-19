#include <stdio.h>

void print_help(){
  printf(
    "Usage: artdmx [OPTION]... [DATA]...\n"
    "Send the specified DMX512 data to the specified host via the Art-Net protocol.\n\n"

    "OPTIONS:\n"
    "    -h, --host           ip address of receiver\n"
    "    -p, --port           port of receiver (6454)\n"
    "    -s, --sequence       sequence integer to be transmitted (0)\n"
    "    -P, --physical       physical input port from which DMX512 data was input (0)\n"
    "    -u, --universe       destination universe\n\n"

    "DATA:\n"
    "The data should be given in a key:value format where the key is the node id (integer 0 to 511) and the value is an integer between 0 and 255.\n\n"

    "\n"
  );
}

int main(int argc, char const *argv[]) {
  for (size_t i = 0; i < argc; i++) {
    printf("%s\n", argv[i]);
  }
  print_help();
  return 0;
}
