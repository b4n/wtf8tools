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

#ifndef H_VERBOSE
#define H_VERBOSE

#if !defined(__attribute__) && !defined(__GNUC__)
# define __attribute__(x) /* nothing */
#endif

__attribute__((format(printf, 1, 2)))
void verbose(const char *fmt,
             ...);

#endif /* H_VERBOSE */
