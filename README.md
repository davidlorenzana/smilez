## SMILEZ - compression for SMILES strings

This is a Python 3 port of smilez library.

Original SMILEZ home page: https://bitbucket.org/dalke/smilez

Smilez is a simple compression library for SMILES strings. It's
closely based on the SMAZ compression library for short strings,
available from https://github.com/antirez/smaz .

Smilez is not a general purpose compression algorithm. It can compress
SMILES strings by about 50-60%, including many SMILES strings which
are only a few bytes long. In the best case it can compress up to 75%,
but if you give it non-SMILES data the result might be 50% larger.

You can use SMILEZ through the C API or the Python API.


## COMPRESSION EXAMPLES

The SMILEZ code dictionaries were trained on a mixture of PubChem and
ChEMBL data. Compression performance will depend on how closely the
SMILES strings match that training set. In general it does best with
"normal" SMILES, and poorly with uncommon features like reaction maps
and isotopes.

| from size | to size | SMILES | 
| --------- | ------- | ------ |
|         1 |     1   | C
|         2 |    1    | CC
|         3 |    2    | CCC
|         4 |    1    | CCCC
|         7 |    4    | F/C=C/F
|         6 |    8    | [NH4+]
|         9 |    4    | c1ccccc1O
|        26 |   13    | Cn1cnc2c1c(=O)n(c(=O)n2C)C
|        28 |   10    | CN1C=NC2=C1C(=O)N(C(=O)N2C)C
|        42 |   17    | COc1ccc2nc([nH]c2c1)S(=O)Cc1ncc(C)c(OC)c1C
|        43 |   46    | [CH2:1]=[CH:2][CH2:1][CH2:3][C:4](C)[CH2:3]
|        60 |   30    | C[N+]1(C2CC(CC1C3C2O3)OC(=O)C(C4=CC=CS4)(C5=CC=CS5)O)C.[Br-]


## ChEMBL Example

I have a SMILES data set derived from ChEMBL 16 with 1,292,344
records. 

* uncompressed:  66,896,248 bytes
* compressed:  25,455,759 bytes

=> 60%  smaller

I timed the process using the Python API. It took about 2.6 seconds to
compress the strings and 1.4 seconds to decompress them.

By comparison, it takes zlib about 51 seconds to compress the same
data set. The result takes up 61,406,214 bytes, which is only 8%
smaller than the original. (This is why you don't use zlib on small
strings.)


I double-checked using the 24,323 SMILES from the NCI data set. The
compressed SMILES are 58% smaller than the original SMILES (1.21 MB to
0.51 MB).

## C API

   `int smilez_compress(const char *in, int inlen, char *out, int outlen,
                       int dictionary)`

Compress the SMILES string in 'in' of length 'inlen' and put the
compressed data into 'out' of maximum length 'outlen' bytes. If the
output buffer is too short to hold the whole compressed string,
outlen+1 is returned. Otherwise, the length of the compressed string
(less then or equal to outlen) is returned.

The 'dictionary' option specifies the encoding dictionary. If
SMILEZ_BYTE_DICTIONARY (=0) then the compressed string may use the
bytes 0-255. If SMILEZ_WHITESPACE_DICTIONARY (=1) then the compressed
strings will not use the bytes "\n", "\r", "\t", or " ". This lets you
use a SMILEZ string as a field in a tab or space separated file, or in
an SD tag, for a very small size penalty.

The dictionary only affects the encoder. The decoder handles both
dictionaries automatically. If the decoder value is out of range then
smilez_compress returns 0.


    int smilez_decompress(char *in, int inlen, char *out, int outlen);

Decompress the buffer 'in' of length 'inlen' and put the decompressed data into
'out' of max length 'outlen' bytes. If the output buffer is too short to hold
the whole decompressed string, outlen+1 is returned. Otherwise the length of the
compressed string (less then or equal to outlen) is returned. This function will
not automatically put a nul-term at the end of the string if the original
compressed string didn't included a nulterm.


The following macros are nearly self-explanatory:

    #define SMILEZ_VERSION "1.0"
    #define SMILEZ_MAJOR_VERSION 1
    #define SMILEZ_MINOR_VERSION 0

    #define SMILEZ_COMPRESSION_VERSION 1

    #define SMILEZ_BYTE_DICTIONARY 0
    #define SMILEZ_WHITESPACE_DICTIONARY 1
    #define SMILEZ_NUM_DICTIONARIES 2

The SMILEZ_COMPRESSION_VERSION will change only when the compression
format changes.

The SMILEZ_VERSION and SMILEZ_COMPRESSION_VERSION values are also
available through function calls:

    const char *smilez_get_version(void);
    int smilez_get_compression_version(void);


## Python API

Use "python setup.py install" to build and install the Python module
"smilez".

The module defines a few constants, based on smilez_get_version() and
smilez_get_compression_version():

    >>> import smilez
    >>> smilez.__version__
    '1.0'
    >>> smilez.compression_version
    1

as well as constants for the two dictionaries:

    >>> smilez.BYTE_DICTIONARY
    0
    >>> smilez.WHITESPACE_DICTIONARY
    1

Here's an example of how to compress and decompress phenol:

    >>> smilez.compress("c1ccccc1O")
    'I\xda\xc9\xb9'

    >>> smilez.decompress('I\xda\xc9\xb9')
    'c1ccccc1O'

and an example of compressing with the whitespace dictionary:

    >>> smilez.compress("P", smilez.WHITESPACE_DICTIONARY)
    '\xfeP'
    >>> smilez.compress("P")
    ' '

## Command-line programs

Running "make" will make three demonstration or test programs:

### smilezip

Convert a SMILES file into a SMILEZ file.

A SMILES file contains one record per line. Each line contains space
or tab delimited fields where the SMILES is the first field. A SMILEZ
file is the same format, after compressing the first field.

    % ./smilezip nci_09425001_09450000.smi > nci.smiz
    % ls -l nci_09425001_09450000.smi nci.smiz
    -rw-r--r--  1 dalke  staff  1433190 Jun  8  2008 nci_09425001_09450000.smi
    -rw-r--r--  1 dalke  staff   724140 Jul  2 02:52 nci.smiz

NOTE! There is no real reason to have a SMILEZ file. The gzip'ed
version of the same file is only 187483 bytes, or almost 1/4th the
size of the SMILEZ file. This is a demonstration of how one might use
the whitespace dictionary.


### smilezcat

Convert a SMILEZ file into a SMILES file:

    % ./smilezcat nci.smiz > nci.smi
    % cmp nci_09425001_09450000.smi nci.smi



### smilez_test

This runs a set of self-tests on the C API.




## CREDITS

Smaz was written by Salvatore Sanfilippo and is released under the BSD license. Check the COPYING file for more information.

Smilez was written by Andrew Dalke and is released under the same BSD license.

Port to Python 3 by David Lorenzana and is released under the same BSD license.
