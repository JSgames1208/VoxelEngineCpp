#pragma once
#include "engine/texture/TextureAtlas.h"

namespace TextureAtlasHelpers
{
	inline BlockUV makeUniformTile(const AtlasTile& tile) {
		return { tile, tile, tile, tile, tile, tile };
	}

	inline BlockUV makeTopBottomSide(const AtlasTile& top, const AtlasTile& bottom, const AtlasTile& side)
	{
		return { top, bottom, side, side, side, side };
	}

	inline BlockUV makeSide(const AtlasTile& top, const AtlasTile& side)
	{
		return { top, top, side, side, side, side };
	}
}