#include "FTFont.h"




#include <vector>
#include <cassert>
#include <fstream>

#define STB_TRUETYPE_IMPLEMENTATION 
#include "../../external/stb/stb_truetype.h"

#include "core/Engine.h"
#include "core/Application.h"
#include "files/FileSystem.h"
#include "utils/Log.h"



namespace ouzel {
	ouzel::FTFont::FTFont() : BMFont()
	{
	}

	ouzel::FTFont::FTFont(const std::string & filename, int16_t pt, UTFChars flag) {
		
		if (!parseFont(filename, pt, flag))
		{
			Log(Log::Level::ERR) << "Failed to parse font " << filename;
		}

		kernCount = static_cast<uint16_t>(kern.size());

	}

	bool FTFont::parseFont(const std::string & filename, int16_t pt, UTFChars flag)
	{


		stbtt_fontinfo font;
		std::vector<unsigned char> data;

		std::string f = ouzel::sharedApplication->getFileSystem()->getPath(filename);

		ouzel::sharedApplication->getFileSystem()->readFile(f, data);

		stbtt_InitFont(&font, data.data(), stbtt_GetFontOffsetForIndex(data.data(), 0));

		float s = stbtt_ScaleForPixelHeight(&font, pt);

		int w, h, xoff, yoff;

		std::vector<uint16_t> glyphs;
		height = 0;
		width = 0;
		std::map<uint32_t, std::pair<Size2, std::vector<uint8_t>> > glyph_to_bitmap_data;

		if (flag && UTFChars::ASCII)
		{
			for (uint16_t i = 32; i < 127; i++)
			{
				glyphs.push_back(i);
			}
		}
		if (flag && UTFChars::ASCIIPLUS)
		{
			for (uint16_t i = 128; i < 256; i++)
			{
				glyphs.push_back(i);
			}
		}

		for (uint16_t c : glyphs)
		{

			unsigned char* bitmap = stbtt_GetCodepointBitmap(&font, s, s, c, &w, &h, &xoff, &yoff);
			for (uint16_t j : glyphs)
			{
				int kx = stbtt_GetCodepointKernAdvance(&font, j, c);
				if (kx == 0) continue;
				kern.emplace(std::pair<uint32_t, uint32_t>(j, c), (int16_t)(kx * s));

			}
			int aw, lb;
			stbtt_GetCodepointHMetrics(&font, c, &aw, &lb);
			CharDescriptor nd;
			nd.xAdvance = (int16_t)(aw * s);
			nd.height = (int16_t)h;
			nd.width = (int16_t)w;
			nd.xOffset = (int16_t)(lb * s);
			nd.yOffset = (int16_t)(h - abs(yoff));

			std::vector<uint8_t> current_buffer(h * w);
			memcpy(current_buffer.data(), bitmap, h * w);

			glyph_to_bitmap_data.emplace(c, std::make_pair(Size2((float)w, (float)h), current_buffer));
			chars.emplace(c, nd);
			height = height < (int16_t)h ? (int16_t)h : height;
			width += (int16_t)w;
		}
		std::vector<std::vector<uint8_t> > scanlines(height, std::vector<uint8_t>());
		int x = 0;
		for (const auto &c : glyph_to_bitmap_data)
		{
			uint16_t char_height = (uint16_t)c.second.first.height();
			uint16_t char_width = (uint16_t)c.second.first.width();
			chars.at(c.first).x = (int16_t)x;
			chars.at(c.first).y = 0;
			x += char_width;

			uint16_t extra_rows = height - char_height;
			chars.at(c.first).yOffset += extra_rows;
			size_t extra_space_size = extra_rows * char_width;
			std::vector<uint8_t> new_char_buffer(extra_space_size + char_height * char_width, 0x00);
			memcpy(new_char_buffer.data(), c.second.second.data(), char_height * char_width);
			assert(new_char_buffer.size() == height * char_width);
			for (int i = 0; i < height; i++)
			{
				size_t scanlines_pre_size = scanlines[i].size();
				scanlines[i].resize(scanlines_pre_size + char_width);
				memcpy(scanlines[i].data() + scanlines_pre_size, new_char_buffer.data() + (int)char_width * i, char_width);
			}

		}
		std::vector<uint32_t> b1(scanlines[0].size() * height);
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < scanlines[0].size(); j++)
			{
				uint8_t b = scanlines[i][j];
				b1[i * scanlines[0].size() + j] = b << 24 | b << 16 | b << 8 | b;
			}

		}
		std::vector<uint8_t> b2(b1.size() * 4);
		memcpy(b2.data(), b1.data(), b1.size() * 4);
		sharedEngine->getCache()->getTextureFromData(filename, b2, Size2(width, height));
		texture = filename;
		pages = 1;
		lineHeight = pt;
		kernCount = static_cast<uint16_t>(kern.size());
		base = 0;
		return true;

			}
	}