#
# Indianapolis MotorSpeedWay
# OpenGL Project
# Makefile Script
#
# Copyright (C) 2010  Matthieu DANIEL <matth.daniel@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 
#

# compiler
CC = gcc

UNAME := $(shell $(CC) -dumpmachine | grep -E -o "linux|darwin|win")

ifeq ($(UNAME), linux)
# Linux
OSSUPPORT = linux
ARCHFLAGS =
LINKS = -lglut -lGLU -lGL -lm -ljpeg -ltiff
INCS = -I. -I ../../include
else ifeq ($(UNAME), darwin)
# Mac OSX 
OSSUPPORT = macos
ARCHFLAGS :=-arch x86_64
LINKS = -framework openGL -framework GLUT -framework cocoa -lm -ltiff -ljpeg
INCS = -I. -I /opt/local/include -L/opt/local/lib 
else
OSSUPPORT = windows
echo "OS NOT SUPPORTED"
endif

SRCS    = .

EXEC =	Indianapolis

PROG:	Indianapolis

FILES =	

Indianapolis:	Indianapolis.c
	@echo "Os supported : $(OSSUPPORT)"
	@echo "+---- Compiling "
	$(CC) $@.c LoadTexture.c $(INCS) $(LDPATH) $(LINKS) -o $@

clean:
	 rm  $(EXEC)
