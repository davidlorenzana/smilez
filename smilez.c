#include <string.h>
#include <smilez.h>

/* SMILEZ - A compression algorithm for SMILES strings

 Based on ideas and code from SMAZ, https://github.com/antirez/smaz

Copyright (c) 2014, Andrew Dalke
Trollhattan, Sweden

Copyright (c) 2006-2009, Salvatore Sanfilippo
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

  * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in
    the documentation and/or other materials provided with the
    distribution.

  * Neither the name of Smaz nor the names of its contributors may be
    used to endorse or promote products derived from this software
    without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/* The compression codebooks, used for compression */
/* This one may encode to '\r', '\n', ' ', and '\t' */
static char *Smilez_cb_byte[229] = {
  "\00232\001", "\0021C\337\003]1C\010", "\005[C@@HH\004(C=CN", "",
  "\002NCt", "", "", "\005[NH+]\352", "", "\0022C`", "\004/C=C\273",
  "\0023=\275", "\005CC1=CQ\002OC?\003)C3\250", "\002n1\211\004l)Cl\200",
  "\003O)O\021", "\005C(C=CL\003c1)\210",
  "\005CCN(C\375\004)NCC\241\003c2)\036", "\0023Cy\005OC(=Oh",
  "\003c(cT", "", "\003COC\341\0022N\016", "\004)CCC\236", "", "", "",
  "\005NC(=OD\005NC(=S\205", "", "",
  "\004=O)C>\003c2c\363\004[nH]\336\005N(C)C\251",
  "\003c3c\343\003H](\306\003)CO\235", "\0053)C(=\357\001=\005", "", "",
  "", "\003C=C(\0051=C(C\240\0041=C(\020", "\0031)CY\003)Clc", "",
  "\004)[C@\225", "", "\004cc2)\201", "\003CCC:", "", "\005=CC=CE",
  "\0032)C#\0041)CC\322", "\004C=C2\314\002SC\301", "\003C(C-",
  "\003C)C\334\005)c2cc\324", "", "", "\003cc(\312", "", "\001#\015", "",
  "", "\003C1C\260\003=C3\026", "\005Oc1cc\262\004=O)c\226", "",
  "\001]\213", "\005S(=O)G\004CCC1\223", "",
  "\005COc1c\373\004=O)O\025\005=C2)C\014", "", "\004C1)C\257",
  "\002nc\362\003(=N\323", "\004C3)C\274", "", "\005[C@H]F\003c(n\244",
  "", "\004C=C1\353", "\0043CCC\351", "", "\0054=CC='",
  "\003)NCX\003OCC\354", "\003)OCS\004O)OC\227", "", "\003cc3\346",
  "\004)N(C\361", "\003O-]w\0042CCC^\005c(c(c\310", "\003O)C5\001C\364",
  "\0043=CC&\003N2Cq", "", "\005)C(C)\246", "", "", "\005C(C)C\300",
  "\003(CC$", "\005COC1=[\003=C1\231", "\0052=CC=!", "\003)O)\350", "",
  "\003N=C\360", "\005c1cccI\005c1cc(r\001(~\003cc2\276",
  "\005c2cccJ\003)C(\256\005c2cc(\242",
  "\005c3cccM\003)CC\333\003C(N\013", "\005c4ccc+\004(c(cd\004O)O)\232",
  "\0051=CC=9\005c5ccc\321", "", "\002=C7\005C=C(C\335", "\005c(cc1u",
  "\001)\342", "\003(C)f", "\003N(C]", "\003CC1|\001F\022",
  "\005CCC(C\265", "\0041)NC\367", "\001c\253", "", "\003(cc%",
  "\002=Nj\002[C\234", "\005cccccl", "", "\003)F)\237", "",
  "\005CC(=O2\005CC(=Ca\002=S\031", "\005CNC(=\263", "\001+\006", "",
  "\0051C(=O\355\004OC)C\264", "\002C(\313", "\002C)\215",
  "\005[N+](/\003CCN\247", "", "", "", "", "\0044CCC\243", "",
  "\002C1\261", "\002C2\365", "\002C3{\002#N\222", "",
  "\002C5\204\001-\012", "", "\005=C(C=O\0032CC}\004=C(C\315", "", "",
  "", "\003NN=\007", "", "\001.\017", "", "\004)C2=\316", "\0051)C(=)",
  "\005(=O)CB\005(=O)NC\004(=O)R\005(=O)O6\002F)@", "", "\002CCZ", "",
  "\003=NN\255\001/\032", "", "\005CC(C)V\005CC(C(\307\004(C2=\214",
  "\0052)C(=3", "\0031=C\212", "\005(F)(F\325", "", "\002c1\347",
  "\0034)Ck\005(cc2)_\002c2\217\004(cc2\034", "\002CN\340",
  "\0031CCn\002CO\037", "\002(CU", "\0032=Co", "", "", "",
  "\005(cc3)\356\0011\311", "\0052cccc\027\0042ccc\002", "",
  "\002)CW\001N\374", "\003=O)\267", "\003SCC\221", "\002)F\030",
  "\005CCCCC;\004CCCCb\002(O\344", "\0012\326", "\0042)Cl\224", "",
  "\001O\271", "", "", "\002)N=\003[N+\004", "\002)O\233",
  "\0013\277\005]([C@\272", "", "\005)C(=OK\005cc(cc\371",
  "\004)C)C\266\001P ", "\005=CC(=8", "\004C1CC\320\002Br\305",
  "\003(c2\207", "\005C(=O)A\002Cl\370", "\0014\372", "\004=C3)\011",
  "\005(C(=Oz\005(C(=C\252", "\004(C)C*\005(C)C)1\002(c\345", "", "",
  "\001n\230", "", "\0015\302", "\0052=O)C\220",
  "\005CC=C4x\004CC=C\\005CC=C2\177\005CC=C1\245\005CC=C3\003", "", "",
  "", "\004O)NCg", "", "\0016\203", "\004N(CC0\0021)m\002cc\332",
  "\0041)OC\327", "", "", "\005CN(CCp\0032)O\254", "", "\003ccc.",
  "\005C1=CC<\005CCOC(\330\004CCOC\304\002H]\033",
  "\003(=C4\0022)s\005C2=CCe\005C2=C(\317", "\005C3=CCi", "",
  "\005Cc1cc\"\004Cc1c\216\002O)\202\002cn\035", "", "",
  "\004C(=CP\005C[C@@\270\002l)\206", "", "\0023),\004=O)[v",
  "\003](C\366\003C)c\024", "", "", "\002-]\023", "", "", "\003O)N\331",
  "\0024)\303"
};

