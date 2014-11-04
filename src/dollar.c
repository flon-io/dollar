
//
// Copyright (c) 2013-2014, John Mettraux, jmettraux+flon@gmail.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// Made in Japan.
//

// https://github.com/flon-io/dollar

#define _POSIX_C_SOURCE 200809L

#include <string.h>

#include "flutil.h"
#include "aabro.h"

#include "dollar.h"


static fabr_parser *fdol_parser = NULL;

static void fdol_parser_init()
{
  fabr_parser *dol =
    fabr_seq(fabr_string("$("), fabr_n("span"), fabr_string(")"), NULL);
  fabr_parser *str =
    fabr_n_rex("str", ".+");

  fabr_parser *span =
    fabr_n_rep("span", fabr_alt(dol, str, NULL), 0, -1);

  fdol_parser = span;
}


//
// fdol_expand()

char *fdol_expand(const char *s, fdol_lookup *func, void *data)
{
  if (strchr(s, '$') == NULL) return strdup(s);

  if (fdol_parser == NULL) fdol_parser_init();

  return strdup(s);
}

