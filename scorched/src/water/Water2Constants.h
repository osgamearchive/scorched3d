////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
//
//    This file is part of Scorched3D.
//
//    Scorched3D is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    Scorched3D is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Scorched3D; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
////////////////////////////////////////////////////////////////////////////////

#if !defined(__INCLUDE_Water2Constantsh_INCLUDE__)
#define __INCLUDE_Water2Constantsh_INCLUDE__

static inline float myfmod(float a, float b) { return a-floorf(a/b)*b; }//fmod is different for negative a/b

static const float wave_tidecycle_time = 10.24f;
static const unsigned int wave_phases = 256;
static const unsigned int wave_patch_width = 16;
static const unsigned int wave_resolution = 128;

static const float wavetile_length = 256.0f;
static const float wave_waterwidth = wavetile_length;
static const float wavetile_length_rcp = 1.0f / wavetile_length;

#endif // __INCLUDE_Water2Constantsh_INCLUDE__