/* This one does not encode to '\r', '\n', ' ', or '\t' */
static char *Smilez_cb_whitespace_safe[229] = {
  "\00232\001", "\0021C\337\003]1C\010", "\005[C@@HH\004(C=CN", "",
  "\002NCt", "", "", "\005[NH+]\352", "", "\0022C`", "\004/C=C\273",
  "\0023=\275", "\005CC1=CQ\002OC?\003)C3\250", "\002n1\211\004l)Cl\200",
  "\003O)O\021", "\005C(C=CL\003c1)\210",
  "\005CCN(C\375\004)NCC\241\003c2)\036", "\0023Cy\005OC(=Oh",
  "\003c(cT", "", "\003COC\341\0022N\016", "\004)CCC\236", "", "", "",
  "\005NC(=OD\005NC(=S\205", "", "",
  "\004=O)C>\003c2c\363\004[nH]\336\005N(C)C\251",
  "\003c3c\343\003H](\306\003)CO\235", "\0053)C(=\357\001=\005", "", "",
  "", "\003C=C(\0051=C(C\240\0041=C(\020", "\0031)CY\003)Clc", "",
  "\004)[C@\225", "", "\004cc2)\201", "\003CCC:", "", "\005=CC=CE",
  "\0032)C#\0041)CC\322", "\004C=C2\314\002SC\301", "\003C(C-",
  "\003C)C\334\005)c2cc\324", "", "", "\003cc(\312", "", "", "", "",
  "\003C1C\260\003=C3\026", "\005Oc1cc\262\004=O)c\226", "", "\001]\213",
  "\005S(=O)G\004CCC1\223", "", "\005COc1c\373\004=O)O\025\005=C2)C\014",
  "", "\004C1)C\257", "\002nc\362\003(=N\323", "\004C3)C\274", "",
  "\005[C@H]F\003c(n\244", "", "\004C=C1\353", "\0043CCC\351", "",
  "\0054=CC='", "\003)NCX\003OCC\354", "\003)OCS\004O)OC\227", "",
  "\003cc3\346", "\004)N(C\361", "\003O-]w\0042CCC^\005c(c(c\310",
  "\003O)C5\001C\364", "\0043=CC&\003N2Cq", "", "\005)C(C)\246", "", "",
  "\005C(C)C\300", "\003(CC$", "\005COC1=[\003=C1\231", "\0052=CC=!",
  "\003)O)\350", "", "\003N=C\360",
  "\005c1cccI\005c1cc(r\001(~\003cc2\276",
  "\005c2cccJ\003)C(\256\005c2cc(\242",
  "\005c3cccM\003)CC\333\003C(N\013", "\005c4ccc+\004(c(cd\004O)O)\232",
  "\0051=CC=9\005c5ccc\321", "", "\002=C7\005C=C(C\335", "\005c(cc1u",
  "\001)\342", "\003(C)f", "\003N(C]", "\003CC1|\001F\022",
  "\005CCC(C\265", "\0041)NC\367", "\001c\253", "", "\003(cc%",
  "\002=Nj\002[C\234", "\005cccccl", "", "\003)F)\237", "",
  "\005CC(=O2\005CC(=Ca\002=S\031", "\005CNC(=\263", "\001+\006", "",
  "\0051C(=O\355\004OC)C\264", "\002C(\313", "\002C)\215",
  "\005[N+](/\003CCN\247", "", "", "", "", "\0044CCC\243", "",
  "\002C1\261", "\002C2\365", "\002C3{\002#N\222", "", "\002C5\204", "",
  "\005=C(C=O\0032CC}\004=C(C\315", "", "", "", "\003NN=\007", "",
  "\001.\017", "", "\004)C2=\316", "\0051)C(=)",
  "\005(=O)CB\005(=O)NC\004(=O)R\005(=O)O6\002F)@", "", "\002CCZ", "",
  "\003=NN\255\001/\032", "", "\005CC(C)V\005CC(C(\307\004(C2=\214",
  "\0052)C(=3", "\0031=C\212", "\005(F)(F\325", "", "\002c1\347",
  "\0034)Ck\005(cc2)_\002c2\217\004(cc2\034", "\002CN\340",
  "\0031CCn\002CO\037", "\002(CU", "\0032=Co", "", "", "",
  "\005(cc3)\356\0011\311", "\0052cccc\027\0042ccc\002", "",
  "\002)CW\001N\374", "\003=O)\267", "\003SCC\221", "\002)F\030",
  "\005CCCCC;\004CCCCb\002(O\344", "\0012\326", "\0042)Cl\224", "",
  "\001O\271", "", "", "\002)N=\003[N+\004", "\002)O\233",
  "\0013\277\005]([C@\272", "", "\005)C(=OK\005cc(cc\371",
  "\004)C)C\266", "\005=CC(=8", "\004C1CC\320\002Br\305", "\003(c2\207",
  "\005C(=O)A\002Cl\370", "\0014\372", "", "\005(C(=Oz\005(C(=C\252",
  "\004(C)C*\005(C)C)1\002(c\345", "", "", "\001n\230", "", "\0015\302",
  "\0052=O)C\220",
  "\005CC=C4x\004CC=C\\005CC=C2\177\005CC=C1\245\005CC=C3\003", "", "",
  "", "\004O)NCg", "", "\0016\203", "\004N(CC0\0021)m\002cc\332",
  "\0041)OC\327", "", "", "\005CN(CCp\0032)O\254", "", "\003ccc.",
  "\005C1=CC<\005CCOC(\330\004CCOC\304\002H]\033",
  "\003(=C4\0022)s\005C2=CCe\005C2=C(\317", "\005C3=CCi", "",
  "\005Cc1cc\"\004Cc1c\216\002O)\202\002cn\035", "", "",
  "\004C(=CP\005C[C@@\270\002l)\206", "", "\0023),\004=O)[v",
  "\003](C\366\003C)c\024", "", "", "\002-]\023", "", "", "\003O)N\331",
  "\0024)\303"
};


