#ifndef HAVE_PHP_SDL_WINDOW
#define HAVE_PHP_SDL_WINDOW

#include "php.h"

#include <SDL2/SDL.h>

#include <classes/common.h>
#include <classes/exceptions.h>
#include <classes/renderer.h>
#include <classes/window.h>

static zend_object_handlers php_sdl_renderer_handlers;

zend_class_entry *sdlRenderer_ce;

static zend_object* php_sdl_renderer_create(zend_class_entry *ce)
{
	php_sdl_renderer_t *renderer = 
		(php_sdl_renderer_t*) ecalloc(1, sizeof(php_sdl_renderer_t) + zend_object_properties_size(ce));

	zend_object_std_init(&renderer->std, ce);
	object_properties_init(&renderer->std, ce);
	renderer->std.handlers = &php_sdl_renderer_handlers;

	return &renderer->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_sdl_renderer___construct_info, 0, 0, 3)
	ZEND_ARG_OBJ_INFO(0, window, SDL\\Window, 0)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Renderer Renderer::__construct(Window window, int index, int flags) */
PHP_METHOD(Renderer, __construct) 
{
	php_sdl_renderer_t *renderer = php_sdl_renderer_fetch(getThis());
    php_sdl_window_t *w;

	zval *window;
    zend_long index;
    zend_long flags;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "Oll", &window, sdlWindow_ce, &index, &flags) != SUCCESS) {
		return;
	}

	w = php_sdl_window_fetch(window);

    renderer->r = SDL_CreateRenderer(w->w, index, flags);

    if (renderer->r == NULL) {
        php_sdl_error(SDL_GetError());
    }
}
/* }}} */

/* {{{ */
const zend_function_entry php_sdl_renderer_methods[] = {
	PHP_ME(Renderer, __construct, php_sdl_renderer___construct_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */


PHP_MINIT_FUNCTION(SDL_Renderer)
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, SDL_NS, "Renderer", php_sdl_renderer_methods);

	sdlRenderer_ce = zend_register_internal_class_ex(&ce, NULL);
	sdlRenderer_ce->create_object = php_sdl_renderer_create;

	memcpy(&php_sdl_renderer_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	php_sdl_renderer_handlers.offset = XtOffsetOf(php_sdl_renderer_t, std);

	return SUCCESS;
}

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
