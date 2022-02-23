#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

#include <stdlib.h>

#include "smilez.h"

int compress_input(std::istream &ifs) {
  std::string line;
  std::vector<char> compression_buffer;
  
  int complete_line;
  std::size_t compressed_len;


  while (std::getline(ifs, line)) {
    std::size_t offset = line.find_first_of(" \t\n\r");
    if (offset == std::string::npos) {
      offset = line.length();
      complete_line = 0;
    } else {
      complete_line = 1;
    }

    /* The worst case expansion is less than 2x */
    if (2*offset >= compression_buffer.size()) {
      compression_buffer.resize(2*offset+10, 0);
    }

    compressed_len = smilez_compress(line.data(), offset,
                                     compression_buffer.data(), compression_buffer.size(),
                                     SMILEZ_WHITESPACE_DICTIONARY);
    if (compressed_len == compression_buffer.size()) {
      std::cerr << "Internal error: Compression size is too large for buffer." << std::endl;
      exit(1);
    }
    std::cout.write(compression_buffer.data(), compressed_len);
    
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
      std::cerr << "Usage: smilez [--help] [--version] [--] [filenames]\n";
      std::cerr << "Compress the SMILES field of a SMILES file and send to stdout.\n";
      std::cerr << "If no filenames are specified, read input from stdin.\n";
      return 0;
    }
    if (strcmp(arg, "--version") == 0) {
      std::cerr << "smilezip version " << smilez_get_version() << std::endl;
      return 0;
    }
    std::cerr << "Unsupported option " << arg << std::endl;
    exit(1);
  }
  
  if (arg_i == argc) {
    compress_input(std::cin);
  } else {
    for (; arg_i < argc; arg_i++) {
      arg = argv[arg_i];
      std::ifstream ifs(arg, std::ifstream::in | std::ifstream::binary);
      if (!ifs.good()) {
        std::cerr << "Error: Unable to open input file " << arg << std::endl;
        exit(1);
      }
      compress_input(ifs);
    }
  }
}
