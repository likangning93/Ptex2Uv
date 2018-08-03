set(SOURCE_FILES
    "buildGraph.hpp"
    "buildGraph.cpp"
    "buildPatches.hpp"
    "buildPatches.cpp"
    "Quad.hpp"
    "QuadPatch.hpp"
    )

add_library(src
    ${SOURCE_FILES}
    )
