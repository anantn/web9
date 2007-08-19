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
	var encBigEndian = false;
	var encBase64 = true;
	var buffer = [];

	/* Set the various 9P constants */
	var constants = function() {
		var ret = {};

		ret.VERSION = "9P2000";
		ret.NOTAG = 0xffff;
		ret.NOFID = 0xffffffff;
	
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

	/* Message formats: n (n bytes), S (string, 2 byte length), D (string, 4 byte length), Q (qid) and {} (custom) */
	var messageFormats = function() {
		var ret = {};
		var fmt = ["4S", "4S", "4SS", "Q", "44SS", "Q", "", "S", "2", "", "{Twalk}", "{Rwalk}", "41", "Q4",
					"4S41", "Q4", "484", "D", "48D", "4", "4", "", "4", "", "4", "{Stat}", "4{Stat}", ""];

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
	function _createBuffer(data) {
		var buf = [];
		if (data) {
			var l = data.length;
			for(var i = l; i; buf[l - i] = data.charCodeAt(--i));
			if (encBigEndian) {
				buf.reverse();
			}
		}
		return buf;
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

	/* Encode an integer into 1, 2, 4 or 8 byte characters */
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

		if (encBigEndian) {
			return str.reverse();
		} 
		
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

		var tmp = _createBuffer(data);
		dec = _readBits(tmp, 0, bits);
		max = Math.pow(2, bits);

		if (dec >= max/2) {
			return dec - max;
		} else {
			return dec;
		}
	}

	/* Set of methods to encode/decode message parts to/from the buffer */
	function _enc1(val) {
		buffer[buffer.length] = _encodeInt(val, 1);
	}
	function _dec1(val, index) {
		buffer[buffer.length] = _decodeInt(val.slice(index, index + 1), 1);
		return index + 1;
	}

	function _enc2(val) {
		buffer[buffer.length] = _encodeInt(val, 2);
	}
	function _dec2(val) {
		buffer[buffer.length] = _decodeInt(val.slice(index, index + 2), 2);
		return index + 2;
	}

	function _enc4(val) {
		buffer[buffer.length] = _encodeInt(val, 4);
	}
	function _dec4(val, index) {
		buffer[buffer.length] = _decodeInt(val.slice(index, index + 4), 4);
		return index + 4;
	}

	function _enc8(val) {
		buffer[buffer.length] = _encodeInt(val, 8);
	}
	function _dec8(val, index) {
		buffer[buffer.length] = _decodeInt(val.slice(index, index + 8), 8);
		return index + 8;
	}

	// Encode a variable length string with 2 byte length
	function _encS(val) {
		_enc2(val.length);
		buffer[buffer.length] = val;
	}
	function _decS(val, index) {
		var len = _decodeInt(val.slice(index, index + 2), 2);
		buffer[buffer.length] = val.slice(index + 2, index + len + 2);
		return index + len + 2;
	}

	// Encode a variable length string with 4 byte length
	function _encD(val) {
		_enc4(val.length);
		buffer[buffer.length] = val;
	}
	function _decD(val, index) {
		var len = _decodeInt(val.slice(index, index + 4), 4);
		buffer[buffer.length] = val.slice(index + 4, index + len + 4);
		return index + len + 4;
	}

	// Encode a QID (13bit). qid = [type, version, path]
	function _encQ(qid) {
		_enc1(qid[0]);
		_enc2(qid[1]);
		_enc8(qid[2]);
	}
	function _decQ(val, index) {
		var type = _decodeInt(val.slice(index, index + 1), 1);
		var version = _decodeInt(val.slice(index + 1, index + 3), 2);
		var path = _decodeInt(val.slice(index + 3, index + 11), 8);

		buffer[buffer.length] = [type, version, path];
		return index + 13;
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
	function _decTwalk(val, index) {
		var fid = _decodeInt(val.slice(index, index + 4), 4);
		var newfid = _decodeInt(val.slice(index + 4, index + 8), 8);

		var len = _decodeInt(val.slice(index + 8, index + 10), 2);
		var tindex = index + 10;
		for (var i = 0; i < len; i++) {
			tindex = _decS(val, tindex);
		}

		var names = buffer.splice(buffer.length - len - 1, len);
		buffer[buffer.length] = [fid, newfid, names];
		return tindex;
	}

	// Encode an Rwalk message. qids is an array of Qids. Each qid = [type, version, path]
	function _encRwalk(qids) {
		_enc2(qids.length);
		for (var i = 0; i < qids.length; i++) {
			_encQ(qids[i]);
		}
	}
	function _decRwalk(val, index) {
		var len = _decodeInt(val.slice(index, index + 2), 2);
		var tindex = index + 2;
		for (var i = 0; i < len; i++) {
			tindex = _decQ(val, tindex);
		}

		var qids = buffer.splice(buffer.length - len - 1, len);
		buffer[buffer.length] = qids;
		return tindex;
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
	function _decStat(val, index) {
		var size = _decodeInt(val.slice(index, index + 2), 2);
		var type = _decodeInt(val.slice(index + 2, index + 4), 2);
		var dev = _decodeInt(val.slice(index + 4, index + 8), 4);
		index = _decQ(val, index + 8);
		var mode = _decodeInt(val.slice(index, index + 4), 4);
		var atime = _decodeInt(val.slice(index + 4, index + 8), 4);
		var mtime = _decodeInt(val.slice(index + 8, index + 12), 4);
		var length = _decodeInt(val.slice(index + 12, index + 20), 8);
		index = _decS(val, index + 20);
		index = _decS(val, index);
		index = _decS(val, index);
		index = _decS(val, index);
		var muid = buffer.splice(buffer.length - 1, 1);
		var gid = buffer.splice(buffer.length - 1, 1);
		var uid = buffer.splice(buffer.length - 1, 1);
		var name = buffer.splice(buffer.length - 1, 1);
		var qid = buffer.splice(buffer.length - 1, 1);
	
		buffer[buffer.length] = [type, dev, qid, mode, atime, mtime, length, name, uid, gid, muid];
		return index;
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
				case "D": _encD(args[j]); break;
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
		_enc4(len + 4);

		var ret = buffer.join("") + tmp.join("")
		if (encBase64) {
			ret = _encode64(ret);
		}

		return ret;
	}

	// Decode a message
	function _decodeMessage(msg) {
		buffer = [];
		var buf;
		if (encBase64) {
			buf = _decode64(msg);
		} else {
			buf = msg;
		}

		var size = _decodeInt(buf.slice(0, 4), 4);
		if (size != buf.length) {
			alert("decodeMessage: Invalid message size! " + buf.length + " found, " + size + " expected.");
			return false;
		}
		var type = _decodeInt(buf.slice(4, 5), 1);
		var tag = _decodeInt(buf.slice(5, 7), 2);
		var args = buf.slice(7);

		var fmt = messageFormats[type];
		for (var i = 0, j = 0; i < fmt.length; i++) {
			switch(fmt[i]) {
				case "1": j = _dec1(args, j); break;	
				case "2": j = _dec2(args, j); break;
				case "4": j = _dec4(args, j); break;
				case "8": j = _dec8(args, j); break;
				case "S": j = _decS(args, j); break;
				case "D": j = _decD(args, j); break;
				case "Q": j = _decQ(args, j); break;
				case "{":
					var k = i + 1;
					var func = "";
					while (fmt[k] != "}") {
						func += fmt[k];	
						k++;
					}
					switch(func) {
						case "Twalk":	j = _decTwalk(args, j); break;
						case "Rwalk":	j = _decRwalk(args, j); break;
						case "Stat":	j = _decStat(args, j); break;
					}
					i = k;
					break;
				default:
					alert("Invalid format type!");
					return false; break;
			}
		}

		return [type, tag, buffer];
	}

	return {
		constants: constants,
		fmt: messageFormats,
		msg: messages,
		setBigEndian: function(val) {
			encBigEndian = val;
		},
		setBase64: function(val) {
			encBase64 = val; 
		},
		encodeMessage: function(tag, type, args) {
			if (_checkType(type)) {
				return _encodeMessage(tag, messages[type], args);
			} else {
				alert("Not a valid type");
				return false;
			}
		},
		decodeMessage: function(data) {
			return _decodeMessage(data);
		},
		decodeRaw: function(data, bytes) {
			if (encBase64) {
	   			return _decodeInt(_decode64(data), bytes);
			} else {
				return _decodeInt(data, bytes);
			}
		}
	};

} ();
