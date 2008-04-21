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
 *
 * $URL$
 * $Id$
 *
 */


#include "common/endian.h"
#include "common/scummsys.h"
#include "common/util.h"

#include "cine/cine.h"
#include "cine/object.h"
#include "cine/part.h"
#include "cine/various.h"

namespace Cine {

objectStruct objectTable[NUM_MAX_OBJECT];
Common::List<overlay> overlayList;

void loadObject(char *pObjectName) {
	uint16 numEntry;
	uint16 entrySize;
	uint16 i;
	byte *ptr, *dataPtr;

	checkDataDisk(-1);

	ptr = dataPtr = readBundleFile(findFileInBundle(pObjectName));

	setMouseCursor(MOUSE_CURSOR_DISK);

	numEntry = READ_BE_UINT16(ptr); ptr += 2;

	entrySize = READ_BE_UINT16(ptr); ptr += 2;

	assert(numEntry <= NUM_MAX_OBJECT);

	for (i = 0; i < numEntry; i++) {
		if (objectTable[i].costume != -2) {	// flag is keep ?
			Common::MemoryReadStream readS(ptr, entrySize);

			objectTable[i].x = readS.readSint16BE();
			objectTable[i].y = readS.readSint16BE();
			objectTable[i].mask = readS.readUint16BE();
			objectTable[i].frame = readS.readSint16BE();
			objectTable[i].costume = readS.readSint16BE();
			readS.read(objectTable[i].name, 20);
			objectTable[i].part = readS.readUint16BE();
		}
		ptr += entrySize;
	}

	if (!strcmp(pObjectName, "INTRO.OBJ")) {
		for (i = 0; i < 10; i++) {
			objectTable[i].costume = 0;
		}
	}

	free(dataPtr);
}

/*! \brief Remove overlay sprite from the list
 * \param objIdx Remove overlay associated with this object
 * \param param Remove overlay of this type
 */
int removeOverlay(uint16 objIdx, uint16 param) {
	Common::List<overlay>::iterator it;

	for (it = overlayList.begin(); it != overlayList.end(); ++it) {
		if (it->objIdx == objIdx && it->type == param) {
			overlayList.erase(it);
			return 1;
		}
	}

	return 0;
}

/*! \brief Add new overlay sprite to the list
 * \param objIdx Associate the overlay with this object
 * \param param Type of new overlay
 * \todo Why are x, y, width and color left uninitialized?
 */
void addOverlay(uint16 objIdx, uint16 param) {
	Common::List<overlay>::iterator it;
	overlay tmp;

	for (it = overlayList.begin(); it != overlayList.end(); ++it) {
		if (objectTable[it->objIdx].mask >= objectTable[objIdx].mask) {
			break;
		}
	}

	tmp.objIdx = objIdx;
	tmp.type = param;

	overlayList.insert(it, tmp);
}

/*! \brief Add new background mask overlay
 * \param objIdx Associate the overlay with this object
 * \param param source background index
 */
void addGfxElementA0(int16 objIdx, int16 param) {
	Common::List<overlay>::iterator it;
	overlay tmp;

	for (it = overlayList.begin(); it != overlayList.end(); ++it) {
		// wtf?!
		if (objectTable[it->objIdx].mask == objectTable[objIdx].mask &&
			(it->type == 2 || it->type == 3)) {
			break;
		}
	}

	if (it != overlayList.end() && it->objIdx == objIdx && it->type == 20 && it->x == param) {
		return;
	}

	tmp.objIdx = objIdx;
	tmp.type = 20;
	tmp.x = param;
	tmp.y = 0;
	tmp.width = 0;
	tmp.color = 0;

	overlayList.insert(it, tmp);
}

/*! \brief Remove background mask overlay
 * \param objIdx Remove overlay associated with this object
 * \param param Remove overlay using this background
 * \todo Check that it works
 */
void removeGfxElementA0(int16 objIdx, int16 param) {
	Common::List<overlay>::iterator it;

	for (it = overlayList.begin(); it != overlayList.end(); ++it) {
		if (it->objIdx == objIdx && it->type == 20 && it->x == param) {
			overlayList.erase(it);
			return;
		}
	}
}

void setupObject(byte objIdx, uint16 param1, uint16 param2, uint16 param3, uint16 param4) {
	objectTable[objIdx].x = param1;
	objectTable[objIdx].y = param2;
	objectTable[objIdx].mask = param3;
	objectTable[objIdx].frame = param4;

	if (removeOverlay(objIdx, 0)) {
		addOverlay(objIdx, 0);
	}
}

void subObjectParam(byte objIdx, byte paramIdx, int16 newValue) {
	addObjectParam(objIdx, paramIdx, -newValue);
}

void addObjectParam(byte objIdx, byte paramIdx, int16 newValue) {
	int16 currentValue = getObjectParam(objIdx, paramIdx);
	modifyObjectParam(objIdx, paramIdx, currentValue + newValue);
}

void modifyObjectParam(byte objIdx, byte paramIdx, int16 newValue) {
	paramIdx--;

	assert(paramIdx <= 5);

	switch (paramIdx) {
	case 0:
		objectTable[objIdx].x = newValue;
		break;
	case 1:
		objectTable[objIdx].y = newValue;
		break;
	case 2:
		objectTable[objIdx].mask = newValue;

		if (removeOverlay(objIdx, 0)) {
			addOverlay(objIdx, 0);
		}
		break;
	case 3:
		objectTable[objIdx].frame = newValue;
		break;
	case 4:
		// is it really in Future Wars? it breaks the newspaper machine
		// on the airport in Operation Stealth
		if (newValue == -1 && g_cine->getGameType() != Cine::GType_OS) {
			objectTable[objIdx].costume = globalVars[0];
		} else {
			objectTable[objIdx].costume = newValue;
		}
		break;
	case 5:
		objectTable[objIdx].part = newValue;
		break;
	}
}

uint16 compareObjectParam(byte objIdx, byte type, int16 value) {
	uint16 compareResult = 0;
	int16 objectParam = getObjectParam(objIdx, type);

	if (objectParam > value) {
		compareResult |= kCmpGT;
	} else if (objectParam < value) {
		compareResult |= kCmpLT;
	} else {
		compareResult |= kCmpEQ;
	}

	return compareResult;
}

int16 getObjectParam(uint16 objIdx, uint16 paramIdx) {
	assert(objIdx <= NUM_MAX_OBJECT);

	paramIdx--;

	assert(paramIdx <= 5);

	switch (paramIdx) {
	case 0:
		return objectTable[objIdx].x;
	case 1:
		return objectTable[objIdx].y;
	case 2:
		return objectTable[objIdx].mask;
	case 3:
		return objectTable[objIdx].frame;
	case 4:
		return objectTable[objIdx].costume;
	case 5:
		return objectTable[objIdx].part;
	}

	return 0;
}

} // End of namespace Cine
