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

#include "base/plugins.h"
#include "common/debug.h"
#include "engines/advancedDetector.h"

#include "ags/ags.h"
#include "ags/constants.h"
#include "ags/resourceman.h"

namespace AGS {

struct AGSGameDescription {
	ADGameDescription desc;

	const char *title;
	const char *filename;
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

const char *AGSEngine::getDetectedGameFile() const {
	return _gameDescription->filename;
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

	virtual const ADGameDescription *fallbackDetect(const FileMap &allFiles, const Common::FSList &fslist) const;

	virtual const char *getName() const {
		return "Adventure Game Studio";
	}

	virtual const char *getOriginalCopyright() const {
		return "Copyright (C) 1999-2011 Chris Jones";
	}

	virtual bool hasFeature(MetaEngineFeature f) const;
	virtual bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const;

protected:
	virtual void updateGameDescriptor(GameDescriptor &desc, const ADGameDescription *realDesc) const;
};

static AGS::AGSGameDescription s_fallbackDesc = {
	{
	"",
	"",
	AD_ENTRY1(0, 0),
	Common::UNK_LANG,
	Common::kPlatformUnknown,
	ADGF_NO_FLAGS,
	GUIO0()
	},
	"",
	""
};
static char s_fallbackFilenameBuffer[21];
static char s_fallbackTitleBuffer[51];

const ADGameDescription *AGSMetaEngine::fallbackDetect(const FileMap &allFiles, const Common::FSList &fslist) const {
	const ADGameDescription *d = detectGameFilebased(allFiles, fslist, AGS::fileBased);
	if (d)
		return d;

	// reset fallback description
	AGS::AGSGameDescription *desc = &s_fallbackDesc;
	desc->desc.gameId = "ags";
	desc->desc.extra = "";
	desc->desc.language = Common::UNK_LANG;
	desc->desc.flags = ADGF_NO_FLAGS;
	desc->desc.platform = Common::kPlatformUnknown;
	desc->desc.guiOptions = GUIO0();
	desc->title = "";
	desc->filename = "";

	for (Common::FSList::const_iterator file = fslist.begin(); file != fslist.end(); ++file) {
		if (file->isDirectory())
			continue;

		Common::String filename = file->getName();
		filename.toLowercase();
		if (!filename.hasSuffix(".exe"))
			continue;

		SearchMan.clear();
		SearchMan.addDirectory(file->getParent().getName(), file->getParent());
		AGS::ResourceManager resourceManager;
		if (!resourceManager.init(filename))
			continue;

		Common::SeekableReadStream *dta = resourceManager.getFile(AGS::kGameDataNameV2);
		if (!dta) {
			// if no 2.x data, try 3.x
			dta = resourceManager.getFile(AGS::kGameDataNameV3);
			if (!dta) {
				debug(3, "AGS detection couldn't find game data file in '%s'", filename.c_str());
				continue;
			}
		}
		dta->skip(30 + 4); // signature + version
		uint32 versionStringLength = dta->readUint32LE();
		dta->skip(versionStringLength);

		char gameTitle[51];
		dta->read(gameTitle, 50);
		gameTitle[50] = '\0';
		delete dta;
		strncpy(s_fallbackTitleBuffer, gameTitle, 50);
		s_fallbackTitleBuffer[50] = '\0';
		desc->title = s_fallbackTitleBuffer;

		strncpy(s_fallbackFilenameBuffer, filename.c_str(), 20);
		s_fallbackFilenameBuffer[20] = '\0';
		desc->filename = s_fallbackFilenameBuffer;

		return (ADGameDescription *)desc;
	}

	return NULL;
}

void AGSMetaEngine::updateGameDescriptor(GameDescriptor &desc, const ADGameDescription *realDesc) const {
	AdvancedMetaEngine::updateGameDescriptor(desc, realDesc);

	// try to update title
	const AGS::AGSGameDescription *agsDesc = (const AGS::AGSGameDescription *)realDesc;
	if (!agsDesc->title[0])
		return;
	desc["description"] = agsDesc->title;
	desc.updateDesc("AGS");
}

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
