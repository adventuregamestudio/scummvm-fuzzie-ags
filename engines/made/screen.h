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

#ifndef MADE_SCREEN_H
#define MADE_SCREEN_H

#include "common/endian.h"
#include "common/util.h"

#include "graphics/surface.h"

#include "made/resource.h"

namespace Made {

struct SpriteChannel {
	int16 type;
	int16 state;
	int16 needRefresh;
	uint16 index;
	int16 x, y;
	int16 x1, y1, x2, y2;
	uint32 area;
	uint16 fontNum;
	int16 textColor, outlineColor;
	int16 frameNum;
	int16 mask;
};

struct ClipInfo {
	uint16 x, y, w, h;
	Graphics::Surface *destSurface;
};

class MadeEngine;

class Screen {
public:
	Screen(MadeEngine *vm);
	~Screen();

	void clearScreen();
	
	void drawSurface(Graphics::Surface *sourceSurface, int x, int y, int16 flipX, int16 flipY, int16 mask, const ClipInfo &clipInfo);
	
	void loadRGBPalette(byte *palRGB, int count = 256);
	void setRGBPalette(byte *palRGB, int start = 0, int count = 256);
	bool isPaletteLocked() { return _paletteLock; }
	void setScreenLock(bool lock) { _screenLock = lock; }
	void setPaletteLock(bool lock) { _paletteLock = lock; }
	void setVisualEffectNum(int visualEffectNum) { _visualEffectNum = visualEffectNum; }

	void setClipArea(uint16 x1, uint16 y1, uint16 x2, uint16 y2) { 
		_clipArea.x = x1;
		_clipArea.y = y1;
		_clipArea.w = ABS(x2 - x1);
		_clipArea.h = ABS(y2 - y1);
	}

	void setExcludeArea(uint16 x1, uint16 y1, uint16 x2, uint16 y2);

	void setClip(int16 clip) { _clip = clip; }
	void setExclude(int16 exclude) { _exclude = exclude; }
	void setGround(int16 ground) { _ground = ground; }
	void setMask(int16 mask) { _mask = mask; }

	void setTextColor(int16 color) { _textColor = color; }

	void setTextRect(const Common::Rect &textRect) {
		_textRect = textRect;
		_textX = _textRect.left;
		_textY = _textRect.top;
	}

	void getTextRect(Common::Rect &textRect) {
		textRect = _textRect;
	}

	void setOutlineColor(int16 color) {
		_outlineColor = color;
		_dropShadowColor = -1;
	}

	void setDropShadowColor(int16 color) { 
		_outlineColor = -1;
		_dropShadowColor = color;
	}
	
	void setTextXY(int16 x, int16 y) {
		_textX = x;
		_textY = y;
	}
	
	uint16 updateChannel(uint16 channelIndex);
	void deleteChannel(uint16 channelIndex);
	int16 getChannelType(uint16 channelIndex);
	int16 getChannelState(uint16 channelIndex);
	void setChannelState(uint16 channelIndex, int16 state);
	uint16 setChannelLocation(uint16 channelIndex, int16 x, int16 y);
	uint16 setChannelContent(uint16 channelIndex, uint16 index);
	void setChannelUseMask(uint16 channelIndex);
	void drawSpriteChannels(const ClipInfo &clipInfo, int16 includeStateMask, int16 excludeStateMask);
	void updateSprites();
	void clearChannels();
	
	uint16 drawFlex(uint16 flexIndex, int16 x, int16 y, int16 flipX, int16 flipY, int16 mask, const ClipInfo &clipInfo);
	void drawAnimFrame(uint16 animIndex, int16 x, int16 y, int16 frameNum, int16 flipX, int16 flipY, const ClipInfo &clipInfo);

	uint16 drawPic(uint16 index, int16 x, int16 y, int16 flipX, int16 flipY);
	uint16 drawMask(uint16 index, int16 x, int16 y);
	
	uint16 drawAnimPic(uint16 animIndex, int16 x, int16 y, int16 frameNum, int16 flipX, int16 flipY);
	
	void addSprite(uint16 spriteIndex);
	
	uint16 drawSprite(uint16 flexIndex, int16 x, int16 y);
	uint16 placeSprite(uint16 channelIndex, uint16 flexIndex, int16 x, int16 y);

	uint16 placeAnim(uint16 channelIndex, uint16 animIndex, int16 x, int16 y, int16 frameNum);
	int16 setAnimFrame(uint16 channelIndex, int16 frameNum);
	int16 getAnimFrame(uint16 channelIndex);
	// TODO: Move to script function
	int16 getAnimFrameCount(uint16 animIndex);

	uint16 placeText(uint16 channelIndex, uint16 textObjectIndex, int16 x, int16 y, uint16 fontNum, int16 textColor, int16 outlineColor);
	
	void show();
	void flash(int count);
	
	void setFont(int16 fontNum);
	void printChar(uint c, int16 x, int16 y, byte color);
	void printText(const char *text);
	void printTextEx(const char *text, int16 x, int16 y, int16 fontNum, int16 textColor, int16 outlineColor, const ClipInfo &clipInfo);
	void printObjectText(int16 objectIndex, int16 x, int16 y, int16 fontNum, int16 textColor, int16 outlineColor, const ClipInfo &clipInfo);
	int16 getTextWidth(int16 fontNum, const char *text);

protected:
	MadeEngine *_vm;
	
	bool _screenLock;
	bool _paletteLock;

	byte _screenPalette[256 * 4];
	byte _palette[768], _newPalette[768], _fxPalette[768];
	int _paletteColorCount, _oldPaletteColorCount;
	bool _paletteInitialized, _needPalette;
	int16 _textColor;
	int16 _outlineColor;
	int16 _dropShadowColor;

	int16 _textX, _textY;
	Common::Rect _textRect;
	int16 _currentFontNum;
	FontResource *_font;
	ClipInfo _fontDrawCtx;

	int16 _clip, _exclude, _ground, _mask;
	int _visualEffectNum;
	
	Graphics::Surface *_screen1, *_screen2, *_screenMask;
	ClipInfo _clipArea, _clipInfo1, _clipInfo2, _maskDrawCtx;
	
	ClipInfo _excludeClipArea[4];
	bool _excludeClipAreaEnabled[4];
	
	uint16 _channelsUsedCount;
	SpriteChannel _channels[100];
	
};

} // End of namespace Made

#endif /* MADE_H */
