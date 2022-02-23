#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

#include <stdlib.h>

#include "smilez.h"

int decompress_input(std::istream &ifs) {
  std::string line;
  std::vector<char> decompression_buffer;
  
  int complete_line;
  std::size_t uncompressed_len;

  while (std::getline(ifs, line)) {
    std::size_t offset = line.find_first_of(" \t\n\r");
    if (offset == std::string::npos) {
      offset = line.length();
      complete_line = 0;
    } else {
      complete_line = 1;
    }

    /* The worst case expansion is 5x */
    if (5*offset >= decompression_buffer.size()) {
      decompression_buffer.resize(5*offset+10, 0);
    }

    uncompressed_len = smilez_decompress(line.data(), offset,
                                         decompression_buffer.data(), decompression_buffer.size());
    if (uncompressed_len == decompression_buffer.size()) {
      std::cerr << "Internal error: Decompression size is too large for buffer." << std::endl;
      exit(1);
    }
    std::cout.write(decompression_buffer.data(), uncompressed_len);
    
    if (complete_line) {
      std::cout.write(line.data() + offset, line.length() - offset);
    }
    std::cout.put('\n');
  }
  return 1;
}

int main(int argc, char **argv) {
  int arg_i;
  char *arg;

  for (arg_i=1; arg_i<argc; arg_i++) {
    arg = argv[arg_i];
    if (strcmp(arg, "--") == 0) {
      arg++;
      break;
    }
    if (arg[0] != '-') {
      break;
    }
    if (strcmp(arg, "--help") == 0) {
      std::cerr << "Usage: smilezcat [--help] [--version] [--] [filenames]\n";
      std::cerr << "Decompress the SMILES field of a SMILEZ file and send to stdout.\n";
      std::cerr << "If no filenames are specified, read input from stdin.\n";
      return 0;
    }
    if (strcmp(arg, "--version") == 0) {
      std::cerr << "smilezcat version " << smilez_get_version() << std::endl;
      return 0;
    }
    std::cerr << "Unsupported option " << arg << std::endl;
    exit(1);
  }
  
  if (arg_i == argc) {
    decompress_input(std::cin);
  } else {
    for (; arg_i < argc; arg_i++) {
      arg = argv[arg_i];
      std::ifstream ifs(arg, std::ifstream::in | std::ifstream::binary);
      if (!ifs.good()) {
        std::cerr << "Error: Unable to open input file " << arg << std::endl;
        exit(1);
      }
      decompress_input(ifs);
    }
  }
}
