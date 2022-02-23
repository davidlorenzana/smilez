/* Define PY_SSIZE_T_CLEAN to silence:
 * DeprecationWarning: PY_SSIZE_T_CLEAN will be required for '#' formats
 *
 * PY_SSIZE_T_CLEAN was introduced in Python 2.5
 */
#define PY_SSIZE_T_CLEAN

#include <Python.h>

#include "smilez.h"
#include "smilez.c"

#include <stdlib.h>

#include <stdio.h>


#define FAST_BUFFER_SIZE 8010

static PyObject *
compress_py(PyObject *self, PyObject *args) {
  char *smiles;
  int smiles_len;
  char fast_buffer[FAST_BUFFER_SIZE];
  char *compressed_smiles;
  int compressed_len;
  int dictionary = 0;
  PyObject *result;

  if (!PyArg_ParseTuple(args, "s#|i:compress", &smiles, &smiles_len, &dictionary)) {
//   if (!PyArg_ParseTuple(args, "t#|i:compress", &smiles, &smiles_len, &dictionary)) {
    return NULL;
  }

  if (dictionary < 0 || dictionary >= SMILEZ_NUM_DICTIONARIES) {
    PyErr_SetString(PyExc_ValueError, "dictionary value is out of range");
    return NULL;
  }
  

  /* Won't handle more than 50 MB of SMILES string */
  if (smiles_len > 50*1024*1024) {
    PyErr_SetString(PyExc_ValueError, "SMILES string is too long");
    return NULL;
  }
  
  compressed_len = smilez_compress(smiles, smiles_len,
                                   fast_buffer, FAST_BUFFER_SIZE, dictionary);
  if (compressed_len < FAST_BUFFER_SIZE) {
    return PyBytes_FromStringAndSize(fast_buffer, compressed_len);
  }

  /* Grrr. Need to try again with more space. */
  /* Worse case compression is 2x, and only if the encoder is broken. */
  /* Real worst-case compression is 50% larger. (escape 1 byte, encode 1 byte => 3 bytes) */
  
  compressed_smiles = PyMem_New(char, 2*smiles_len+1);
  compressed_len = smilez_compress(smiles, smiles_len, compressed_smiles, 2*smiles_len+1, dictionary);
  if (compressed_len == 2*smiles_len+1) {
      PyErr_SetString(PyExc_AssertionError,
                      "Compression growth beyond expected bounds");
      PyMem_Del(compressed_smiles);
      return NULL;
  }
  
  result = PyBytes_FromStringAndSize(compressed_smiles, compressed_len);
  PyMem_Del(compressed_smiles);
  return result;
}



static PyObject *
decompress_py(PyObject *self, PyObject *args) {
  char *compressed_smiles;
  int compressed_smiles_len;
  char fast_buffer[FAST_BUFFER_SIZE];
  char *uncompressed_smiles;
  int uncompressed_len;
  PyObject *result;

  if (!PyArg_ParseTuple(args, "y*|i:decompress", &compressed_smiles, &compressed_smiles_len)) {
    return NULL;
  }

  /* Won't handle more than 50 MB of compressed SMILES string */
  if (compressed_smiles_len > 50*1024*1024) {
    PyErr_SetString(PyExc_ValueError, "compressed SMILES string is too long");
    return NULL;
  }
    
  uncompressed_len = smilez_decompress(compressed_smiles, compressed_smiles_len,
                                       fast_buffer, FAST_BUFFER_SIZE);
  
  printf("decompressed.");
  
  if (uncompressed_len < FAST_BUFFER_SIZE) {
    return PyUnicode_FromStringAndSize(fast_buffer, uncompressed_len);
  }

  /* Grrr. Need to try again with more space. */
  /* Maximum decompression ratio is 5x */
  
  uncompressed_smiles = PyMem_New(char, 5*compressed_smiles_len+1);
  uncompressed_len = smilez_decompress(compressed_smiles, compressed_smiles_len,
                                       uncompressed_smiles, 5*compressed_smiles_len+1);
  if (uncompressed_len == 5*compressed_smiles_len+1) {
      PyErr_SetString(PyExc_AssertionError,
                      "Decompression growth beyond expected bounds");
      PyMem_Del(uncompressed_smiles);
      return NULL;
  }
  result = PyUnicode_FromStringAndSize(uncompressed_smiles, uncompressed_len);
  PyMem_Del(uncompressed_smiles);
  return result;
}


static PyMethodDef SmilezMethods[] = {
  {"compress", compress_py, METH_VARARGS,
   "Compress"},
  {"decompress", decompress_py, METH_VARARGS,
   "Decompress"},

  {NULL, NULL, 0, NULL}      /*  Sentinel  */
};

static struct PyModuleDef smilez = {
    PyModuleDef_HEAD_INIT,
    "smilez",     /* m_name */
    "smilez doc.",  /* m_doc */
    -1,                  /* m_size */
    SmilezMethods,    /* m_methods */
    NULL,                /* m_reload */
    NULL,                /* m_traverse */
    NULL,                /* m_clear */
    NULL,                /* m_free */
};

PyMODINIT_FUNC
PyInit_smilez(void) {
  PyObject *m;

  m = PyModule_Create(&smilez);
  if (m == NULL) {
    return;
  }

  PyModule_AddObject(m, "__version__",
                     Py_BuildValue("s", smilez_get_version()));

  PyModule_AddObject(m, "compression_version",
                     Py_BuildValue("i", smilez_get_compression_version()));

  PyModule_AddObject(m, "BYTE_DICTIONARY",
                     Py_BuildValue("i", SMILEZ_BYTE_DICTIONARY));
  PyModule_AddObject(m, "WHITESPACE_DICTIONARY",
                     Py_BuildValue("i", SMILEZ_WHITESPACE_DICTIONARY));
  PyModule_AddObject(m, "NUM_DICTIONARIES",
                     Py_BuildValue("i", SMILEZ_NUM_DICTIONARIES));
  
  return m;
  
}
