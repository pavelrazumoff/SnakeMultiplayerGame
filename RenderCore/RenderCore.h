#pragma once

#ifdef RENDERCORE_EXPORTS
#define RENDERCORE_API __declspec(dllexport)
#else // RENDERCORE_EXPORTS
#define RENDERCORE_API __declspec(dllimport)
#endif // RENDERCORE_EXPORTS
