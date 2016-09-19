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

#include "wtf8.h"
#include "verbose.h"

int wtf8_read(FILE *const fp,
              void (*callback) (uint32_t point,
                                void *data),
              void *data)
{
  int c;
  
  do {
    c = getc(fp);
    
    if (c != EOF) {
      unsigned char b = (unsigned char) c;
      int len = 1;
      uint32_t point = b;
      
      if ((b & 0xf8) == 0xf8) {
        verbose("read invalid leading byte 0x%X\n", b);
        return -2;
      } else if ((b & 0xf8) == 0xf0) {
        len = 4;
        point = b & 0x07;
      } else if ((b & 0xf0) == 0xe0) {
        len = 3;
        point = b & 0x0f;
      } else if ((b & 0xe0) == 0xc0) {
        len = 2;
        point = b & 0x1f;
      } else if ((b & 0xc0) == 0x80) {
        verbose("lone inner byte 0x%X\n", b);
        return -2;
      }
      while (len-- > 1) {
        c = getc(fp);
        
        b = (unsigned char) c;
        if (c == EOF) {
          verbose("EOF inside a sequence\n");
          return -2;
        } else if ((b & 0xc0) != 0x80) {
          verbose("leading byte 0x%X inside a sequence\n", b);
          return -2;
        } else {
          point = (point << 6) | (b & 0x3f);
        }
      }
      callback(point, data);
    }
  } while (c != EOF);
  
  return (c != EOF && feof(fp)) ? 0 : -1;
}

void wtf8_write(uint32_t const point,
                FILE *const fp)
{
  if (point < 0x80) {
    unsigned char ch[] = {
      (unsigned char) point & 0xff
    };
    fwrite(ch, 1, sizeof ch, fp);
  } else if (point < 0x800) {
    unsigned char ch[] = {
      (unsigned char) (0xc0 | ((point >> 6) & 0x1f)),
      (unsigned char) (0x80 | ((point >> 0) & 0x3f))
    };
    fwrite(ch, 1, sizeof ch, fp);
  } else if (point < 0x10000) {
    unsigned char ch[] = {
      (unsigned char) (0xe0 | ((point >> 12) & 0x0f)),
      (unsigned char) (0x80 | ((point >>  6) & 0x3f)),
      (unsigned char) (0x80 | ((point >>  0) & 0x3f))
    };
    fwrite(ch, 1, sizeof ch, fp);
  } else if (point < 0x110000) {
    unsigned char ch[] = {
      (unsigned char) (0xf0 | ((point >> 18) & 0x07)),
      (unsigned char) (0x80 | ((point >> 12) & 0x3f)),
      (unsigned char) (0x80 | ((point >>  6) & 0x3f)),
      (unsigned char) (0x80 | ((point >>  0) & 0x3f))
    };
    fwrite(ch, 1, sizeof ch, fp);
  } else {
    verbose("tried to write point U+%.8X\n", point);
    assert("invalid input point > 0x10FFFF" == NULL);
  }
}
