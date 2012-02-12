/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/* Based on the Adventure Game Studio source code, copyright 1999-2011 Chris Jones,
 * which is licensed under the Artistic License 2.0.
 */

namespace AGS {

static const AGSGameDescription gameDescriptions[] = {
	{
		{
			"5days",
			"Version 1.3",
			AD_ENTRY1s("5days.exe", "e88fd6a23a5e498d7b0d50e3bb914085", 4440143),
			Common::EN_USA,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			GUIO1(GUIO_NOASPECT)
		},
	},
	{
		{
			"5days",
			"Special Edition",
			AD_ENTRY1s("5days.exe", "3b7cceb3e4bdb031dc5d8f290936e94b", 4614351),
			Common::EN_USA,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			GUIO1(GUIO_NOASPECT)
		},
	},
	{
		{
			"7days",
			"",
			AD_ENTRY1s("7days.exe", "465f972675db2da6040518221af5b0ba", 4693374),
			Common::EN_USA,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			GUIO1(GUIO_NOASPECT)
		},
	},
	{
		{
			"7days",
			"Special Edition",
			AD_ENTRY1s("7days.exe", "3b7cceb3e4bdb031dc5d8f290936e94b", 4863356),
			Common::EN_USA,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			GUIO1(GUIO_NOASPECT)
		},
	},
	{
		{
			"trilbysnotes",
			"",
			AD_ENTRY1s("notes.exe", "3b7cceb3e4bdb031dc5d8f290936e94b", 5736960),
			Common::EN_USA,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			GUIO1(GUIO_NOASPECT)
		},
	},
	{
		{
			"trilbysnotes",
			"Special Edition",
			AD_ENTRY1s("notes se.exe", "3b7cceb3e4bdb031dc5d8f290936e94b", 5880445),
			Common::EN_USA,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			GUIO1(GUIO_NOASPECT)
		},
	},
	{
		{
			"6days",
			"",
			AD_ENTRY1s("6days.exe", "3b7cceb3e4bdb031dc5d8f290936e94b", 7993899),
			Common::EN_USA,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			GUIO1(GUIO_NOASPECT)
		},
	},
	{
		{
			"6days",
			"Special Edition",
			AD_ENTRY1s("6das se.exe", "3b7cceb3e4bdb031dc5d8f290936e94b", 8173314),
			Common::EN_USA,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			GUIO1(GUIO_NOASPECT)
		},
	},
	{ AD_TABLE_END_MARKER }
};

static const AGSGameDescription fallbackDescs[] = {
	{ //0
		{
			"ags",
			"unknown",
			AD_ENTRY1(0, 0),
			Common::UNK_LANG,
			Common::kPlatformUnknown,
			ADGF_NO_FLAGS,
			GUIO_NONE
		},
	}
};

static const ADFileBasedFallback fileBased[] = {
	{ &fallbackDescs[ 0].desc, { "ac2game.dat", 0 } },
	{ 0, { 0 } }
};

} // End of namespace AGS
