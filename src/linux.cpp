/*
  Stockfish, a UCI chess playing engine derived from Glaurung 2.1
  Copyright (C) 2004-2008 Tord Romstad (Glaurung author)
  Copyright (C) 2008-2013 Marco Costalba, Joona Kiiski, Tord Romstad

  Stockfish is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Stockfish is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cstring>
#include <iostream>
#include <cerrno>

extern "C" {
#include <sys/mman.h>
}

#include "os.h"

class Linux : public OS {
    public:
    void FreeTable(void* table, size_t size);
    void* AllocTable(size_t size); 
};


void Linux::FreeTable(void* table, size_t size) 
{
    munmap(table, size);
}

void* Linux::AllocTable(size_t size) 
{
  void*	addr;
  int	prot = PROT_READ | PROT_WRITE;
  int	flags = MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB;
  addr = mmap(NULL, size, prot, flags, -1, 0);

  if (addr != (void*) -1) {
      std::cerr << "mmap " << size << " with huge page OK " << std::endl ;
  } else {
      addr = 0;
      std::cerr << "mmap allocation " << size << "bytes failed";
      std::cerr	<< " with errno == " << errno << " " << std::endl ;
      std::cerr << "echo `size' > /proc/sys/vm/nr_hugepages might help !" << std::endl ;
  }

  return addr;
}

OS* os = new Linux;
