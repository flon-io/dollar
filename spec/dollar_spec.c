
//
// specifying dollar
//
// Wed Nov  5 06:02:29 JST 2014
//

#include "flutil.h"

#include "dollar.h"


context "dollar"
{
  before each
  {
    flu_dict *dict = flu_d(
      "brown", "fox",
      "lazy", "dog",
      "quick", "jump",
      "l", "la",
      "z", "zy",
      "black", "PuG",
      "func", "u",
      "ba", "black adder",
      "bs", "bLACK shEEp",
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
      char *s1 = fdol_expand(s, fdol_dlup, dict);

      expect(s != s1);
      expect(s === s1);

      free(s1);
    }

    it "expands \"$(brown)\""
    {
      expect(fdol_expand("$(brown)", fdol_dlup, dict) ===f "fox");
    }
    it "expands \".$(brown).\""
    {
      expect(fdol_expand(".$(brown).", fdol_dlup, dict) ===f ".fox.");
    }
    it "expands \"$(brown) $(lazy)\""
    {
      expect(fdol_expand("$(brown) $(lazy)", fdol_dlup, dict) ===f "fox dog");
    }
    it "expands \"$($(l)$(z))\""
    {
      expect(fdol_expand("$($(l)$(z))", fdol_dlup, dict) ===f "dog");
    }

    it "expands to a blank string if it doesn't find"
    {
      expect(fdol_expand("<$(blue)>", fdol_dlup, dict) ===f "<>");
    }

    it "doesn't expand \"a)b\""
    {
      expect(fdol_expand("a)b", fdol_dlup, dict) ===f "a)b");
    }
    it "doesn't expand \"$xxx\""
    {
      expect(fdol_expand("$xxx", fdol_dlup, dict) ===f "$xxx");
    }
    it "doesn't expand \"x$xxx\""
    {
      expect(fdol_expand("x$xxx", fdol_dlup, dict) ===f "x$xxx");
    }
    it "doesn't expand \"$(nada||'$xxx)\""
    {
      expect(fdol_expand("$(nada||'$xxx)", fdol_dlup, dict) ===f "$xxx");
    }

    it "accepts an escaped )"
    {
      expect(fdol_expand("$(nada||'su\\)rf)", fdol_dlup, dict) ===f ""
        "su)rf");
    }
    it "accepts an escaped ) (deeper)"
    {
      expect(fdol_expand("$(a||'$(nada||'su\\)rf))", fdol_dlup, dict) ===f ""
        "su)rf");
    }
    it "accepts an escaped $"

    context "pipes"
    {
      it "understands || (or)"
      {
        expect(fdol_expand("$(blue||brown)", fdol_dlup, dict) ===f "fox");
      }
      it "understands |r (reverse)"
      {
        expect(fdol_expand("$(brown|r)", fdol_dlup, dict) ===f "xof");
      }
      it "understands |u (uppercase)"
      {
        expect(fdol_expand("$(brown|u)", fdol_dlup, dict) ===f "FOX");
      }
      it "understands |u|r"
      {
        expect(fdol_expand("$(brown|u|r)", fdol_dlup, dict) ===f "XOF");
      }
      it "understands |d (downcase)"
      {
        expect(fdol_expand("$(black|d)", fdol_dlup, dict) ===f "pug");
      }

      it "understands |1..-1"
      {
        expect(fdol_expand("$(quick|1..-1)", fdol_dlup, dict) ===f "ump");
      }
      it "understands |2"
      {
        expect(fdol_expand("$(quick|2)", fdol_dlup, dict) ===f "m");
      }
      it "understands |-3"
      {
        expect(fdol_expand("$(quick|-3)", fdol_dlup, dict) ===f "u");
      }

      it "understands ||'text"
      {
        expect(fdol_expand("$(nada||'text|u)", fdol_dlup, dict) ===f "TEXT");
      }

      it "understands |c (capitalize)"
      {
        expect(fdol_expand("$(ba|c)", fdol_dlup, dict) ===f "Black Adder");
        expect(fdol_expand("$(bs|c)", fdol_dlup, dict) ===f "Black Sheep");
      }

      it "understands |s/xx/yy/ (substitution filter)"
    }

    context "filter pipes"
    {
      it "understands |l>4 (length filter)"
      {
        expect(fdol_expand("$(lazy|l>4||'none)", fdol_dlup, dict) ===f "none");
        expect(fdol_expand("$(lazy|l<4||'none)", fdol_dlup, dict) ===f "dog");

        expect(fdol_expand("$(lazy|l<=3||'none)", fdol_dlup, dict) ===f "dog");
        expect(fdol_expand("$(lazy|l>=3||'none)", fdol_dlup, dict) ===f "dog");

        expect(fdol_expand("$(lazy|l=3||'none)", fdol_dlup, dict) ===f "dog");
        expect(fdol_expand("$(lazy|l=4||'none)", fdol_dlup, dict) ===f "none");
        expect(fdol_expand("$(lazy|l==3||'none)", fdol_dlup, dict) ===f "dog");
        expect(fdol_expand("$(lazy|l==4||'none)", fdol_dlup, dict) ===f "none");

        expect(fdol_expand("$(lazy|l!=4||'none)", fdol_dlup, dict) ===f "dog");
        expect(fdol_expand("$(lazy|l!=3||'none)", fdol_dlup, dict) ===f "none");
        expect(fdol_expand("$(lazy|l<>4||'none)", fdol_dlup, dict) ===f "dog");
        expect(fdol_expand("$(lazy|l<>3||'none)", fdol_dlup, dict) ===f "none");
      }

      it "understands |m/xx/ (match filter)"
    }

    context "combos"
    {
      it "understands |$(func)"
      {
        expect(fdol_expand("$(lazy|$(func))", fdol_dlup, dict) ===f "DOG");
      }
    }
  }
}

