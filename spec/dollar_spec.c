
//
// specifying dollar
//
// Wed Nov  5 06:02:29 JST 2014
//

#include "flutil.h"

#include "dollar.h"


context "dollar"
{
  char *fd_lookup(const char *path, void *data)
  {
    //printf("      lookup: >%s<\n", path);
    char *r = flu_list_get(data, path);
    return r ? rdz_strdup(r) : NULL;
  }

  before each
  {
    flu_dict *dict = flu_d(
      "brown", "fox",
      "lazy", "dog",
      "quick", "jump",
      "l", "la",
      "z", "zy",
      "black", "PuG",
      NULL);
  }
  after each
  {
    flu_list_free(dict);
  }

  describe "fdol_expand()"
  {
    it "returns a copy of the string, even if no expansion took place"
    {
      char *s = "quick brown fox";
      char *s1 = fdol_expand(s, fd_lookup, dict);

      expect(s != s1);
      expect(s === s1);

      free(s1);
    }

    it "expands \"$(brown)\""
    {
      expect(fdol_expand("$(brown)", fd_lookup, dict) ===f "fox");
    }
    it "expands \".$(brown).\""
    {
      expect(fdol_expand(".$(brown).", fd_lookup, dict) ===f ".fox.");
    }
    it "expands \"$(brown) $(lazy)\""
    {
      expect(fdol_expand("$(brown) $(lazy)", fd_lookup, dict) ===f "fox dog");
    }
    it "expands \"$($(l)$(z))\""
    {
      expect(fdol_expand("$($(l)$(z))", fd_lookup, dict) ===f "dog");
    }

    it "expands to a blank string if it doesn't find"
    {
      expect(fdol_expand("<$(blue)>", fd_lookup, dict) ===f "<>");
    }

    context "pipes"
    {
      it "understands ||"
      {
        expect(fdol_expand("$(blue||brown)", fd_lookup, dict) ===f "fox");
      }
      it "understands |r"
      {
        expect(fdol_expand("$(brown|r)", fd_lookup, dict) ===f "xof");
      }
      it "understands |u"
      {
        expect(fdol_expand("$(brown|u)", fd_lookup, dict) ===f "FOX");
      }
      it "understands |u|r"
      {
        expect(fdol_expand("$(brown|u|r)", fd_lookup, dict) ===f "XOF");
      }
      it "understands |d"
      {
        expect(fdol_expand("$(black|d)", fd_lookup, dict) ===f "pug");
      }

      it "understands |1..-1"
      {
        expect(fdol_expand("$(quick|1..-1)", fd_lookup, dict) ===f "ump");
      }
      it "understands |2"
      {
        expect(fdol_expand("$(quick|2)", fd_lookup, dict) ===f "m");
      }
      it "understands |-3"
      {
        expect(fdol_expand("$(quick|-3)", fd_lookup, dict) ===f "u");
      }

      it "understands ||'text"
      {
        expect(fdol_expand("$(nada||'text|u)", fd_lookup, dict) ===f "TEXT");
      }
    }
  }
}

