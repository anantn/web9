/*
   +----------------------------------------------------------------------+
   | This library is free software; you can redistribute it and/or        |
   | modify it under the terms of the GNU Lesser General Public           |
   | License as published by the Free Software Foundation; either         |
   | version 2.1 of the License, or (at your option) any later version.   | 
   |                                                                      |
   | This library is distributed in the hope that it will be useful,      |
   | but WITHOUT ANY WARRANTY; without even the implied warranty of       |
   | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU    |
   | Lesser General Public License for more details.                      | 
   |                                                                      |
   | You should have received a copy of the GNU Lesser General Public     |
   | License in the file LICENSE along with this library;                 |
   | if not, write to the                                                 | 
   |                                                                      |
   |   Free Software Foundation, Inc.,                                    |
   |   59 Temple Place, Suite 330,                                        |
   |   Boston, MA  02111-1307  USA                                        |
   +----------------------------------------------------------------------+
   | Authors: Anant Narayanan <anant@php.net>                             |
   +----------------------------------------------------------------------+
*/

/* $ Id: $ */ 

#ifndef PHP_IXP_H
#define PHP_IXP_H

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>

#ifdef HAVE_IXP

#include <php_ini.h>
#include <SAPI.h>
#include <ext/standard/info.h>
#include <Zend/zend_extensions.h>
#ifdef  __cplusplus
} // extern "C" 
#endif
#ifdef  __cplusplus
extern "C" {
#endif

extern zend_module_entry ixp_module_entry;
#define phpext_ixp_ptr &ixp_module_entry

#ifdef PHP_WIN32
#define PHP_IXP_API __declspec(dllexport)
#else
#define PHP_IXP_API
#endif

PHP_MINIT_FUNCTION(ixp);
PHP_MSHUTDOWN_FUNCTION(ixp);
PHP_RINIT_FUNCTION(ixp);
PHP_RSHUTDOWN_FUNCTION(ixp);
PHP_MINFO_FUNCTION(ixp);

#ifdef ZTS
#include "TSRM.h"
#endif

#define FREE_RESOURCE(resource) zend_list_delete(Z_LVAL_P(resource))

#define PROP_GET_LONG(name)    Z_LVAL_P(zend_read_property(_this_ce, _this_zval, #name, strlen(#name), 1 TSRMLS_CC))
#define PROP_SET_LONG(name, l) zend_update_property_long(_this_ce, _this_zval, #name, strlen(#name), l TSRMLS_CC)

#define PROP_GET_DOUBLE(name)    Z_DVAL_P(zend_read_property(_this_ce, _this_zval, #name, strlen(#name), 1 TSRMLS_CC))
#define PROP_SET_DOUBLE(name, d) zend_update_property_double(_this_ce, _this_zval, #name, strlen(#name), d TSRMLS_CC)

#define PROP_GET_STRING(name)    Z_STRVAL_P(zend_read_property(_this_ce, _this_zval, #name, strlen(#name), 1 TSRMLS_CC))
#define PROP_GET_STRLEN(name)    Z_STRLEN_P(zend_read_property(_this_ce, _this_zval, #name, strlen(#name), 1 TSRMLS_CC))
#define PROP_SET_STRING(name, s) zend_update_property_string(_this_ce, _this_zval, #name, strlen(#name), s TSRMLS_CC)
#define PROP_SET_STRINGL(name, s, l) zend_update_property_string(_this_ce, _this_zval, #name, strlen(#name), s, l TSRMLS_CC)


PHP_METHOD(IxpClient, __construct);
ZEND_BEGIN_ARG_INFO(IxpClient____construct_args, 0)
ZEND_END_ARG_INFO()
PHP_METHOD(IxpClient, unmount);
ZEND_BEGIN_ARG_INFO(IxpClient__unmount_args, 0)
ZEND_END_ARG_INFO()
PHP_METHOD(IxpClient, create);
ZEND_BEGIN_ARG_INFO(IxpClient__create_args, 0)
  ZEND_ARG_INFO(0, name)
  ZEND_ARG_INFO(0, permissions)
  ZEND_ARG_INFO(0, mode)
ZEND_END_ARG_INFO()
PHP_METHOD(IxpClient, open);
ZEND_BEGIN_ARG_INFO(IxpClient__open_args, 0)
  ZEND_ARG_INFO(0, name)
  ZEND_ARG_INFO(0, mode)
ZEND_END_ARG_INFO()
PHP_METHOD(IxpClient, remove);
ZEND_BEGIN_ARG_INFO(IxpClient__remove_args, 0)
  ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()
PHP_METHOD(IxpClient, stat);
ZEND_BEGIN_ARG_INFO(IxpClient__stat_args, 0)
  ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()
PHP_METHOD(IxpCFid, read);
ZEND_BEGIN_ARG_INFO(IxpCFid__read_args, 0)
  ZEND_ARG_INFO(0, buffer)
  ZEND_ARG_INFO(0, count)
ZEND_END_ARG_INFO()
PHP_METHOD(IxpCFid, write);
ZEND_BEGIN_ARG_INFO(IxpCFid__write_args, 0)
  ZEND_ARG_INFO(0, buffer)
ZEND_END_ARG_INFO()
PHP_METHOD(IxpCFid, close);
ZEND_BEGIN_ARG_INFO(IxpCFid__close_args, 0)
ZEND_END_ARG_INFO()
PHP_METHOD(IxpConn, serve);
ZEND_BEGIN_ARG_INFO(IxpConn__serve_args, 0)
ZEND_END_ARG_INFO()
PHP_METHOD(IxpRequest, respond);
ZEND_BEGIN_ARG_INFO(IxpRequest__respond_args, 0)
  ZEND_ARG_INFO(0, error)
ZEND_END_ARG_INFO()
PHP_METHOD(IxpServer, __construct);
ZEND_BEGIN_ARG_INFO(IxpServer____construct_args, 0)
  ZEND_ARG_INFO(0, IxpCallbacks)
  ZEND_ARG_INFO(0, IxpServerCallbacks)
ZEND_END_ARG_INFO()
PHP_METHOD(IxpServer, start);
ZEND_BEGIN_ARG_INFO(IxpServer__start_args, 0)
ZEND_END_ARG_INFO()
PHP_METHOD(IxpServer, stop);
ZEND_BEGIN_ARG_INFO(IxpServer__stop_args, 0)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO(IxpCallbacks__attach_args, 0)
  ZEND_ARG_INFO(0, IxpRequest)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO(IxpCallbacks__clunk_args, 0)
  ZEND_ARG_INFO(0, IxpRequest)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO(IxpCallbacks__create_args, 0)
  ZEND_ARG_INFO(0, IxpRequest)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO(IxpCallbacks__flush_args, 0)
  ZEND_ARG_INFO(0, IxpRequest)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO(IxpCallbacks__open_args, 0)
  ZEND_ARG_INFO(0, IxpRequest)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO(IxpCallbacks__read_args, 0)
  ZEND_ARG_INFO(0, IxpRequest)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO(IxpCallbacks__remove_args, 0)
  ZEND_ARG_INFO(0, IxpRequest)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO(IxpCallbacks__stat_args, 0)
  ZEND_ARG_INFO(0, IxpRequest)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO(IxpCallbacks__walk_args, 0)
  ZEND_ARG_INFO(0, IxpRequest)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO(IxpCallbacks__write_args, 0)
  ZEND_ARG_INFO(0, IxpRequest)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO(IxpCallbacks__freefid_args, 0)
  ZEND_ARG_INFO(0, IxpFid)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO(IxpServerCallbacks__read_args, 0)
  ZEND_ARG_INFO(0, IxpConn)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO(IxpServerCallbacks__close_args, 0)
  ZEND_ARG_INFO(0, IxpConn)
ZEND_END_ARG_INFO()
#ifdef  __cplusplus
} // extern "C" 
#endif

#endif /* PHP_HAVE_IXP */

#endif /* PHP_IXP_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
