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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * MIT License:
 *
 * Copyright (c) 2009 Alexei Svitkine, Eugene Sandulenko
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "common/file.h"
#include "common/debug.h"
#include "common/debug-channels.h"
#include "common/config-manager.h"
#include "common/savefile.h"
#include "common/system.h"
#include "common/textconsole.h"
#include "common/translation.h"

#include "gui/saveload.h"

#include "graphics/thumbnail.h"
#include "graphics/surface.h"

#include "wage/wage.h"
#include "wage/world.h"
#include "wage/entities.h"

#define SAVEGAME_CURRENT_VERSION 1

//
// Version 0 (ScummVM):  first ScummVM version
//

namespace Wage {

static const uint32 AGIflag = MKTAG('W', 'A', 'G', 'E');

//TODO: make sure these are calculated right: (we add flag, description, etc)
#define VARS_INDEX 0x005E
#define SCENES_INDEX 0x0232

#define SCENE_SIZE 0x0010
#define CHR_SIZE 0x0016
#define OBJ_SIZE 0x0010

#define GET_HEX_OFFSET(ptr, baseOffset, entrySize) ((ptr) == nullptr ? -1 : ((baseOffset) + (entrySize) * (ptr)->_index))
#define GET_HEX_CHR_OFFSET(ptr) GET_HEX_OFFSET((ptr), chrsHexOffset, CHR_SIZE)
#define GET_HEX_OBJ_OFFSET(ptr) GET_HEX_OFFSET((ptr), objsHexOffset, OBJ_SIZE)
#define GET_HEX_SCENE_OFFSET(ptr) ((ptr) == nullptr ? -1 : (SCENES_INDEX + getSceneIndex(_world->_player->_currentScene) * SCENE_SIZE))

int WageEngine::getSceneIndex(Scene *scene) const {
	assert(scene);
	Common::Array<Scene *> &orderedScenes = _world->_orderedScenes;
	for (uint32 i = 0; i < orderedScenes.size(); ++i) {
		if (orderedScenes[i] == scene) return i;
	}

	warning("Scene's index not found");
	return -1;
}

int WageEngine::saveGame(const Common::String &fileName, const Common::String &descriptionString) {
	Common::OutSaveFile *out;
	int result = 0;

	debug(9, "WageEngine::saveGame(%s, %s)", fileName.c_str(), descriptionString.c_str());
	if (!(out = _saveFileMan->openForSaving(fileName))) {
		warning("Can't create file '%s', game not saved", fileName.c_str());
		return -1;
	} else {
		debug(9, "Successfully opened %s for writing", fileName.c_str());
	}

	out->writeUint32BE(AGIflag);

	// Write description of saved game, limited to WAGE_SAVEDGAME_DESCRIPTION_LEN characters + terminating NUL
	const int WAGE_SAVEDGAME_DESCRIPTION_LEN = 127;
	char description[WAGE_SAVEDGAME_DESCRIPTION_LEN + 1];

	memset(description, 0, sizeof(description));
	strncpy(description, descriptionString.c_str(), WAGE_SAVEDGAME_DESCRIPTION_LEN);
	assert(WAGE_SAVEDGAME_DESCRIPTION_LEN + 1 == 128); // safety
	out->write(description, 128);

	out->writeByte(SAVEGAME_CURRENT_VERSION);
	debug(9, "Writing save game version (%d)", SAVEGAME_CURRENT_VERSION);

	// Thumbnail
	Graphics::saveThumbnail(*out);

	// Counters
	out->writeSint16LE(_world->_scenes.size()); //numScenes
	out->writeSint16LE(_world->_chrs.size()); //numChars
	out->writeSint16LE(_world->_objs.size()); //numObjs

	// Hex Offsets
	int chrsHexOffset = SCENES_INDEX + _world->_scenes.size() * SCENE_SIZE; //chrs start after scenes
	int objsHexOffset = chrsHexOffset + _world->_chrs.size() * CHR_SIZE; //objs start after chrs
	out->writeSint32LE(chrsHexOffset);
	out->writeSint32LE(objsHexOffset);

	// Unique 8-byte World Signature
	out->writeSint32LE(0); //TODO: 8-byte ints? seriously?

	Chr *player = _world->_player;
	Context &playerContext = player->_context;

	// More Counters
	out->writeSint32LE(playerContext._visits); //visitNum
	out->writeSint32LE(0); //TODO: state.getLoopNum()
	out->writeSint32LE(playerContext._kills); //killNum

	// Hex offset to player character
	out->writeSint32LE(GET_HEX_CHR_OFFSET(player)); //getPlayerHexOffset() == getHexOffsetForChr(player)

	// character in this scene?
	out->writeSint32LE(GET_HEX_CHR_OFFSET(_monster)); //getPresCharHexOffset() == getHexOffsetForChr(monster)

	// Hex offset to current scene
	out->writeSint32LE(GET_HEX_SCENE_OFFSET(player->_currentScene)); //getCurSceneHexOffset()

	// wearing a helmet?
	out->writeSint32LE(GET_HEX_OBJ_OFFSET(player->_armor[Chr::ChrArmorType::HEAD_ARMOR])); //helmetIndex

	// holding a shield?
	out->writeSint32LE(GET_HEX_OBJ_OFFSET(player->_armor[Chr::ChrArmorType::SHIELD_ARMOR])); //shieldIndex

	// wearing chest armor?
	out->writeSint32LE(GET_HEX_OBJ_OFFSET(player->_armor[Chr::ChrArmorType::BODY_ARMOR])); //chestArmIndex

	// wearing spiritual armor?
	out->writeSint32LE(GET_HEX_OBJ_OFFSET(player->_armor[Chr::ChrArmorType::MAGIC_ARMOR])); //sprtArmIndex

	// TODO:
	out->writeSint16LE(0xffff);	// ???? - always FFFF
	out->writeSint16LE(0xffff);	// ???? - always FFFF
	out->writeSint16LE(0xffff);	// ???? - always FFFF
	out->writeSint16LE(0xffff);	// ???? - always FFFF

	// did a character just escape?
	out->writeSint32LE(GET_HEX_CHR_OFFSET(_running)); //getRunCharHexOffset() == getHexOffsetForChr(running)

	// players experience points
	out->writeSint32LE(playerContext._experience);

	out->writeSint16LE(0); //TODO: state.getAim()
	out->writeSint16LE(0); //TODO: state.getOpponentAim()

	// TODO:
	out->writeSint16LE(0x0000);	// always 0
	out->writeSint16LE(0x0000);	// always 0
	out->writeSint16LE(0x0000);	// always 0

	// Base character stats
	// TODO: are these *base* btw? looks like we don't want to save *current* stats
	out->writeByte(player->_physicalStrength); //state.getBasePhysStr()
	out->writeByte(player->_physicalHp); //state.getBasePhysHp()
	out->writeByte(player->_naturalArmor); //state.getBasePhysArm()
	out->writeByte(player->_physicalAccuracy); //state.getBasePhysAcc()
	out->writeByte(player->_spiritualStength); //state.getBaseSprtStr()
	out->writeByte(player->_spiritialHp); //state.getBaseSprtHp()
	out->writeByte(player->_resistanceToMagic); //state.getBaseSprtArm()
	out->writeByte(player->_spiritualAccuracy); //state.getBaseSprtAcc()
	out->writeByte(player->_runningSpeed); //state.getBaseRunSpeed()

	// TODO:
	out->writeByte(0x0A);		// ???? - always seems to be 0x0A

	// write user vars
	for (uint32 i = 0; i < 26 * 9; ++i)
		out->writeSint16LE(playerContext._userVariables[i]);
	
	// write updated info for all scenes
	Common::Array<Scene *> &orderedScenes = _world->_orderedScenes;
	for (uint32 i = 0; i < orderedScenes.size(); ++i) {
		Scene *scene = orderedScenes[i];
		if (scene != _world->_storageScene) {
			out->writeSint16LE(0); //TODO: scene.resourceId
			out->writeSint16LE(scene->_worldY);
			out->writeSint16LE(scene->_worldX);
			out->writeByte(scene->_blocked[NORTH] ? 0x01 : 0x00);
			out->writeByte(scene->_blocked[SOUTH] ? 0x01 : 0x00);
			out->writeByte(scene->_blocked[EAST] ? 0x01 : 0x00);
			out->writeByte(scene->_blocked[WEST] ? 0x01 : 0x00);
			out->writeSint16LE(scene->_soundFrequency);
			out->writeByte(scene->_soundType);
			// the following two bytes are currently unknown
			out->writeByte(0);
			out->writeByte(0);
			out->writeByte(scene->_visited ? 0x01 : 0x00);
		}
	}

	// write updated info for all characters
	Common::Array<Chr *> &orderedChrs = _world->_orderedChrs;
	for (uint32 i = 0; i < orderedChrs.size(); ++i) {
		Chr *chr = orderedChrs[i];
		out->writeSint16LE(0); //TODO: chr.getResourceID()
		out->writeSint16LE(0); //TODO: chr->_currentScene.getResourceID()
		//TODO: here we want to write *current* stats
		out->writeByte(chr->_physicalStrength);
		out->writeByte(chr->_physicalHp);
		out->writeByte(chr->_naturalArmor);
		out->writeByte(chr->_physicalAccuracy);
		out->writeByte(chr->_spiritualStength);
		out->writeByte(chr->_spiritialHp);
		out->writeByte(chr->_resistanceToMagic);
		out->writeByte(chr->_spiritualAccuracy);
		out->writeByte(chr->_runningSpeed);
		out->writeByte(chr->_rejectsOffers);
		out->writeByte(chr->_followsOpponent);
		// bytes 16-20 are unknown
		out->writeByte(0);
		out->writeByte(0);
		out->writeByte(0);
		out->writeByte(0);
		out->writeByte(0);
		out->writeByte(chr->_weaponDamage1);
		out->writeByte(chr->_weaponDamage2);
	}

	// write updated info for all objects
	Common::Array<Obj *> &orderedObjs = _world->_orderedObjs;
	for (uint32 i = 0; i < orderedObjs.size(); ++i) {
		Obj *obj = orderedObjs[i];
		Scene *location = obj->_currentScene;
		Chr *owner = obj->_currentOwner;

		out->writeSint16LE(0); //TODO: obj.getResourceID()
		out->writeSint16LE(0); //TODO: location == nullptr ? 0 : location.getResourceID());
		out->writeSint16LE(0); //TODO: owner == nullptr ? 0 : owner.getResourceID());

		// bytes 7-9 are unknown (always = 0)
		out->writeByte(0);
		out->writeByte(0);
		out->writeByte(0);

		out->writeByte(obj->_accuracy);
		out->writeByte(obj->_value);
		out->writeByte(obj->_type);
		out->writeByte(obj->_damage);
		out->writeByte(obj->_attackType);
		out->writeSint16LE(obj->_numberOfUses);
	}

	out->finalize();
	if (out->err()) {
		warning("Can't write file '%s'. (Disk full?)", fileName.c_str());
		result = -1;
	} else
		debug(9, "Saved game %s in file %s", descriptionString.c_str(), fileName.c_str());

	delete out;
	return result;
}

Common::String WageEngine::getSavegameFilename(int16 slotId) const {
	Common::String saveLoadSlot = _targetName;
	saveLoadSlot += Common::String::format(".%.3d", slotId);
	return saveLoadSlot;
}

Common::Error WageEngine::saveGameState(int slot, const Common::String &description) {
	Common::String saveLoadSlot = getSavegameFilename(slot);
	if (saveGame(saveLoadSlot, description) == 0)
		return Common::kNoError;
	else
		return Common::kUnknownError;
}

} // End of namespace Agi
