// Ouzel by Elviss Strazdins

#include <cstdlib>
#include "SystemIOS.hpp"
#include "EngineIOS.hpp"
#include "../../platform/foundation/AutoreleasePool.hpp"
#include "../../utils/Log.hpp"

int main(int argc, char* argv[])
{
    try
    {
        ouzel::platform::foundation::AutoreleasePool autoreleasePool;
        
        ouzel::core::ios::System system(argc, argv);

        ouzel::core::ios::Engine engine(argc, argv);
        engine.run(argc, argv);
        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        ouzel::logger.log(ouzel::Log::Level::error) << e.what();
        return EXIT_FAILURE;
    }
}

namespace ouzel::core::ios
{
    namespace
    {
        std::vector<std::string> parseArgs(int argc, char* argv[])
        {
            std::vector<std::string> result;
            for (int i = 0; i < argc; ++i)
                result.push_back(argv[i]);
            return result;
        }
    }

    System::System(int argc, char* argv[]):
        core::System{parseArgs(argc, argv)}
    {
    }
}
