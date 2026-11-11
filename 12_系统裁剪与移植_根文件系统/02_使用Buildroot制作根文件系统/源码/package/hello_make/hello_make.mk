################################################################################
#
# hello_make
#
################################################################################

HELLO_MAKE_SOURCE =hello_make.tar.gz
HELLO_MAKE_SITE =$(TOPDIR)/dl/hello_make

define HELLO_MAKE_BUILD_CMDS
    $(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D) 
endef

define HELLO_MAKE_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/hello_make $(TARGET_DIR)/usr/bin
endef

$(eval $(generic-package))
