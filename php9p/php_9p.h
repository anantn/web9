/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2007 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header,v 1.16.2.1.2.1 2007/01/01 19:32:09 iliaa Exp $ */

#ifndef PHP_9P_H
#define PHP_9P_H

extern zend_module_entry _9p_module_entry;
#define phpext_9p_ptr &_9p_module_entry

#ifdef PHP_WIN32
#define PHP_9P_API __declspec(dllexport)
#else
#define PHP_9P_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(9p);
PHP_MSHUTDOWN_FUNCTION(9p);
PHP_RINIT_FUNCTION(9p);
PHP_RSHUTDOWN_FUNCTION(9p);
PHP_MINFO_FUNCTION(9p);

PHP_FUNCTION(confirm_9p_compiled);	/* For testing, remove later. */

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(9p)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(9p)
*/

/* In every utility function you add that needs to use variables 
   in php_9p_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as 9P_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define _9P_G(v) TSRMG(9p_globals_id, zend_9p_globals *, v)
#else
#define _9P_G(v) (9p_globals.v)
#endif

#endif	/* PHP_9P_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
