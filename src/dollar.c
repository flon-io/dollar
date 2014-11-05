
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

#include <stdlib.h>
#include <string.h>

#include "flutil.h"
#include "aabro.h"

#include "dollar.h"


static fabr_parser *fdol_parser = NULL;

static void fdol_parser_init()
{
  fabr_parser *dol =
    fabr_n_seq(
      "d", fabr_string("$("), fabr_n("p"), fabr_string(")"), NULL);
  fabr_parser *str =
    fabr_n_rex(
      "s",
      "("
        "[^\\$\\)]" "|"
        "\\$[^\\(]"
      ")+");

  //fabr_parser *span =
    fabr_n_rep(
      "p", fabr_alt(dol, str, NULL), 0, -1);

  fabr_parser *outerstr =
    fabr_n_rex(
      "s",
      "("
        "[^\\$]" "|" // doesn't mind ")"
        "\\$[^\\(]"
      ")+");

  fdol_parser =
    fabr_n_rep(
      "r", fabr_alt(dol, outerstr, NULL), 0, -1);

  // TODO: give possibility to escape )
}


//
// fdol_expand()

static char *expand(const char *s, fabr_tree *t, fdol_lookup *func, void *data)
{
  //puts(fabr_tree_to_string(t, s, 1));

  if (*t->name == 's') return fabr_tree_string(s, t);

  if (*t->name == 'd')
  {
    char *d = expand(s, t->child->sibling, func, data);
    char *dd = func(d, data);
    free(d);
    //printf("d >%s<\n", dd);
    return dd;
  }

  // 'r' or 'p'

  flu_sbuffer *b = flu_sbuffer_malloc();

  for (fabr_tree *c = t->child; c != NULL; c = c->sibling)
  {
    fabr_tree *cc = c->child;
    //puts("-");
    //puts(fabr_tree_to_string(cc, s, 1));
    char *r = expand(s, cc, func, data);
    //printf("r: >%s<\n", r);
    if (r) { flu_sbputs(b, r); free(r); }
  }

  return flu_sbuffer_to_string(b);
}

char *fdol_expand(const char *s, fdol_lookup *func, void *data)
{
  if (strchr(s, '$') == NULL) return strdup(s);

  if (fdol_parser == NULL) fdol_parser_init();

  //printf("s >%s<\n", s);
  ////fabr_tree *tt = fabr_parse_f(s, 0, fdol_parser, FABR_F_ALL);
  ////fabr_tree *tt = fabr_parse_f(s, 0, fdol_parser, FABR_F_PRUNE | FABR_F_ALL);
  //fabr_tree *tt = fabr_parse_all(s, 0, fdol_parser);
  //puts(fabr_tree_to_string(tt, s, 1));
  //fabr_tree_free(tt);

  fabr_tree *t = fabr_parse_all(s, 0, fdol_parser);
  //puts(fabr_tree_to_string(t, s, 1)); puts("---");
  char *r = expand(s, t, func, data);
  fabr_tree_free(t);

  return r;
}