/* Reverse compression codebook, used for decompression */
/* (Note: smilez does not use the '\0' character) */
static char *Smilez_rcb[254] = {
  "", "32", "2ccc", "CC=C3", "[N+", "=", "+", "NN=", "]1C", "=C3)", "-",
  "C(N", "=C2)C", "#", "2N", ".", "1=C(", "O)O", "F", "-]", "C)c",
  "=O)O", "=C3", "2cccc", ")F", "=S", "/", "H]", "(cc2", "cn", "c2)",
  "CO", "P", "2=CC=", "Cc1cc", "2)C", "(CC", "(cc", "3=CC", "4=CC=",
  "C=C", "1)C(=", "(C)C", "c4ccc", "3)", "C(C", "ccc", "[N+](", "N(CC",
  "(C)C)", "CC(=O", "2)C(=", "(=C", "O)C", "(=O)O", "=C", "=CC(=",
  "1=CC=", "CCC", "CCCCC", "C1=CC", ")N", "=O)C", "OC", "F)", "C(=O)",
  "(=O)C", "(=O)N", "NC(=O", "=CC=C", "[C@H]", "S(=O)", "[C@@H", "c1ccc",
  "c2ccc", ")C(=O", "C(C=C", "c3ccc", "(C=C", "=C(C=", "C(=C", "CC1=C",
  "(=O)", ")OC", "c(c", "(C", "CC(C)", ")C", ")NC", "1)C", "CC", "COC1=",
  "CC=C", "N(C", "2CCC", "(cc2)", "2C", "CC(=C", "CCCC", ")Cl", "(c(c",
  "C2=CC", "(C)", "O)NC", "OC(=O", "C3=CC", "=N", "4)C", "ccccc", "1)",
  "1CC", "2=C", "CN(CC", "N2C", "c1cc(", "2)", "NC", "c(cc1", "=O)[",
  "O-]", "CC=C4", "3C", "(C(=O", "C3", "CC1", "2CC", "(", "CC=C2",
  "l)Cl", "cc2)", "O)", "6", "C5", "NC(=S", "l)", "(c2", "c1)", "n1",
  "1=C", "]", "(C2=", "C)", "Cc1c", "c2", "2=O)C", "SCC", "#N", "CCC1",
  "2)Cl", ")[C@", "=O)c", "O)OC", "n", "=C1", "O)O)", ")O", "[C", ")CO",
  ")CCC", ")F)", "1=C(C", ")NCC", "c2cc(", "4CCC", "c(n", "CC=C1",
  ")C(C)", "CCN", ")C3", "N(C)C", "(C(=C", "c", "2)O", "=NN", ")C(",
  "C1)C", "C1C", "C1", "Oc1cc", "CNC(=", "OC)C", "CCC(C", ")C)C", "=O)",
  "C[C@@", "O", "]([C@", "/C=C", "C3)C", "3=", "cc2", "3", "C(C)C", "SC",
  "5", "4)", "CCOC", "Br", "H](", "CC(C(", "c(c(c", "1", "cc(", "C(",
  "C=C2", "=C(C", ")C2=", "C2=C(", "C1CC", "c5ccc", "1)CC", "(=N",
  ")c2cc", "(F)(F", "2", "1)OC", "CCOC(", "O)N", "cc", ")CC", "C)C",
  "C=C(C", "[nH]", "1C", "CN", "COC", ")", "c3c", "(O", "(c", "cc3",
  "c1", ")O)", "3CCC", "[NH+]", "C=C1", "OCC", "1C(=O", "(cc3)", "3)C(=",
  "N=C", ")N(C", "nc", "c2c", "C", "C2", "](C", "1)NC", "Cl", "cc(cc",
  "4", "COc1c", "N", "CCN(C"
};


