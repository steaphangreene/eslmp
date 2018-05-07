# *************************************************************************
#  This file is part of the eslmp project by Steaphan Greene
#
#  Copyright 1995-2018 Steaphan Greene <steaphan@gmail.com>
#
#  eslmp is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 3 of the License, or
#  (at your option) any later version.
#
#  eslmp is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with eslmp (see the file named "LICENSE");
#  If not, see <http://www.gnu.org/licenses/>.
#
# *************************************************************************

OBJS:=
LIBS:=

all:	eslmp_dec eslmp_enc

#Production Settings (dynamic)
CXX=	clang++ -std=c++14 -Wall -Werror -O3

#Use debugging settings
debug:
	+make CXX='clang++ -std=c++14 -g -fsanitize=thread -Wall -Werror'

#Use profiling settings
profile:
	+make CXX='g++ -std=c++14 -g -pg -fprofile-arcs -Wall -Werror'

clean:
	rm -f gmon.out deps.mk *.o *.da eslmp_dec

eslmp_dec: eslmp_dec.o *.hpp
	rm -f "$@"
	$(CXX) -Wall -Werror -o "$@" eslmp_dec.o $(OBJS) $(LIBS)

eslmp_enc: eslmp_enc.o *.hpp
	rm -f "$@"
	$(CXX) -Wall -Werror -o "$@" eslmp_enc.o $(OBJS) $(LIBS)

%.o: %.cpp
	$(CXX) -c $<

include deps.mk

deps.mk:	*.cpp *.hpp
	$(CXX) -MM *.cpp > deps.mk
