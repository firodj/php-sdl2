#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_sdl2.h"

#include <SDL2/SDL.h>

#include <src/exceptions.h>
#include <src/sdl.h>

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(sdl2)
{
	PHP_MINIT(SDL_Exceptions)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(SDL_SDL)(INIT_FUNC_ARGS_PASSTHRU);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(sdl2)
{
#if defined(COMPILE_DL_SDL2) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* {{{ sdl2_functions[] */
const zend_function_entry sdl2_functions[] = {
	SDL_SDL_FE
	PHP_FE_END
};
/* }}} */

/* {{{ sdl2_module_entry */
zend_module_entry sdl2_module_entry = {
	STANDARD_MODULE_HEADER,
	"SDL2",
	sdl2_functions,
	PHP_MINIT(sdl2),
	NULL,
	PHP_RINIT(sdl2),
	NULL,
	NULL,
	PHP_SDL2_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SDL2
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(sdl2)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