static int internal_smilez_compress(const char *in, int inlen, char *out, int outlen,
                                    char **Smilez_cb) {
  unsigned int h1, h2, h3, h4;
  int verblen = 0, _outlen = outlen;
  char verb[256], *_out = out;

  while (inlen) {
    int j = 5, needed;
    char *flush = NULL;
    char *slot;

    /* I can't use the smaz hash function because there are too many
       common prefixes. This one seems to do a good enough job. */

    h1 = h2 = in[0] << 3;
    if (inlen > 1) {
      h2 += in[1];
    }
    if (inlen > 2) {
      h3 = h4 = in[2] << 4;
      if (inlen > 3) {
        h3 = h2 ^ h3;
        h4 = h2 ^ ((h4 + in[3])<<3);
      }
    } else {
      h3 = h4 = 0;
    }

    if (j > inlen) {
      j = inlen;
    }

    /* Try to lookup substrings into the hash table, starting from the
     * longer to the shorter substrings */
    for (; j > 0; j--) {
      switch(j) {
      case 1: slot = Smilez_cb[h1 % 229]; break;
      case 2: slot = Smilez_cb[h2 % 229]; break;
      case 3: slot = Smilez_cb[h3 % 229]; break;
      default: slot = Smilez_cb[h4 % 229]; break;
      }
      while (slot[0]) {
        if (slot[0] == j && memcmp(slot+1, in, j) == 0) {
          /* Match found in the hash table,
           * prepare a verbatim bytes flush if needed */
          if (verblen) {
            needed = (verblen == 1) ? 2 : 2+verblen;
            flush = out;
            out += needed;
            outlen -= needed;
          }
          /* Emit the byte */
          if (outlen <= 0) {
            return _outlen+1;
          }
          out[0] = slot[slot[0]+1];
          out++;
          outlen--;
          inlen -= j;
          in += j;
          goto out;
        } else {
          slot += slot[0]+2;
        }
      }
    }
    /* Match not found - add the byte to the verbatim buffer */
    verb[verblen] = in[0];
    verblen++;
    inlen--;
    in++;

  out:

    /* Prepare a flush if we reached the flush length limit, and there
     * is not already a pending flush operation. */
    if (!flush && (verblen == 256 || (verblen > 0 && inlen == 0))) {
      needed = (verblen == 1) ? 2 : 2+verblen;
      flush = out;
      out += needed;
      outlen -= needed;
      if (outlen < 0) {
        return _outlen+1;
      }
    }
    /* Perform a verbatim flush if needed */
    if (flush) {
      if (verblen == 1) {
        flush[0] = (signed char)254;
        flush[1] = verb[0];
      } else {
        flush[0] = (signed char)255;
        flush[1] = (signed char)(verblen-1);
        memcpy(flush+2,verb,verblen);
      }
      flush = NULL;
      verblen = 0;
    }
  }
  return out-_out;
}

