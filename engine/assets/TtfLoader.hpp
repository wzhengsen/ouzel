// Ouzel by Elviss Strazdins

#ifndef OUZEL_ASSETS_TTFLOADER_HPP
#define OUZEL_ASSETS_TTFLOADER_HPP

#include "Loader.hpp"
#include "Bundle.hpp"
#include "../gui/TTFont.hpp"
#include "stb_truetype.h"

namespace ouzel::assets
{
    class TtfLoader final: public Loader
    {
    public:
        explicit TtfLoader(): Loader{Asset::Type::font} {}

        bool loadAsset(Cache&,
                       Bundle& bundle,
                       const std::string& name,
                       const std::vector<std::byte>& data,
                       const Asset::Options& options) override
        {
            try
            {
                // TODO: move the loader here
                auto font = std::make_unique<gui::TTFont>(data, options.mipmaps);
                bundle.setFont(name, std::move(font));
            }
            catch (const std::exception&)
            {
                return false;
            }

            return true;
        }
    };
}

#endif // OUZEL_ASSETS_TTFLOADER_HPP
