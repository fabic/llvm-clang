#!/bin/sh

echo | clang++ -Wp,-v -x c++ - -fsyntax-only

