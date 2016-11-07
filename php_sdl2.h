#ifndef PHP_SDL2_H
#define PHP_SDL2_H

#define SDL_NS "SDL"
#define SDL_FE(name) ZEND_NS_NAMED_FE(SDL_NS, name, zif_SDL_##name, php_sdl_##name##_info)
#define SDL_LONG_CONSTANT(name) REGISTER_NS_LONG_CONSTANT(SDL_NS, #name, SDL_##name, CONST_CS | CONST_PERSISTENT)

extern zend_module_entry sdl2_module_entry;
#define phpext_sdl2_ptr &sdl2_module_entry

#define PHP_SDL2_VERSION "0.0.0"

#ifdef PHP_WIN32
#	define PHP_SDL2_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_SDL2_API __attribute__ ((visibility("default")))
#else
#	define PHP_SDL2_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#if defined(ZTS) && defined(COMPILE_DL_SDL2)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
