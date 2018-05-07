// *************************************************************************
//  This file is part of the esmp test project by Steaphan Greene
//
//  Copyright 2018 Steaphan Greene <steaphan@gmail.com>
//
//  esmp is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  esmp is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with esmp (see the file named "LICENSE");
//  If not, see <http://www.gnu.org/licenses/>.
//
// *************************************************************************

#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "eslmp_tags.hpp"

int verbose = 0;
bool no_symbols = true;

uint32_t next_size = 0;
uint32_t max_containers = 0;

constexpr bool isnumcap(int ch) {
  return (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9');
}

size_t pslen(const char* cur) {
  size_t ret = 0;
  while (std::isprint(*cur)) {
    ++ret;
    ++cur;
  }
  return ret;
}

void print_tag(uint8_t* tag, const char* prefix = "") {
  if (no_symbols && tag[0] == '@' && tag[1] == 'I' && tag[2] == 'A' && tag[3] == 'D') {
    // Replace "@" with "Z", when enabled
    printf("%s%c%c%c%c:", prefix, 'Z', tag[1], tag[2], tag[3]);
  } else if (no_symbols && tag[0] == 0 && tag[1] == 'I' && tag[2] == 'A' && tag[3] == 'D') {
    // Replace control character with "z", instead of "`", when enabled
    printf("%s%c%c%c%c:", prefix, 'z', tag[1], tag[2], tag[3]);
  } else if (no_symbols && tag[0] > '9' && tag[1] == '0' && tag[2] == 'T' && tag[3] == 'X') {
    // Replace numbers-and-symbols with lowercase hex-plus when enabled
    printf("%s%c%c%c%c:", prefix, tag[0] + 'a' - '0' - 10, tag[1], tag[2], tag[3]);
  } else if (tag[0] < 32) {
    // Always replace control characters with lowercase letters (and "`")
    printf("%s%c%c%c%c:", prefix, tag[0] + 'a' - 1, tag[1], tag[2], tag[3]);
  } else {
    printf("%s%c%c%c%c:", prefix, tag[0], tag[1], tag[2], tag[3]);
  }
}

uint32_t parse(FILE* input) {
  uint32_t num_containers = 0;
  uint8_t tag[4];
  fread(tag, 4, 1, input);
  auto token = tokenize(tag);
  tag_type props = get_tag_props(token);
  if (!props.is_known) {
    if (isnumcap(tag[0]) && isnumcap(tag[1]) && isnumcap(tag[2]) && isnumcap(tag[3])) {
      printf("\n");
      fflush(stdout);
      fprintf(
          stderr,
          "WARNING: Reading unknown token '%c%c%c%c' at %lx\n",
          tag[0],
          tag[1],
          tag[2],
          tag[3],
          ftell(input) - 4);
    } else {
      printf("\n");
      fflush(stdout);
      fprintf(
          stderr,
          "ERROR: Invalid token '%c%c%c%c' at %lx\n",
          tag[0],
          tag[1],
          tag[2],
          tag[3],
          ftell(input) - 4);
      std::exit(1);
    }
  }

  if (!props.is_container) {
    uint32_t offset = 0;
    uint8_t lbyte, hbyte;
    fread(&lbyte, 1, 1, input);
    fread(&hbyte, 1, 1, input);
    offset |= hbyte;
    offset <<= 8;
    offset |= lbyte;

    // Srsly?
    if (offset == 0 && next_size > 0) {
      offset = next_size;
      next_size = 0;
    }

    uint64_t pos = ftell(input);
    if (verbose > 1) {
      fprintf(
          stderr,
          "[%.4lx] %c%c%c%c [%.4lx]\n",
          pos - 6,
          tag[0],
          tag[1],
          tag[2],
          tag[3],
          offset + pos);
    }

    if (token != tokenize("XXXX")) {
      print_tag(tag, "  ");
    }
    uint8_t buffer[offset + 1];
    fread(buffer, 1, offset, input);
    buffer[offset] = 0; // Just in case.
    if (offset <= 0) {
      printf("\n");
    } else if (
        props.is_string || (offset != 4 && offset != 2 && buffer[offset - 1] == 0 &&
                            pslen(reinterpret_cast<char*>(buffer)) == (offset - 1))) {
      printf(" :%s\n", reinterpret_cast<char*>(buffer));
    } else if (props.is_metadata) {
      for (uint32_t base = 0; base < offset; base += 4) {
        uint32_t val = 0;
        val |= buffer[base + 3];
        val <<= 8;
        val |= buffer[base + 2];
        val <<= 8;
        val |= buffer[base + 1];
        val <<= 8;
        val |= buffer[base + 0];
        if (token != tokenize("XXXX")) {
          printf(
              " x%.2X x%.2X x%.2X x%.2X",
              buffer[base + 0],
              buffer[base + 1],
              buffer[base + 2],
              buffer[base + 3]);
        }
        if (base == 4 && token == tokenize("HEDR")) {
          max_containers = val;
        } else if (base == 0 && token == tokenize("XXXX")) {
          next_size = val;
        }
      }
      if (token != tokenize("XXXX")) {
        printf("\n");
      }
    } else {
      for (uint32_t ctr = 0; ctr < offset; ++ctr) {
        if (ctr != 0 && ctr % 23 == 0) {
          printf("\n       ");
        }
        printf(" x%.2X", buffer[ctr]);
      }
      printf("\n");
    }

  } else {
    ++num_containers;
    uint32_t offset = 0;
    uint8_t bytes[4];
    fread(bytes, 1, 4, input);
    offset |= bytes[3];
    offset <<= 8;
    offset |= bytes[2];
    offset <<= 8;
    offset |= bytes[1];
    offset <<= 8;
    offset |= bytes[0];
    if (token == tokenize("GRUP")) {
      offset -= 24; // ???
    }

    print_tag(tag);
    for (uint32_t ctr = 0; ctr < 16; ++ctr) {
      uint8_t val;
      fread(&val, 1, 1, input);
      printf(" x%.2X", val);
    }
    printf("\n");
    size_t pos = ftell(input);
    if (verbose > 0) {
      fprintf(
          stderr,
          "\n[%.4lx] %c%c%c%c [%.4lx]\n",
          pos - 8,
          tag[0],
          tag[1],
          tag[2],
          tag[3],
          offset + pos);
    }

    if (props.is_big_item) {
      for (uint32_t ctr = 0; ctr < offset; ++ctr) {
        if (ctr == 0) {
          printf("     ");
        } else if (ctr % 23 == 0) {
          printf("\n     ");
        }
        uint8_t val;
        fread(&val, 1, 1, input);
        printf(" x%.2X", val);
      }
      printf("\n");
    } else {
      while (ftell(input) < (offset + pos)) {
        num_containers += parse(input);
      }
      if (token == tokenize("GRUP")) {
        printf("ENDGRUP\n");
      }
    }

    if (token == tokenize("TES4")) {
      --num_containers; // ???
      if (verbose > 2) {
        fprintf(stderr, "[Parsed: %u/%u]\n", num_containers, max_containers);
      }
      while (num_containers < max_containers) {
        num_containers += parse(input);
        if (verbose > 2) {
          fprintf(stderr, "[Parsed: %u/%u]\n", num_containers, max_containers);
        }
      }
      if (num_containers > max_containers) {
        printf("\n");
        fflush(stdout);
        fprintf(stderr, "ERROR: Found Too Many Containers!\n");
        std::exit(1);
      }
    }
  }

  return num_containers;
}

int main(int argc, char** argv) {
  FILE* input = stdin;
  if (argc > 2) {
    fprintf(stderr, "USAGE: eslmp_dec <filename>");
  } else if (argc > 1) {
    input = fopen(argv[1], "rb");
  }

  parse(input);

  return 0;
}
