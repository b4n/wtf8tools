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

#include "wtf8.h"
#include "wtf32.h"

static void output(uint32_t const point,
                   void *const data)
{
  wtf8_write(point, data);
}

int main(void)
{
  return -wtf32_read(stdin, output, stdout);
}
