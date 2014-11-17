
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
    flu_dict *d = flu_d(
      "brown", "fox",
      "lazy", "dog",
      "quick", "jump",
      "l", "la",
      "z", "zy",
      "black", "PuG",
      "func", "u",
      "ba", "black adder",
      "bs", "bLACK shEEp",
      "msg", "\"hello world\"",
      "msg1", "hello \"le monde\"",
      NULL);
  }
  after each
  {
    flu_list_free(d);
  }

  describe "fdol_expand()"
  {
    it "returns a copy of the string, even if no expansion took place"
    {
      char *s = "quick brown fox";
      char *s1 = fdol_expand(s, d, fdol_dlup);

      expect(s != s1);
      expect(s === s1);

      free(s1);
    }

    it "expands \"$(brown)\""
    {
      expect(fdol_expand("$(brown)", d, fdol_dlup) ===f "fox");
    }
    it "expands \".$(brown).\""
    {
      expect(fdol_expand(".$(brown).", d, fdol_dlup) ===f ".fox.");
    }
    it "expands \"$(brown) $(lazy)\""
    {
      expect(fdol_expand("$(brown) $(lazy)", d, fdol_dlup) ===f "fox dog");
    }
    it "expands \"$($(l)$(z))\""
    {
      expect(fdol_expand("$($(l)$(z))", d, fdol_dlup) ===f "dog");
    }

    it "expands to a blank string if it doesn't find"
    {
      expect(fdol_expand("<$(blue)>", d, fdol_dlup) ===f "<>");
    }

    it "doesn't expand \"a)b\""
    {
      expect(fdol_expand("a)b", d, fdol_dlup) ===f "a)b");
    }
    it "doesn't expand \"$xxx\""
    {
      expect(fdol_expand("$xxx", d, fdol_dlup) ===f "$xxx");
    }
    it "doesn't expand \"x$xxx\""
    {
      expect(fdol_expand("x$xxx", d, fdol_dlup) ===f "x$xxx");
    }
    it "doesn't expand \"$(nada||'$xxx)\""
    {
      expect(fdol_expand("$(nada||'$xxx)", d, fdol_dlup) ===f "$xxx");
    }

    it "accepts an escaped )"
    {
      expect(fdol_expand("$(nada||'su\\)rf)", d, fdol_dlup) ===f ""
        "su)rf");
    }
    it "accepts an escaped ) (deeper)"
    {
      expect(fdol_expand("$(a||'$(nada||'su\\)rf))", d, fdol_dlup) ===f ""
        "su)rf");
    }
    it "accepts an escaped $"

    context "pipes"
    {
      it "understands || (or)"
      {
        expect(fdol_expand("$(blue||brown)", d, fdol_dlup) ===f "fox");
      }
      it "understands |r (reverse)"
      {
        expect(fdol_expand("$(brown|r)", d, fdol_dlup) ===f "xof");
      }
      it "understands |u (uppercase)"
      {
        expect(fdol_expand("$(brown|u)", d, fdol_dlup) ===f "FOX");
      }
      it "understands |u|r"
      {
        expect(fdol_expand("$(brown|u|r)", d, fdol_dlup) ===f "XOF");
      }
      it "understands |d (downcase)"
      {
        expect(fdol_expand("$(black|d)", d, fdol_dlup) ===f "pug");
      }

      it "understands |1..-1"
      {
        expect(fdol_expand("$(quick|1..-1)", d, fdol_dlup) ===f "ump");
      }
      it "understands |2"
      {
        expect(fdol_expand("$(quick|2)", d, fdol_dlup) ===f "m");
      }
      it "understands |-3"
      {
        expect(fdol_expand("$(quick|-3)", d, fdol_dlup) ===f "u");
      }

      it "understands ||'text"
      {
        expect(fdol_expand("$(nada||'text|u)", d, fdol_dlup) ===f "TEXT");
      }

      it "understands |c (capitalize)"
      {
        expect(fdol_expand("$(ba|c)", d, fdol_dlup) ===f "Black Adder");
        expect(fdol_expand("$(bs|c)", d, fdol_dlup) ===f "Black Sheep");
      }

      it "understands |q ([double] quote)"
      {
        expect(fdol_expand("the $(ba|c|q)", d, fdol_dlup) ===f ""
          "the \"Black Adder\"");
        expect(fdol_expand("the $(bs|c|q)", d, fdol_dlup) ===f ""
          "the \"Black Sheep\"");
      }
      it "doesn't double quote when |q"
      {
        expect(fdol_expand("$(msg|q)", d, fdol_dlup) ===f ""
          "\"hello world\"");
      }
      it "double quotes when |Q"
      {
        expect(fdol_expand("$(msg|Q)", d, fdol_dlup) ===f ""
          "\"\\\"hello world\\\"\"");
      }
      it "escapes when |q"
      {
        expect(fdol_expand("$(msg1|q)", d, fdol_dlup) ===f ""
          "\"hello \\\"le monde\\\"\"");
      }

      it "understands |s/xx/yy/ (substitution filter)"
    }

    context "filter pipes"
    {
      it "understands |l>4 (length filter)"
      {
        expect(fdol_expand("$(lazy|l>4||'none)", d, fdol_dlup) ===f "none");
        expect(fdol_expand("$(lazy|l<4||'none)", d, fdol_dlup) ===f "dog");

        expect(fdol_expand("$(lazy|l<=3||'none)", d, fdol_dlup) ===f "dog");
        expect(fdol_expand("$(lazy|l>=3||'none)", d, fdol_dlup) ===f "dog");

        expect(fdol_expand("$(lazy|l=3||'none)", d, fdol_dlup) ===f "dog");
        expect(fdol_expand("$(lazy|l=4||'none)", d, fdol_dlup) ===f "none");
        expect(fdol_expand("$(lazy|l==3||'none)", d, fdol_dlup) ===f "dog");
        expect(fdol_expand("$(lazy|l==4||'none)", d, fdol_dlup) ===f "none");

        expect(fdol_expand("$(lazy|l!=4||'none)", d, fdol_dlup) ===f "dog");
        expect(fdol_expand("$(lazy|l!=3||'none)", d, fdol_dlup) ===f "none");
        expect(fdol_expand("$(lazy|l<>4||'none)", d, fdol_dlup) ===f "dog");
        expect(fdol_expand("$(lazy|l<>3||'none)", d, fdol_dlup) ===f "none");
      }

      it "understands |m/xx/ (match filter)"
    }

    context "combos"
    {
      it "understands |$(func)"
      {
        expect(fdol_expand("$(lazy|$(func))", d, fdol_dlup) ===f "DOG");
      }
    }
  }

  describe "fdol_quote_expand()"
  {
    it "expands and wraps in quote at the first level"
    {
      expect(fdol_quote_expand("quick $(brown)", d, fdol_dlup) ===f ""
        "quick \"fox\"");
    }

    it "expands and escapes"
    {
      expect(fdol_quote_expand("$(msg1)", d, fdol_dlup) ===f ""
        "\"hello \\\"le monde\\\"\"");
    }

    it "expands into empty quotes"
    {
      expect(fdol_quote_expand("quick $(nada)", d, fdol_dlup) ===f ""
        "quick \"\"");
    }

    it "only expands at the first level"
    {
      expect(fdol_quote_expand("the $($(l)$(z))", d, fdol_dlup) ===f ""
        "the \"dog\"");
    }
  }
}

