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

#include "wtf16.h"
#include "verbose.h"

int wtf16_read(FILE *const fp,
               void (*callback) (uint32_t point,
                                 void *data),
               void *data)
{
  size_t len;
  uint16_t unit;
  
  do {
    len = fread(&unit, sizeof unit, 1, fp);
    
  next:
    if (len == 1) {
      if (unit >= 0xD800 && unit <= 0xDBFF) { /* high surrogate */
        uint32_t high = unit;
        
        /* try and read the next unit */
        len = fread(&unit, sizeof unit, 1, fp);
        if (len == 1 && unit >= 0xDC00 && unit <= 0xDFFF) { /* low surrogate */
          callback(0x10000 | ((high & 0x3ff) << 10) | (unit & 0x3ff), data);
        } else {
          verbose("ignoring missing low surrogate\n");
          callback(high, data);
          goto next; /* be forgiving and ignore missing low surrogate */
        }
      } else if (unit >= 0xDC00 && unit <= 0xDFFF) { /* naked low surrogate */
        verbose("ignoring missing high surrogate\n");
        callback(unit, data);
      } else {
        callback(unit, data);
      }
    }
  } while (len == 1);
  
  return (len == 0 && feof(fp)) ? 0 : -1;
}

void wtf16_write(uint32_t const point,
                 FILE *const fp)
{
  if (/*point < 0xD800 || (point >= 0xE000 && point <= 0xFFFF)*/
      point <= 0xFFFF /* include reserved U+D800-U+DFFF */) {
    uint16_t ch[] = {
      (uint16_t) (point & 0xFFFF)
    };
    fwrite(ch, sizeof *ch, sizeof ch / sizeof *ch, fp);
  } else if (point <= 0x10FFFF) {
    uint16_t ch[] = {
      (uint16_t) (0xD800 + (((point - 0x10000) >> 10) & 0x3ff)),
      (uint16_t) (0xDC00 + (((point - 0x10000) >>  0) & 0x3ff))
    };
    fwrite(ch, sizeof *ch, sizeof ch / sizeof *ch, fp);
  } else {
    verbose("tried to write point U+%.8X\n", point);
    assert("invalid input point > 0x10FFFF" == NULL);
  }
}
