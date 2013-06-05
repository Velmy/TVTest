#!/bin/sh
version=""
clean="no"

config_h="./src/config.h"

# Check options
for opt do
    case "$opt" in
        --clean)
            clean="yes"
        ;;
    esac
done

# Check versions of libraries
libz=""
libpng=""
libjpeg=""
libz_check_h="./src/TVTest/TVTest_Image/zlib/zlib.h"
libpng_check_h="./src/TVTest/TVTest_Image/libpng/png.h"
libjpeg_check_h="./src/TVTest/TVTest_Image/libjpeg/jversion.h"
libjpeg_conf_h="./src/TVTest/TVTest_Image/libjpeg/jconfig.h"
libjname=""

if test "$clean" = "no" ; then
  if test -f "$libz_check_h" ; then
    libz=`cat "$libz_check_h" | awk '/#define ZLIB_VERSION/{print $3}' | sed -e 's/"*"//g'`
    echo "[zlib:$libz]"
  fi
  if test -f "$libpng_check_h" ; then
    libpng=`cat "$libpng_check_h" | awk '/#define PNG_LIBPNG_VER_STRING/{print $3}' | sed -e 's/"//g'`
    echo "[libpng:$libpng]"
  fi
  if test -f "$libjpeg_check_h" ; then
    libjname="libjpeg"
    libjpeg=`cat "$libjpeg_check_h" | awk '/#define JVERSION/{print $3}' | sed -e 's/"//g'`
    if test -f "$libjpeg_conf_h" ; then
      libjturbo=`cat "$libjpeg_conf_h" | awk '/#define LIBJPEG_TURBO_VERSION/{print $3}'`
      if test "$libjturbo" != "" ; then
        libjver=`cat "$libjpeg_conf_h" | awk '/#define JPEG_LIB_VERSION/{print $3}'`
        if test $libjver -ge 80 ; then
          libjturbo_select=1
        elif test $libjver -ge 70 ; then
          libjturbo_select=2
        else
          libjturbo_select=3
        fi
        libjpeg=`echo $libjpeg | cut -d " " -f $libjturbo_select`
        libjpeg="$libjturbo-$libjpeg"
        libjname="libjpeg-turbo"
      fi
    fi
    echo "[$libjname:$libjpeg]"
  fi
fi

# Output config.h
if test "$clean" = "yes" ; then
cat > "$config_h" << EOF
#undef TVTEST_GIT_VERSION
#undef TVTEST_IMAGE_ZLIB
#undef TVTEST_IMAGE_LIBPNG
#undef TVTEST_IMAGE_LIBJPEG
#undef TVTEST_IMAGE_LIBJPEG_NAME
EOF
else
  if [ -d ".git" ] && [ -n "`git tag`" ]; then
    version="`git describe --tags`"
    echo "$version"
    echo "#define TVTEST_GIT_VERSION          \"$version\"" > "$config_h"
  else
    echo "#undef TVTEST_GIT_VERSION" > "$config_h"
  fi

def_libs(){
  if test "$1" != "" ; then
    count=`echo ${#2}`
    count=$((28-1-${count}))
    space=" "
    while [ "$count" != "0" ]
    do
      space=`echo "$space "`
      count=$((${count}-1))
    done
    echo "#define $2$space\"$1\"" >> "$config_h"
  else
    echo "#undef $2" >> "$config_h"
  fi
}
  def_libs  "$libz"      "TVTEST_IMAGE_ZLIB"
  def_libs  "$libpng"    "TVTEST_IMAGE_LIBPNG"
  def_libs  "$libjpeg"   "TVTEST_IMAGE_LIBJPEG"
  def_libs  "$libjname"  "TVTEST_IMAGE_LIBJPEG_NAME"
fi
