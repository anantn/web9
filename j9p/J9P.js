J9P = function() {

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

		for (var i = 0; i < messages.length; i++) {
			ret[messages[i]] = fmt[i];
		}
		return ret;
	} ();

	/* Set the various 9P constants */
	var version = "9P2000";
	var notag = 0xffff;
	var nofid = 0xffffffff;
	
	var MAX_VERSION = 32;
	var MAX_MSG = 8192;
	var MAX_ERROR = 128;
	var MAX_CACHE = 32;
	var MAX_FLEN = 128;
	var MAX_ULEN = 32;
	var MAX_WELEM = 16;

	/* From libc.h in p9p */
	var OREAD = 0;
	var OWRITE = 1;
	var ORDWR = 2;
	var OEXEC = 3;
	var OTRUNC = 16;
	var OCEXEC = 32;
	var ORCLOSE = 64;
	var ODIRECT = 128;
	var ONONBLOCK = 256;
	var OEXCL = 0x1000;
	var OLOCK = 0x2000;
	var OAPPEND = 0x4000;

	/* Bits for Qid */
	var QTDIR = 0x80;
	var QTAPPEND = 0x40;
	var QTEXCL = 0x20;
	var QTMOUNT = 0x10;
	var QTAUTH = 0x08;
	var QTTMP = 0x04;
	var QTSYMLINK = 0x02;
	var QTFILE = 0x00;

	/* Bits for Dir */
	var DMDIR = 0x80000000;
	var DMAPPEND = 0x40000000;	
	var DMEXCL = 0x20000000;	
	var DMMOUNT = 0x10000000;
	var DMAUTH = 0x08000000;	
	var DMTMP = 0x04000000;

	/* The message buffer */
	var buffer = [];

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

	// Check if type is valid
	function _checkType(type) {
		for (var i = 0; i < messages.length; i++) {
			if (type == messages[i]) {
				return true;
			}
		}
		return false;
	}

	// Do encoding on the basis of format string
	function _encodeMessage(args) {
		buffer = "";
		fmt = messageFormats[args[0]];
		var j = 1;
		for (var i = 0; i < fmt.length; i++) {
			switch(fmt[i]) {
				case "1":
				case "2":
				case "4":
				case "8":
				case "S":
				case "Q":
					this["_enc" + fmt[i]](args[j]); j++;
					break;
				case "{":
					var k = i + 1;
					var func = "";
					while (fmt[k] != "}") {
						func += fmt[k];	
						k++;
					}
					this["_enc" + func](args[j]);
					i = k; j++;
					break;
				default:
					alert("Invalid format type!");
					return false; break;
			}
		}
		return buffer;
	}

	return {
		getBuffer: function() {
			return buffer;
		},
		setBuffer: function(buf) {
			buffer = buf;
		},
		createMessage: function() {
			var n = arguments.length;
			if (_checkType(arguments[0])) {
				return _encodeMessage(arguments);
			} else {
				alert("Not a valid type");
				return false;
			}	
		},
		sendMessage: function() {
					 
		}
	};

} ();
