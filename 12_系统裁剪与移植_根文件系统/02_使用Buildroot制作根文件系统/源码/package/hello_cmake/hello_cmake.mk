################################################################################
#
# hello_cmake
#
################################################################################

HELLO_CMAKE_SOURCE =hello_cmake.tar.bz2
HELLO_CMAKE_SITE =$(TOPDIR)/dl/hello_cmake


$(eval $(cmake-package))
