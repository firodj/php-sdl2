#ifndef HAVE_PHP_SDL_WINDOW
#define HAVE_PHP_SDL_WINDOW

#include "php.h"

#include <SDL2/SDL.h>

#include <classes/common.h>
#include <classes/exceptions.h>
#include <classes/window.h>

static zend_object_handlers php_sdl_window_handlers;

zend_class_entry *sdlWindow_ce;

static zend_object* php_sdl_window_create(zend_class_entry *ce)
{
	php_sdl_window_t *window = 
		(php_sdl_window_t*) ecalloc(1, sizeof(php_sdl_window_t) + zend_object_properties_size(ce));

	zend_object_std_init(&window->std, ce);
	object_properties_init(&window->std, ce);
	window->std.handlers = &php_sdl_window_handlers;

	return &window->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_sdl_window___construct_info, 0, 0, 5)
	ZEND_ARG_TYPE_INFO(0, title, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, x, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, w, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, h, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Window Window::__construct(string title, int x, int y, int w, int h[, int flags]) */
PHP_METHOD(Window, __construct) 
{
	php_sdl_window_t *window = php_sdl_window_fetch(getThis());

	zend_string *title;
    zend_long x;
    zend_long y;
    zend_long w;
    zend_long h;
    zend_long flags = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "Sllll|l", &title, &x, &y, &w, &h, &flags) != SUCCESS) {
		return;
	}

	window->w = SDL_CreateWindow(ZSTR_VAL(title), x, y, w, h, flags);

    if (window->w == NULL) {
        php_sdl_error(SDL_GetError());
    }
}
/* }}} */

/* {{{ */
const zend_function_entry php_sdl_window_methods[] = {
	PHP_ME(Window, __construct, php_sdl_window___construct_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */


PHP_MINIT_FUNCTION(SDL_Window)
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, SDL_NS, "Window", php_sdl_window_methods);

	sdlWindow_ce = zend_register_internal_class_ex(&ce, NULL);
	sdlWindow_ce->create_object = php_sdl_window_create;

	memcpy(&php_sdl_window_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	php_sdl_window_handlers.offset = XtOffsetOf(php_sdl_window_t, std);

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
