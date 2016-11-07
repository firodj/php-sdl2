#ifndef HAVE_PHP_SDL_COMMON_H
#define HAVE_PHP_SDL_COMMON_H

#define SDL_NS "SDL"

#define SDL_FE(name) ZEND_NS_NAMED_FE(SDL_NS, name, zif_SDL_##name, php_sdl_##name##_info)
#define SDL_FUNC(name) PHP_FUNCTION(SDL_##name)

#define SDL_CONST_LONG(name) REGISTER_NS_LONG_CONSTANT(SDL_NS, #name, SDL_##name, CONST_CS | CONST_PERSISTENT)

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
