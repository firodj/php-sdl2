#ifndef HAVE_PHP_SDL_CONST_H
#define HAVE_PHP_SDL_CONST_H

extern zend_class_entry *sdlEventType_ce;
extern zend_class_entry *sdlPixelFormat_ce;
extern zend_class_entry *sdlRendererFlags_ce;
extern zend_class_entry *sdlRendererFlip_ce;
extern zend_class_entry *sdlTextureAccess_ce;
extern zend_class_entry *sdlWindowEvent_ce;
extern zend_class_entry *sdlWindowFlags_ce;
extern zend_class_entry *sdlWindowPos_ce;

#define SDL_WINDOWEVENT_CONST(name) zend_declare_class_constant_long(sdlWindowEvent_ce, ZEND_STRL(#name), SDL_WINDOWEVENT_##name);
#define SDL_WINDOWFLAGS_CONST(name) zend_declare_class_constant_long(sdlWindowFlags_ce, ZEND_STRL(#name), SDL_WINDOW_##name);
#define SDL_TEXTUREACCESS_CONST(name) zend_declare_class_constant_long(sdlTextureAccess_ce, ZEND_STRL(#name), SDL_TEXTUREACCESS_##name);
#define SDL_WINDOWPOS_CONST(name) zend_declare_class_constant_long(sdlWindowPos_ce, ZEND_STRL(#name), SDL_WINDOWPOS_##name);
#define SDL_RENDERERFLAGS_CONST(name) zend_declare_class_constant_long(sdlRendererFlags_ce, ZEND_STRL(#name), SDL_RENDERER_##name);
#define SDL_RENDERERFLIP_CONST(name) zend_declare_class_constant_long(sdlRendererFlip_ce, ZEND_STRL(#name), SDL_FLIP_##name);
#define SDL_EVENTTYPE_CONST(name) zend_declare_class_constant_long(sdlEventType_ce, ZEND_STRL(#name), SDL_##name);
#define SDL_PIXELFORMAT_CONST(name) zend_declare_class_constant_long(sdlPixelFormat_ce, ZEND_STRL(#name), SDL_PIXELFORMAT_##name);

PHP_MINIT_FUNCTION(SDL_Const);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

