#!/bin/bash
#/******************************************************************************
# *
# * Project:  OpenCPN
# * Purpose:  helper shell script
# * Author:   David Register
# *
# ***************************************************************************
# *   Copyright (C) 2010 by David S. Register   *
# *                                                                         *
# *   This program is free software; you can redistribute it and/or modify  *
# *   it under the terms of the GNU General Public License as published by  *
# *   the Free Software Foundation; either version 2 of the License, or     *
# *   (at your option) any later version.                                   *
# *                                                                         *
# *   This program is distributed in the hope that it will be useful,       *
# *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
# *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
# *   GNU General Public License for more details.                          *
# *                                                                         *
# *   You should have received a copy of the GNU General Public License     *
# *   along with this program; if not, write to the                         *
# *   Free Software Foundation, Inc.,                                       *
# *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
# ***************************************************************************
# */

path=$(dirname $0)

# Require icotool from package icoutils
# Require convert from package ImageMagick
# Require inkscape

for pic in instrument_engine.svg
do
  echo "converting $pic"
  inkscape --export-type="png" --export-dpi=72 --export-background-opacity=0 --export-width=20 --export-height=20 $pic
done

png2wx.pl -C icon_engine.cpp -H icon_engine.h -M ICONS_H instrument_engine.png

rm $path/*.png

