#!/usr/bin/env python

import os
from distutils.core import setup, Extension

def read(fname):
    return open(os.path.join(os.path.dirname(__file__), fname)).read()

setup(
    name = "smilez",
    version = "1.0",
    description = "A compressor and decompressor library for cheminformatics SMILES strings",
    keywords = "cheminformatics SMILES compression",
    
    author = "Andrew Dalke",
    author_email = "dalke@dalkescientific.com",
    
    url = "https://bitbucket.org/dalke/smilez",
    license = "BSD",
    long_description = read("README"),

    classifiers = [
        "Intended Audience :: Science/Research",
        "License :: OSI Approved :: BSD License",
        "Operating System :: OS Independent",
        "Programming Language :: C",
        "Programming Language :: Python :: 2 :: Only",
        "Topic :: Scientific/Engineering :: Chemistry",
        "Topic :: Software Development :: Libraries :: Python Modules",
        "Development Status :: 5 - Production/Stable",
        ],
    
    
    ext_modules = [Extension("smilez", ["smilez_python.c"],
                             include_dirs = ["."]) ]
    )
