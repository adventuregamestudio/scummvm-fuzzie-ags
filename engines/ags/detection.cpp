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

#include "base/plugins.h"
#include "engines/advancedDetector.h"

#include "ags/ags.h"

namespace AGS {

struct AGSGameDescription {
	ADGameDescription desc;
};

const char *AGSEngine::getGameId() const {
	return _gameDescription->desc.gameId;
}

Common::Language AGSEngine::getLanguage() const {
	return _gameDescription->desc.language;
}

Common::Platform AGSEngine::getPlatform() const {
	return _gameDescription->desc.platform;
}

bool AGSEngine::isDemo() const {
	return (_gameDescription->desc.flags & ADGF_DEMO) != 0;
}

const ADGameFileDescription *AGSEngine::getGameFiles() const {
	return _gameDescription->desc.filesDescriptions;
}

} // End of namespace AGS

static const PlainGameDescriptor AGSGames[] = {
	{"5days"       , "5 Days a Stranger" },
	{"7days"       , "7 Days a Skeptic"  },
	{"trilbysnotes", "Trilby's Notes"    },
	{"6days"       , "6 Days a Sacrifice"},
	{"ags"         , "Generic AGS game"  },
	{0             , 0                   }
};

#include "ags/detection_tables.h"

class AGSMetaEngine : public AdvancedMetaEngine {
public:
	AGSMetaEngine() : AdvancedMetaEngine(AGS::gameDescriptions, sizeof(AGS::AGSGameDescription), AGSGames) {
		_singleId = "ags";
		_guiOptions = GUIO1(GUIO_NOLAUNCHLOAD);
	}

	virtual const ADGameDescription *fallbackDetect(const FileMap &allFiles, const Common::FSList &fslist) const {
		return detectGameFilebased(allFiles, fslist, AGS::fileBased);
	}

	virtual const char *getName() const {
		return "Adventure Game Studio";
	}

	virtual const char *getOriginalCopyright() const {
		return "Copyright (C) Chris Jones";
	}

	virtual bool hasFeature(MetaEngineFeature f) const;
	virtual bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const;
};

bool AGSMetaEngine::hasFeature(MetaEngineFeature f) const {
	return false;
}

bool AGS::AGSEngine::hasFeature(EngineFeature f) const {
	return f == kSupportsRTL;
}

bool AGSMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	const AGS::AGSGameDescription *gd = (const AGS::AGSGameDescription *)desc;
	if (gd) {
		*engine = new AGS::AGSEngine(syst, gd);
		((AGS::AGSEngine *)*engine)->initGame(gd);
	}
	return gd != 0;
}

#if PLUGIN_ENABLED_DYNAMIC(AGS)
	REGISTER_PLUGIN_DYNAMIC(AGS, PLUGIN_TYPE_ENGINE, AGSMetaEngine);
#else
	REGISTER_PLUGIN_STATIC(AGS, PLUGIN_TYPE_ENGINE, AGSMetaEngine);
#endif

namespace AGS {

void AGSEngine::initGame(const AGSGameDescription *gd) {
}

} // End of namespace AGS
