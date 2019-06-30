// Copyright 2015-2019 Elviss Strazdins. All rights reserved.

#include "BmfLoader.hpp"
#include "Bundle.hpp"
#include "gui/BMFont.hpp"

namespace ouzel
{
    namespace assets
    {
        BmfLoader::BmfLoader(Cache& initCache):
            Loader(initCache, TYPE)
        {
        }

        bool BmfLoader::loadAsset(Bundle& bundle,
                                  const std::string& name,
                                  const std::vector<uint8_t>& data,
                                  bool)
        {
            try
            {
                // TODO: move the loader here
                auto font = std::make_shared<gui::BMFont>(data);
                bundle.setFont(name, font);
            }
            catch (const std::exception&)
            {
                return false;
            }

            return true;
        }
    } // namespace assets
} // namespace ouzel
