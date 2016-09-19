/*
 * wtf8tools - WTF-8 conversion tools
 * Copyright (C) 2016  Colomban Wendling <ban@herbesfolles.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "wtf32.h"
#include "verbose.h"

int wtf32_read(FILE *const fp,
               void (*callback) (uint32_t point,
                                 void *data),
               void *data)
{
  size_t len;
  uint32_t point;
  
  do {
    len = fread(&point, sizeof point, 1, fp);
    
    if (len == 1) {
      if (point <= 0x10FFFF) {
        callback(point, data);
      } else {
        verbose("read invalid code point U+%.8X\n", point);
        return -2;
      }
    }
  } while (len == 1);
  
  return (len == 0 && feof(fp)) ? 0 : -1;
}

void wtf32_write(uint32_t const point,
                 FILE *const fp)
{
  if (point <= 0x10FFFF) {
    fwrite(&point, sizeof point, 1, fp);
  } else {
    verbose("tried to write point U+%.8X\n", point);
    assert("invalid input point > 0x10FFFF" == NULL);
  }
}
