/*
   +-----------------------------------------------------------------------+
   | Permission is hereby granted, free of charge, to any person obtaining |
   | a copy of this software and associated documentation files (the       |
   | "Software"), to deal in the Software without restriction, including   |
   | without limitation the rights to use, copy, modify, merge, publish,   |
   | distribute, sublicense, and/or sell copies of the Software, and to    |
   | permit persons to whom the Software is furnished to do so, subject to |
   | the following conditions:                                             |
   |                                                                       |
   | The above copyright notice and this permission notice shall be        |
   | included in all copies or substantial portions of the Software.	   |
   |                                                                       |
   | THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,	   |
   | EXPRESS OR	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    |
   | MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.|
   | IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  |
   | CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  |
   | TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     |
   | SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                |
   +-----------------------------------------------------------------------+
   | (C) 2007, Anant Narayanan <anant@kix.in>                              |
   +-----------------------------------------------------------------------+
*/

JS9P.XHR = function () {
	
	function _version() {
		return JS9P.Base.encodeMessage(JS9P.Base.constants["NOTAG"], "Tversion", [16*1024, JS9P.Base.constants["VERSION"]]);
	}

	function _attach(tag, fid, user, share) {
		return JS9P.Base.encodeMessage(tag, "Tattach", [fid, user, share]);
	}

	function _walk(tag, fid, newfid, path) {
		return JS9P.Base.encodeMessage(tag, "Twalk", [[fid, newfid, path]]);
	}

	function _stat(tag, fid) {
		return JS9P.Base.encodeMessage(tag, "Tstat", [fid]);
	}

	return {
		stat: function() {
			var msg = [_version(), _attach(1, 10, "js9p", ""), _walk(2, 10, 11, ""), _stat(3, 11)];
			return msg.join(",");
		}
	}

} ();
