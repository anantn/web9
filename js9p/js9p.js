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

JS9P = {}
JS9P.Base = function() {

	/* Globals */
	var keyStr = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	var buffer = [];

	/* Set the various 9P constants */
	var constants = function() {
		var ret = {};

		ret.version = "9P2000";
		ret.notag = 0xffff;
		ret.nofid = 0xffffffff;
	
		ret.MAX_VERSION = 32;
		ret.MAX_MSG = 8192;
		ret.MAX_ERROR = 128;
		ret.MAX_CACHE = 32;
		ret.MAX_FLEN = 128;
		ret.MAX_ULEN = 32;
		ret.MAX_WELEM = 16;

		/* From libc.h in p9p */
		ret.OREAD = 0;
		ret.OWRITE = 1;
		ret.ORDWR = 2;
		ret.OEXEC = 3;
		ret.OTRUNC = 16;
		ret.OCEXEC = 32;
		ret.ORCLOSE = 64;
		ret.ODIRECT = 128;
		ret.ONONBLOCK = 256;
		ret.OEXCL = 0x1000;
		ret.OLOCK = 0x2000;
		ret.OAPPEND = 0x4000;

		/* Bits for Qid */
		ret.QTDIR = 0x80;
		ret.QTAPPEND = 0x40;
		ret.QTEXCL = 0x20;
		ret.QTMOUNT = 0x10;
		ret.QTAUTH = 0x08;
		ret.QTTMP = 0x04;
		ret.QTSYMLINK = 0x02;
		ret.QTFILE = 0x00;

		/* Bits for Dir */
		ret.DMDIR = 0x80000000;
		ret.DMAPPEND = 0x40000000;	
		ret.DMEXCL = 0x20000000;	
		ret.DMMOUNT = 0x10000000;
		ret.DMAUTH = 0x08000000;	
		ret.DMTMP = 0x04000000;

		return ret;
	} ();

	/* Message types with codes */
	var messages = function() {
		var ret = {};
		var start = 100;

		var msg = ["version", "auth", "attach", "error", "flush", "walk", "open",
					"create", "read", "write", "clunk", "remove", "stat", "wstat"];
		for (var i = 0; i < msg.length; i++) {
			ret["T"+msg[i]] = start;
			ret["R"+msg[i]] = start + 1;
			start += 2;
		}
		return ret;
	} ();

	/* Message formats: n (n bytes), S (string, 2 byte length), D (string, 4 byte length), Q (qid) and {} (custom) 
	 * Inspired by py9p.
	 */
	var messageFormats = function() {
		var ret = {};
		var fmt = ["4S", "4S", "4SS", "Q", "", "S", "2", "", "44SS", "Q", "{Twalk}", "{Rwalk}", "41", "Q4",
					"484", "D", "48D", "4", "4", "", "4", "", "4", "{Stat}", "4{Stat}", ""];

		for (var i = 100, j = 0; (j < fmt.length - 1); i += 2, j += 2) {
			ret[i] = fmt[j];
			ret[i+1] = fmt[j+1];
		}
		return ret;
	} ();

	/* Base64 Encoding */
	function _encode64(input) {
		var output = "";
		var chr1, chr2, chr3;
		var enc1, enc2, enc3, enc4;var i = 0;
		
		do {
			chr1 = input.charCodeAt(i++);
			chr2 = input.charCodeAt(i++);
			chr3 = input.charCodeAt(i++);
			
			enc1 = chr1 >> 2;
			enc2 = ((chr1 & 3) << 4) | (chr2 >> 4);
			enc3 = ((chr2 & 15) << 2) | (chr3 >> 6);
			enc4 = chr3 & 63;

			if (isNaN(chr2)) {
				enc3 = enc4 = 64;
			} else if (isNaN(chr3)) {
				enc4 = 64;
			}

			output = output + keyStr.charAt(enc1) + keyStr.charAt(enc2) + keyStr.charAt(enc3) + keyStr.charAt(enc4);
		} while (i < input.length);

		return output;
	}

	function _decode64(input) {
		var output = "";
		var chr1, chr2, chr3;
		var enc1, enc2, enc3, enc4;
		var i = 0;

		// remove all characters that are not A-Z, a-z, 0-9, +, /, or =
		input = input.replace(/[^A-Za-z0-9\+\/\=]/g, "");
   
		do {
			enc1 = keyStr.indexOf(input.charAt(i++));
			enc2 = keyStr.indexOf(input.charAt(i++));
			enc3 = keyStr.indexOf(input.charAt(i++));
			enc4 = keyStr.indexOf(input.charAt(i++));
      
			chr1 = (enc1 << 2) | (enc2 >> 4);
			chr2 = ((enc2 & 15) << 4) | (enc3 >> 2);
			chr3 = ((enc3 & 3) << 6) | enc4;

			output = output + String.fromCharCode(chr1);
      
			if (enc3 != 64) {
				output = output + String.fromCharCode(chr2);
			}

			if (enc4 != 64) {
				output = output + String.fromCharCode(chr3);
			}
		} while (i < input.length);

		return output;
	}

	/* Set some buffer to given data */
	function _createBuffer(buf, data) {
		buf = []
		if (data) {
			var l = data.length;
			for(var i = l; i; buf[l - i] = data.charCodeAt(--i));
		}
	}

	/* Read bits from given buffer */
	function _readBits(buf, start, length) {
		
		/* shl fix */
		function shl(a, b){
			for(++b; --b; a = ((a %= 0x7fffffff + 1) & 0x40000000) == 0x40000000 ? a * 2 : (a - 0x40000000) * 2 + 0x7fffffff + 1);
			return a;
		}

		var givenBytes = -(start + length) >> 3;

		if (start < 0 || length <= 0) {
			alert("readBits::invalidArguments");
			return 0;
		}

		if (buf.length < -(givenBytes)) {
			alert("readBits::invalidDataArray");
			return 0;
		}

		var offsetLeft, offsetRight = start % 8;
		var curByte = buf.length - (start >> 3) - 1;
		var lastByte = buf.length + givenBytes;
		var diff = curByte - lastByte;

		sum = ((buf[curByte] >> offsetRight) & ((1 << (diff ? 8 - offsetRight : length)) - 1));
		if (diff && (offsetLeft = (start + length) % 8)) {
			sum += (buf[lastByte++] & ((1 << offsetLeft) -1)) << (diff-- << 3) - offsetRight;
		}

		while (diff)
			sum += shl(buf[lastByte++], (diff-- << 3) - offsetRight);

		return sum;
	}

	/* Encode an integer into 1, 2, 4 or 8 byte little-endian characters */
	function _encodeInt(num, bytes) {
		switch (bytes) {
			case 1:
			case 2:
			case 4:
			case 8:
				var bits = bytes * 8;
				break;
			default:
				var bits = bytes * 8;
				alert("encodeInt::invalidBytes");
				return;
		}

		var max = Math.pow(2, bits);
		var str = [];

		if ((num >= max) || num < -(max >> 1)) {
			alert("encodeInt::overflow");
			return;
		}

		if (num < 0) {
			num += max;
		}

		while(num) {
			str[str.length] = String.fromCharCode(num % 256);
			num = Math.floor(num / 256);
		}
		for(bits = -(-bits >> 3) - str.length; bits--; str[str.length] = "\0");

		return str.join("");
	};

	/* Decode a 1, 2, 4 or 8 byte string into an integer */
	function _decodeInt(data, bytes) {
		switch (bytes) {
			case 1:
			case 2:
			case 4:
			case 8:
				var bits = bytes * 8;
				break;
			default:
				var bits = bytes * 8;
				alert("encodeInt::invalidBytes");
				return;
		}

		var tmp;
		_createBuffer(tmp, data);
		dec = _readBits(tmp, 0, bits);
		max = Math.pow(2, bits);

		if (dec >= max/2) {
			return dec - max;
		} else {
			return dec;
		}
	}

	/* Set of methods to add message parts to the buffer
	 * Inpired by py9p
	 */

	// Encode a 1 byte integer
	function _enc1(val) {
		buffer[buffer.length] = _encodeInt(val, 1);
	}

	// Encode a 2 byte integer
	function _enc2(val) {
		buffer[buffer.length] = _encodeInt(val, 2);
	}

	// Encode a 4 byte integer
	function _enc4(val) {
		buffer[buffer.length] = _encodeInt(val, 4);
	}

	// Encode a 8 byte integer
	function _enc8(val) {
		buffer[buffer.length] = _encodeInt(val, 8);
	}

	// Encode a variable length string with 2 byte length
	function _encS(val) {
		_enc2(val.length);
		buffer[buffer.length] = val;
	}

	// Encode a variable length string with 4 byte length
	function _encD(val) {
		_enc4(val.length);
		buffer[buffer.length] = val;
	}

	// Encode a QID (13bit). qid = [type, version, path]
	function _encQ(qid) {
		_enc1(qid[0]);
		_enc2(qid[1]);
		_enc8(qid[2]);
	}

	// Encode a Twalk message. Argument is of the form: [fid, newfid, [name1, name2, ...]]
	function _encTwalk(args) {
		_enc4(args[0]);	
		_enc4(args[1]);	
		names = args[2];
		_enc2(names.length);	
		for (var i = 0; i < names.length; i++) {
			_encS(names[i]);
		}
	}

	// Encode an Rwalk message. qids is an array of Qids. Each qid = [type, version, path]
	function _encRwalk(qids) {
		_enc2(qids.length);
		for (var i = 0; i < qids.length; i++) {
			_encQ(qids[i]);
		}
	}

	// Encode an stat message. 
	// [type, dev, qid, mode, atime, mtime, len, name, uid, gid, muid]
	// qid itself is an array: [type, version, path]
	function _encStat(val) {
		size = 2 + 4 + 13 + 4 + 4 + 4 + 8 + name.length + uid.length + gid.length + muid.length;
		_enc2(size);
		_enc2(val[0]);
		_enc4(val[1]);
		_encQ(val[2]);
		_enc4(val[3]);
		_enc4(val[4]);
		_enc4(val[5]);
		_enc8(val[6]);
		_encS(val[7]);
		_encS(val[8]);
		_encS(val[9]);
		_encS(val[10]);
	}	

	// Check if message type is valid
	function _checkType(type) {
		if (messages[type]) {		
			return true;
		} else {
			return false;
		}
	}

	// Do encoding on the basis of format string
	function _encodeMessage(tag, type, args) {
		buffer = [];
		_enc1(type);
		_enc2(tag);
		fmt = messageFormats[type];
		var j = 0;

		/* FIXME: 
		 * How do I get the global object? 'this' doesn't seem to work 
		 * Each case can actually fall through and call this["_enc" + fmt[i]]()
		 */
		for (var i = 0; i < fmt.length; i++) {
			switch(fmt[i]) {
				case "1": _enc1(args[j]); break;
				case "2": _enc2(args[j]); break;
				case "4": _enc4(args[j]); break;
				case "8": _enc8(args[j]); break;
				case "S": _encS(args[j]); break;
				case "Q": _encQ(args[j]); break;
				case "{":
					var k = i + 1;
					var func = "";
					while (fmt[k] != "}") {
						func += fmt[k];	
						k++;
					}
					switch(func) {
						case "Twalk":	_encTwalk(args[j]); break;
						case "Rwalk":	_encRwalk(args[j]); break;
						case "Stat":	_encStat(args[j]); break;
					}
					i = k;
					break;
				default:
					alert("Invalid format type!");
					return false; break;
			}
			j++;
		}

		var len = 0;
		for (var i = 0; i < buffer.length; i++) {
			len += buffer[i].length;
		}
		var tmp = buffer;
		buffer = []
		_enc4(len);

		return _encode64(buffer.join("") + tmp.join(""));
	}

	// Decode a message
	function _decodeMessage(msg) {
		var buf = _decode64(msg);
		var size = _decodeInt(buf.slice(0, 4), 4);
		if (size != buf.length) {
			alert("decodeMessage: Invalid message size!");
			return false;
		}
		var type = _decodeInt(buf.slice(4, 5), 1);
		var tag = _decodeInt(buf.slice(5, 7), 2);
		var args = buf.slice(7);

		var fmt = messageFormats[type];
		for (var i = 0; i < fmt.length; i++) {
			
		}
	}

	return {
		constants: constants,
		msg: messageFormats,	
		createMessage: function(tag, type, args) {
			if (_checkType(type)) {
				return _encodeMessage(tag, messages[type], args);
			} else {
				alert("Not a valid type");
				return false;
			}
		}
	};

} ();
