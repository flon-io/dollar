
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
    //flu_dict *d = (flu_dict *)data;
    return (char *)flu_list_get(data, path);
  }

  before each
  {
    flu_dict *dict = flu_d(
      "brown", "fox", "lazy", "dog", "quick", "jump", NULL);
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

    it "expands $(red)"
    {
      expect(fdol_expand(".$(red).", fd_lookup, dict) ===f ".fox.");
    }
  }
}

