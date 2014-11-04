
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
      "red", "fox", "green", "frog", "blue", "bird", NULL);
  }
  after each
  {
    flu_list_free(dict);
  }

  describe "fdol_expand()"
  {
    it "expands $(red)"
    {
      expect(fdol_expand(".$(red).", fd_lookup, dict) ===f ".fox.");
    }
  }
}