int smilez_compress(const char *in, int inlen, char *out, int outlen, int dictionary) {
  switch (dictionary) {
  case 0:
    return internal_smilez_compress(in, inlen, out, outlen, Smilez_cb_byte);
  case 1:
    return internal_smilez_compress(in, inlen, out, outlen, Smilez_cb_whitespace_safe);
  default:
    return 0;
  }
}

int smilez_decompress(const char *in, int inlen, char *out, int outlen) {
    const unsigned char *c = (const unsigned char*) in;
    char *_out = out;
    int _outlen = outlen;

    while(inlen) {
        if (*c == 254) {
            /* Verbatim byte */
            if (outlen < 1) return _outlen+1;
            *out = *(c+1);
            out++;
            outlen--;
            c += 2;
            inlen -= 2;
        } else if (*c == 255) {
            /* Verbatim string */
            int len = (*(c+1))+1;
            if (outlen < len) return _outlen+1;
            memcpy(out,c+2,len);
            out += len;
            outlen -= len;
            c += 2+len;
            inlen -= 2+len;
        } else {
            /* Codebook entry */
            char *s = Smilez_rcb[*c];
            int len = strlen(s);

            if (outlen < len) return _outlen+1;
            memcpy(out,s,len);
            out += len;
            outlen -= len;
            c++;
            inlen--;
        }
    }
    return out-_out;
}


const char *smilez_get_version(void) {
  return SMILEZ_VERSION;
}
int smilez_get_compression_version(void) {
  return SMILEZ_COMPRESSION_VERSION;
}
