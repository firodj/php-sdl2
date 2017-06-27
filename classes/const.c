#include "php.h"
#include "zend_interfaces.h"

#ifdef _WIN32
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif

#include "./common.h"
#include "./const.h"

zend_class_entry *sdlEventType_ce;
zend_class_entry *sdlPixelFormat_ce;
zend_class_entry *sdlRendererFlags_ce;
zend_class_entry *sdlTextureAccess_ce;
zend_class_entry *sdlWindowEvent_ce;
zend_class_entry *sdlWindowFlags_ce;
zend_class_entry *sdlWindowPos_ce;

PHP_MINIT_FUNCTION(SDL_Const) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, SDL_NS, "EventType", NULL);
	sdlEventType_ce = zend_register_internal_class_ex(&ce, NULL);
	sdlEventType_ce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL;

	SDL_EVENTTYPE_CONST(KEYUP);
	SDL_EVENTTYPE_CONST(KEYDOWN);
	SDL_EVENTTYPE_CONST(WINDOWEVENT);
	SDL_EVENTTYPE_CONST(QUIT);
	SDL_EVENTTYPE_CONST(MOUSEMOTION);
	SDL_EVENTTYPE_CONST(MOUSEBUTTONDOWN);
	SDL_EVENTTYPE_CONST(MOUSEBUTTONUP);
	SDL_EVENTTYPE_CONST(MOUSEWHEEL);
	SDL_EVENTTYPE_CONST(FINGERMOTION);
	SDL_EVENTTYPE_CONST(FINGERDOWN);
	SDL_EVENTTYPE_CONST(FINGERUP);

	INIT_NS_CLASS_ENTRY(ce, SDL_NS, "PixelFormat", NULL);
	sdlPixelFormat_ce = zend_register_internal_class_ex(&ce, NULL);
	sdlPixelFormat_ce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL;

	SDL_PIXELFORMAT_CONST(ARGB8888);
	SDL_PIXELFORMAT_CONST(RGBA32);

	INIT_NS_CLASS_ENTRY(ce, SDL_NS, "RendererFlags", NULL);
	sdlRendererFlags_ce = zend_register_internal_class_ex(&ce, NULL);
	sdlRendererFlags_ce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL;

	SDL_RENDERERFLAGS_CONST(SOFTWARE);
	SDL_RENDERERFLAGS_CONST(ACCELERATED);
	SDL_RENDERERFLAGS_CONST(PRESENTVSYNC);
	SDL_RENDERERFLAGS_CONST(TARGETTEXTURE);

	INIT_NS_CLASS_ENTRY(ce, SDL_NS, "TextureAccess", NULL);
	sdlTextureAccess_ce = zend_register_internal_class_ex(&ce, NULL);
	sdlTextureAccess_ce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL;

	SDL_TEXTUREACCESS_CONST(STATIC);
	SDL_TEXTUREACCESS_CONST(STREAMING);
	SDL_TEXTUREACCESS_CONST(TARGET);

	INIT_NS_CLASS_ENTRY(ce, SDL_NS, "WindowEvent", NULL);
	sdlWindowEvent_ce = zend_register_internal_class_ex(&ce, NULL);
	sdlWindowEvent_ce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL;

	SDL_WINDOWEVENT_CONST(CLOSE);

	INIT_NS_CLASS_ENTRY(ce, SDL_NS, "WindowFlags", NULL);
	sdlWindowFlags_ce = zend_register_internal_class_ex(&ce, NULL);
	sdlWindowFlags_ce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL;

	SDL_WINDOWFLAGS_CONST(FULLSCREEN);
	SDL_WINDOWFLAGS_CONST(OPENGL);
	SDL_WINDOWFLAGS_CONST(SHOWN);
	SDL_WINDOWFLAGS_CONST(HIDDEN);
	SDL_WINDOWFLAGS_CONST(BORDERLESS);
	SDL_WINDOWFLAGS_CONST(RESIZABLE);
	SDL_WINDOWFLAGS_CONST(MINIMIZED);
	SDL_WINDOWFLAGS_CONST(MAXIMIZED);
	SDL_WINDOWFLAGS_CONST(INPUT_GRABBED);
	SDL_WINDOWFLAGS_CONST(INPUT_FOCUS);
	SDL_WINDOWFLAGS_CONST(MOUSE_FOCUS);
	SDL_WINDOWFLAGS_CONST(FULLSCREEN_DESKTOP);
	SDL_WINDOWFLAGS_CONST(FOREIGN);
	SDL_WINDOWFLAGS_CONST(ALLOW_HIGHDPI);
	SDL_WINDOWFLAGS_CONST(MOUSE_CAPTURE);
#if SDL_VERSION_ATLEAST(2,0,5)
	SDL_WINDOWFLAGS_CONST(ALWAYS_ON_TOP);
	SDL_WINDOWFLAGS_CONST(SKIP_TASKBAR);
	SDL_WINDOWFLAGS_CONST(UTILITY);
	SDL_WINDOWFLAGS_CONST(TOOLTIP);
	SDL_WINDOWFLAGS_CONST(POPUP_MENU);
#endif

	INIT_NS_CLASS_ENTRY(ce, SDL_NS, "WindowPos", NULL);
	sdlWindowPos_ce = zend_register_internal_class_ex(&ce, NULL);
	sdlWindowPos_ce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL;

	SDL_WINDOWPOS_CONST(UNDEFINED);
	SDL_WINDOWPOS_CONST(CENTERED);

	return SUCCESS;
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

