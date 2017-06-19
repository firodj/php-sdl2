#ifndef HAVE_PHP_SDL_RENDERERFLAGS_H
#define HAVE_PHP_SDL_RENDERERFLAGS_H

extern zend_class_entry *sdlRendererFlags_ce;

#define SDL_RENDERERFLAGS_CONST(name) zend_declare_class_constant_long(sdlRendererFlags_ce, ZEND_STRL(#name), SDL_RENDERER_##name);

PHP_MINIT_FUNCTION(SDL_RendererFlags);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
