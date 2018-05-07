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
#include <vector>

#include "eslmp_tags.hpp"

int verbose = 0;
uint64_t item_size_offset = 0;

std::vector<uint8_t> out_item;
std::vector<uint8_t> out_cont;
std::vector<uint8_t> out_grup[256];
int grup_depth = 0;
void flush_item() {
  uint32_t size = out_item.size() - 6;
  if (out_item.size() > 0) {
    if (size > 0xFFFF) {
      uint8_t S[4] = {static_cast<uint8_t>(size & 0xFF),
                      static_cast<uint8_t>((size >> 8) & 0xFF),
                      static_cast<uint8_t>((size >> 16) & 0xFF),
                      static_cast<uint8_t>((size >> 24) & 0xFF)};
      uint8_t xxxx[10] = {'X', 'X', 'X', 'X', 4, 0, S[0], S[1], S[2], S[3]};
      out_cont.insert(out_cont.end(), xxxx, xxxx + 10);
    } else {
      out_item[4] = size & 0xFF;
      out_item[5] = (size >> 8) & 0xFF;
    }
    out_cont.insert(out_cont.end(), out_item.begin(), out_item.end());
    out_item.clear();
  }
}
void flush_cont() {
  flush_item();
  if (out_cont.size() > 0) {
    uint32_t size = out_cont.size() - 24;
    out_cont[4] = size & 0xFF;
    out_cont[5] = (size >> 8) & 0xFF;
    out_cont[6] = (size >> 16) & 0xFF;
    out_cont[7] = (size >> 24) & 0xFF;
    out_grup[grup_depth].insert(out_grup[grup_depth].end(), out_cont.begin(), out_cont.end());
    out_cont.clear();
  }
}
void flush_grup() {
  flush_cont();
  if (out_grup[grup_depth].size() > 0) {
    if (out_grup[grup_depth][0] == 'G' && out_grup[grup_depth][1] == 'R' &&
        out_grup[grup_depth][2] == 'U' && out_grup[grup_depth][3] == 'P') {
      uint32_t size = out_grup[grup_depth].size();
      out_grup[grup_depth][4] = size & 0xFF;
      out_grup[grup_depth][5] = (size >> 8) & 0xFF;
      out_grup[grup_depth][6] = (size >> 16) & 0xFF;
      out_grup[grup_depth][7] = (size >> 24) & 0xFF;
    }
    if (grup_depth > 1) {
      out_grup[grup_depth - 1].insert(
          out_grup[grup_depth - 1].end(), out_grup[grup_depth].begin(), out_grup[grup_depth].end());
    } else {
      for (auto b : out_grup[grup_depth]) {
        fwrite(&b, 1, 1, stdout);
      }
    }
  }
  out_grup[grup_depth].clear();
  if (grup_depth > 0) {
    --grup_depth;
  }
}

bool parse(FILE* input) {
  uint8_t tag[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  auto ret = fscanf(input, "%7s", tag);
  if (verbose > 1) {
    fprintf(stderr, "[READ:%d] '%s'\n", ret, reinterpret_cast<char*>(tag));
  }
  if (ret != 1)
    return false;

  if (tag[0] == '#' || tag[0] == '/') { // Comment
    // Just skip this whole line
    fscanf(input, "%*[^\n]");

  } else if (tag[0] == ':') { // String
    char* remainder;
    int cur;
    for (cur = 1; tag[cur] != 0; ++cur) {
      if (out_item.size() > 0) {
        out_item.push_back(tag[cur]);
      } else {
        out_cont.push_back(tag[cur]);
      }
    }
    if (fscanf(input, "%m[^\n]", &remainder) == 1) {
      if (verbose > 0) {
        fprintf(stderr, "[String] '%s%s'\n", reinterpret_cast<char*>(tag + 1), remainder);
      }
      for (cur = 0; remainder[cur] != 0; ++cur) {
        if (out_item.size() > 0) {
          out_item.push_back(static_cast<uint8_t>(remainder[cur]));
        } else {
          out_cont.push_back(static_cast<uint8_t>(remainder[cur]));
        }
      }
      free(remainder);
    } else {
      if (verbose > 0) {
        fprintf(stderr, "[String] '%s'\n", reinterpret_cast<char*>(tag + 1));
      }
    }
    if (out_item.size() > 0) {
      out_item.push_back(0);
    } else {
      out_cont.push_back(0);
    }

  } else if (tag[0] == 'x' && (tag[3] == 0 || tag[3] == ' ')) { // Hex
    uint32_t val;
    sscanf(reinterpret_cast<char*>(tag + 1), "%X", &val);
    if (val > 0xFF) {
      fprintf(stderr, "ERROR: Invalid value (%X).\n", val);
      exit(1);
    }
    if (verbose > 0) {
      fprintf(stderr, "[Hex] '%.2X'\n", val);
    }
    uint8_t to_write = val;
    if (out_item.size() > 0) {
      out_item.push_back(to_write);
    } else {
      out_cont.push_back(to_write);
    }

  } else if (!std::strcmp(reinterpret_cast<char*>(tag), "ENDGRUP")) { // End of GRUP marker
    flush_grup();
  } else { // Tag
    auto token = tokenize(tag);
    tag_type props = get_tag_props(token);

    if (!props.is_known) {
      fprintf(stderr, "ERROR: Unknown tag [%c%c%c%c]!\n", tag[0], tag[1], tag[2], tag[3]);
      exit(1);
    }

    // Normalize
    tag[3] = token & 0xFF;
    tag[2] = (token >> 8) & 0xFF;
    tag[1] = (token >> 16) & 0xFF;
    tag[0] = (token >> 24) & 0xFF;

    if (verbose > 0) {
      fprintf(stderr, "[%c%c%c%c]\n", tag[0], tag[1], tag[2], tag[3]);
    }

    if (props.is_container) {
      flush_cont();
      if (token == tokenize("GRUP")) {
        if (grup_depth == 0) {
          // Write out anything outside any GRUP
          flush_grup();
        }
        ++grup_depth;
      }
      out_cont.push_back(tag[0]);
      out_cont.push_back(tag[1]);
      out_cont.push_back(tag[2]);
      out_cont.push_back(tag[3]);
      out_cont.push_back(0);
      out_cont.push_back(0);
      out_cont.push_back(0);
      out_cont.push_back(0);
    } else {
      flush_item();
      out_item.push_back(tag[0]);
      out_item.push_back(tag[1]);
      out_item.push_back(tag[2]);
      out_item.push_back(tag[3]);
      out_item.push_back(0);
      out_item.push_back(0);
    }
  }
  return true;
}

int main(int argc, char** argv) {
  FILE* input = stdin;
  if (argc > 2) {
    fprintf(stderr, "USAGE: eslmp_enc <filename>");
  } else if (argc > 1) {
    input = fopen(argv[1], "rb");
  }

  while (parse(input))
    ;
  flush_grup();

  return 0;
}
