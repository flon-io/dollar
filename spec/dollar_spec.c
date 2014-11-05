
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
    char *r = flu_list_get(data, path);
    return r ? strdup(r) : NULL;
  }

  before each
  {
    flu_dict *dict = flu_d(
      "brown", "fox",
      "lazy", "dog",
      "quick", "jump",
      "l", "la",
      "z", "zy",
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
  }
}

