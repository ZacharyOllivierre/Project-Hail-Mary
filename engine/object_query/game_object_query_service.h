#pragma once
#include "../core/game_object.h"
#include "../core/geometry/vector2.h"
#include "../tools/singleton.h"

namespace engine::object_query
{

    class GameObjectQueryService final
        : public engine::tools::Singleton<GameObjectQueryService>
    {
        friend engine::tools::Singleton<GameObjectQueryService>;
    public:
    };


}