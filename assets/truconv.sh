#!/bin/sh

GRIT=/opt/wonderful/thirdparty/blocksds/core/tools/grit/grit


$GRIT mainsensor.png -ftB -fh! -gTFF00FF -gt -gB8 -m!
$GRIT sonic.png -ftB -fh! -gTFF00FF -gt -gB8 -m!
$GRIT cmap.png -ftB -fh! -g! -gB8 -mRt -mLf -p!
$GRIT colmap.png -ftB -fh! -gTFF00FF -gt -gB8 -mR8 -mLs