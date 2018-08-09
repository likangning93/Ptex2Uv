set(SOURCE_FILES
    "buildGraph.hpp"
    "buildGraph.cpp"
    "buildPatches.hpp"
    "buildPatches.cpp"
    "PackingGrid.hpp"
    "PackingGrid.cpp"
    "packPatches.hpp"
    "packPatches.cpp"
    "Quad.hpp"
    "QuadPatch.hpp"
    )

add_library(src
    ${SOURCE_FILES}
    )
