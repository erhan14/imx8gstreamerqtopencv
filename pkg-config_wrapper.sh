#!/bin/sh
PKG_CONFIG_SYSROOT_DIR=/home/serkan/Documents/imx8/sysroots/aarch64-poky-linux
export PKG_CONFIG_SYSROOT_DIR
PKG_CONFIG_LIBDIR=/home/serkan/Documents/imx8/sysroots/aarch64-poky-linux/usr/lib/pkgconfig:/home/serkan/Documents/imx8/sysroots/aarch64-poky-linux/usr/share/pkgconfig:/home/serkan/Documents/imx8/sysroots/aarch64-poky-linux/usr/lib/aarch64-poky-linux/pkgconfig
export PKG_CONFIG_LIBDIR
exec pkg-config "$@"
