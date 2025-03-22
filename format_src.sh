#!/bin/bash

find src/ -iname '*.h' -o -iname '*.cpp' -o -iname '*.cxx' | xargs clang-format -i
find inc/ -iname '*.h' -o -iname '*.cpp' -o -iname '*.cxx' | xargs clang-format -i

