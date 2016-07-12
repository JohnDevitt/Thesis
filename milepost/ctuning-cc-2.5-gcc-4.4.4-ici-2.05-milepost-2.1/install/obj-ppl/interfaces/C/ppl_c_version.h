/* Header file providing version information for the C interface.  -*- C -*-
   Copyright (C) 2001-2009 Roberto Bagnara <bagnara@cs.unipr.it>

This file is part of the Parma Polyhedra Library (PPL).

The PPL is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The PPL is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.

For the most up-to-date information see the Parma Polyhedra Library
site: http://www.cs.unipr.it/ppl/ . */

/*! \brief
  A string containing the PPL version.

  \ingroup Version
  Let <CODE>M</CODE> and <CODE>m</CODE> denote the numbers associated
  to PPL_VERSION_MAJOR and PPL_VERSION_MINOR, respectively.  The
  format of PPL_VERSION is <CODE>M "." m</CODE> if both
  PPL_VERSION_REVISION (<CODE>r</CODE>) and PPL_VERSION_BETA
  (<CODE>b</CODE>)are zero, <CODE>M "." m "pre" b</CODE> if
  PPL_VERSION_REVISION is zero and PPL_VERSION_BETA is not zero,
  <CODE>M "." m "." r</CODE> if PPL_VERSION_REVISION is not zero and
  PPL_VERSION_BETA is zero, <CODE>M "." m "." r "pre" b</CODE> if
  neither PPL_VERSION_REVISION nor PPL_VERSION_BETA are zero.
*/
#define PPL_VERSION "0.10.2"

/*! \brief
  The major number of the PPL version.
  \ingroup Version
*/
#define PPL_VERSION_MAJOR 0

/*! \brief
  The minor number of the PPL version.
  \ingroup Version
*/
#define PPL_VERSION_MINOR 10

/*! \brief
  The revision number of the PPL version.
  \ingroup Version
*/
#define PPL_VERSION_REVISION 2

/*! \brief
  The beta number of the PPL version.  This is zero for official
  releases and nonzero for development snapshots.
  \ingroup Version
*/
#define PPL_VERSION_BETA 0