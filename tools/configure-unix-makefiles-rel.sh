#!/bin/sh

MATHPRESSO_CURRENT_DIR=`pwd`
MATHPRESSO_BUILD_DIR="build_makefiles_rel"
MATHPRESSO_ASMJIT_DIR="../asmjit"

mkdir ../${MATHPRESSO_BUILD_DIR}
cd ../${MATHPRESSO_BUILD_DIR}
cmake .. -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DASMJIT_DIR="%MATHPRESSO_ASMJIT_DIR%" -DMATHPRESSO_BUILD_TEST=1
cd ${MATHPRESSO_CURRENT_DIR}