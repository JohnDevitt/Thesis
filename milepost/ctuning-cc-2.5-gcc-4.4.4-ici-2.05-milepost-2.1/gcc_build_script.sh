diff -rupN ctuning-cc-2.5-gcc-4.4.4-ici-2.05-milepost-2.1-original/_build_gcc.sh ctuning-cc-2.5-gcc-4.4.4-ici-2.05-milepost-2.1/_build_gcc.sh
--- ctuning-cc-2.5-gcc-4.4.4-ici-2.05-milepost-2.1-original/_build_gcc.sh	2010-05-19 07:40:45.000000000 +0100
+++ ctuning-cc-2.5-gcc-4.4.4-ici-2.05-milepost-2.1/_build_gcc.sh	2013-06-25 19:03:04.140582253 +0100
@@ -98,7 +98,7 @@ make install
 echo "****** Building PPL ..."
 mkdir ${OBJ_DIR}-ppl
 cd ${OBJ_DIR}-ppl
-CFLAGS=-I$BUILD_DIR/include CXXFLAGS=-I$BUILD_DIR/include LDFLAGS=-L$BUILD_DIR/lib $SRC_DIR/src-third-party/$PPL_DIR/configure --prefix=$BUILD_DIR --with-libgmp-prefix=$BUILD_DIR
+CFLAGS="-I$BUILD_DIR/include -fpermissive" CXXFLAGS="-I$BUILD_DIR/include -fpermissive" LDFLAGS=-L$BUILD_DIR/lib $SRC_DIR/src-third-party/$PPL_DIR/configure --prefix=$BUILD_DIR --with-libgmp-prefix=$BUILD_DIR
  if [ "${?}" != "0" ] ; then
    echo "Error: Configuration failed at $PWD!" 
    exit

