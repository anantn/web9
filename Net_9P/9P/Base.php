<?php

/**
 * PEAR::Net_9P_Base
 *
 * This class provides basic message creation and parsing methods
 * to deal with the 9P Protocol.
 *
 * @category  Net
 * @package   Net_9P
 * @version   1.0
 * @author    Anant Narayanan <anant@php.net>
 * @copyright 2007 Anant Narayanan <anant@php.net>
 * @license   http://www.gnu.org/copyleft/lesser.html  LGPL License 2.1
 */

require_once 'Net/9P/Qid.php';
require_once 'Net/9P/Stat.php';

class Net_9P_Base
{
    // {{{ properties
    
    /**
     * Message type suffixes
     *
     * @var Array
     * @access private
     */
    private $mTypes = array("version", "auth", "attach", "error",
                            "flush", "walk", "open", "create",
                            "read", "write", "clunk", "remove",
                            "stat", "wstat");

    /**
     * Message type format specifiers
     *
     * @var Array
     * @access private
     */
    private $mFormat = array("4S", "4S", "4SS", "Q", "44SS", "Q", "", "S",
                             "2", "", "{Twalk}", "{Rwalk}", "41", "Q4", "4S41", "Q4",
                             "484", "D", "48D", "4", "4", "", "4", "",
                             "4", "{Stat}", "4{Stat}", "");

    /**
     * Current message buffer
     *
     * @var Array
     * @access private
     */
    private $buffer;

    /**
     * Message codes
     *
     * @var Array
     * @access protected
     */
    protected $messageCodes;

    /**
     * Message formats
     *
     * @var Array
     * @access protected
     */
    protected $messageFormats;

    // }}}
    // {{{ __construct()

    /**
     * Sets the class properties
     *
     * @access public
     */
    public function __construct()
    {
        $start = 100;
        foreach ($this->mTypes as $message) {
            $this->messageCodes['T'.$message] = $start;
            $this->messageCodes['R'.$message] = $start + 1;
            $start += 2;
        }

        $start = 100;
        foreach ($this->mFormat as $format) {
            $this->messageFormats[$start] = $format;
            $start += 1;
        }

        $this->buffer = array();
    }

    // }}}
    // {{{ encodeInteger()

    /**
     * Encodes an integer to a 9P binary value
     *
     * @access  private
     * @param   int     $num        Number to encode
     * @param   int     $bytes      Number of bytes to encode into
     *                              (1, 2, 4 and 8 are the only acceptable values)
     */
    private function encodeInteger($num, $bytes)
    {
        switch ($bytes) {
            case 1: $this->buffer[] = pack('C', $num); break;
            case 2: $this->buffer[] = pack('v', $num); break;
            case 4: $this->buffer[] = pack('V', $num); break;
            case 8: /* TODO */ break;
        }
    }

    // }}}
    // {{{ decodeInteger()
    
    /**
     * Decodes a 9P binary value to an integer
     *
     * @access  private
     * @param   String  $value      String to decode from
     * @param   int     $index      Index of string to begin decoding from
     * @param   int     $bytes      Number of bytes to decode
     *                              (1, 2, 4 and 8 are the only acceptable values)
     * @return  Index of next value
     */
    private function decodeInteger($value, $index, $bytes)
    {
        switch ($bytes) {
            case 1: $this->buffer[] = unpack('C', substr($value, $index, 1));
                    return $index + 1;
            case 2: $this->buffer[] = unpack('v', substr($value, $index, 2));
                    return $index + 2;
            case 4: $this->buffer[] = unpack('V', substr($value, $index, 4));
                    return $index + 4;
            case 8: // TODO //
                    return $index + 8;
        }
    }

    // }}}
    // {{{ encodeString()

    /**
     * Encodes a string into the 9P format
     *
     * @access  private
     * @param   String  $value      String to encode
     * @param   int     $size       Number of bytes to represent size
     *                              (2 or 4 are the only acceptable values)
     */
    private function encodeString($value, $size)
    {
        $this->encodeInteger(strlen($value), $size);
        $this->buffer[] = $value; 
    }

    // }}}
    // {{{ decodeString()
    
    /**
     * Decodes a 9P binary value to a string
     *
     * @access  private
     * @param   String  $value      String to decode from
     * @param   int     $index      Index of string to begin decoding from
     * @param   int     $bytes      Number of bytes of string size
     *                              (2 or 4 are the only acceptable values)
     * @return  Index of next value
     */
    private function decodeString($value, $index, $bytes)
    {
        $nIndex = $this->decodeInteger($value, $index, $bytes);
        $retVal = substr($value, $nIndex, $this->buffer[-1]);
        $this->buffer[-1] = $retVal;

        return $nIndex + strlen($retVal);
    }

    // }}}
    // {{{ encodeQid()

    /**
     * Encodes a Qid into the 9P format
     *
     * @access  private
     * @param   Net_9P_Qid  $value  Qid to encode
     */
    private function encodeQid($value)
    {
        $this->encodeInteger($value->qType, 1);
        $this->encodeInteger($value->version, 2);
        $this->encodeInteger($value->path, 8);
    }

    // }}}
    // {{{ decodeQid()
    
    /**
     * Decodes a 9P binary value to a Qid
     *
     * @access  private
     * @param   String  $value      String to decode from
     * @param   int     $index      Index of string to begin decoding from
     * @return  Index of next value
     */
    private function decodeQid($value, $index)
    {
        $nIndex = $this->decodeInteger($value, $index, 1);
        $qType = $this->buffer[-1];

        $nIndex = $this->decodeInteger($value, $nIndex, 2);
        $version = $this->buffer[-1];

        $nIndex = $this->decodeInteger($value, $nIndex, 8);
        $path = $this->buffer[-1];

        $qid = new Net_9P_Qid($qType, $version, $path);
        array_splice($this->buffer, -3, 3, $qid);

        return $nIndex;
    }

    // }}}
    // {{{ encodeTwalk()

    /**
     * Encodes a Twalk 9P message
     *
     * @access  private
     * @param   int     $fid    
     * @param   int     $newfid
     * @param   Array   $names
     */
    private function encodeTwalk($fid, $newfid, $names)
    {
        $this->encodeInteger($fid, 4);
        $this->encodeInteger($newfid, 4);
        $len = count($names);
        $this->encodeInteger($len, 2);
        foreach ($names as $name) {
            $this->encodeString($name, 4);
        }
    }

    // }}}
    // {{{ decodeTwalk()
    
    /**
     * Decodes a Twalk 9P message
     *
     * @access  private
     * @param   String  $value      String to decode from
     * @param   int     $index      Index of string to begin decoding from
     * @return  Index of next value
     */
    private function decodeTwalk($value, $index)
    {
        $nIndex = $this->decodeInteger($value, $index, 4);
        $fid = $this->buffer[-1];

        $nIndex = $this->decodeInteger($value, $nIndex, 4);
        $newfid = $this->buffer[-1];

        $nIndex = $this->decodeInteger($value, $nIndex, 2);
        $len = $this->buffer[-1];

        $names = array();
        for ($i = 0; $i < $len; $i) {
            $nIndex = $this->decodeString($value, $nIndex, 2);
            $names[] = array_splice($this->buffer, -1);
        }

        $twalk = array('fid' => $fid, 'newfid' => $newfid, 'names' => $names);
        array_splice($this->buffer, -2, 2, $twalk);

        return $nIndex;
    }

    // }}}
    // {{{ encodeRwalk()

    /**
     * Encodes an Rwalk 9P message
     *
     * @access  private
     * @param   Array   $qids   An array of Net_9P_Qid objects
     */
    private function encodeRwalk($qids)
    {
        $this->encodeInteger(count($qids), 2);
        foreach ($qids as $qid) {
            $this->encodeQid($qid);
        }
    }

    // }}}
    // {{{ decodeTwalk()
    
    /**
     * Decodes an Rwalk 9P message
     *
     * @access  private
     * @param   String  $value      String to decode from
     * @param   int     $index      Index of string to begin decoding from
     * @return  Index of next value
     */
    private function decodeRwalk($value, $index)
    {
        $nIndex = $this->decodeInteger($value, $index, 2);
        $len = $this->buffer[-1];

        for ($i = 0; $i < $len; $i++) {
            $nIndex = $this->decodeQid($value, $nIndex);
        }

        $qids = array_splice($this->buffer, count($this->buffer) - $len - 1, $len);
        $this->buffer[-1] = $qids;

        return $nIndex;
    }

    // }}}
    // {{{ encodeStat()

    /**
     * Encode a Stat 9P message
     *
     * @access  private
     * @param   Array   $stats   An array of Net_9P_Stat objects
     */
    private function encodeStat($stats)
    {
        $statLength = count($stats);
        $this->encodeInteger($statLength, 2);

        foreach ($stats as $stat) {
            $curSize = 2 + 4 + 13 + 4 + 4 + 4 + 8 + 
                       strlen($stat->uid) + strlen($stat->gid) +
                       strlen($stat->muid);

            $this->encodeInteger($curSize, 2);
            $this->encodeInteger($this->sType, 2);
            $this->encodeInteger($this->dev, 4);
            $this->encodeQid($this->qid);
            $this->encodeInteger($this->mode, 4);
            $this->encodeInteger($this->atime, 4);
            $this->encodeInteger($this->mtime, 4);
            $this->encodeInteger($this->length, 8);
            $this->encodeString($this->sName, 2);
            $this->encodeString($this->uid, 2);
            $this->encodeString($this->gid, 2);
            $this->encodeString($this->muid, 2);
        }
    }

    // }}}
    // {{{ decodeStat()
    
    /**
     * Decodes a Stat 9P message
     *
     * @access  private
     * @param   String  $value      String to decode from
     * @param   int     $index      Index of string to begin decoding from
     * @return  Index of next value
     */
    private function decodeStat($value, $index)
    {
        $nIndex = $this->decodeInteger($value, $index, 2);
        $totalLength = $this->buffer[-1];
        $totalRead = 0;
        array_splice($this->buffer, -1);

        $stats = array();
        while ($totalRead < $totalLength) {
            $nIndex = $this->decodeInteger($value, $nIndex, 2);
            $size = $this->buffer[-1];

            $nIndex = $this->decodeInteger($value, $nIndex, 2);
            $type = $this->buffer[-1];

            $nIndex = $this->decodeInteger($value, $nIndex, 4);
            $dev = $this->buffer[-1];

            $nIndex = $this->decodeQid($value, $nIndex);
            $qid = $this->buffer[-1];

            $nIndex = $this->decodeInteger($value, $nIndex, 4);
            $mode = $this->buffer[-1];

            $nIndex = $this->decodeInteger($value, $nIndex, 4);
            $atime = $this->buffer[-1];

            $nIndex = $this->decodeInteger($value, $nIndex, 4);
            $mtime = $this->buffer[-1];

            $nIndex = $this->decodeInteger($value, $nIndex, 8);
            $len = $this->buffer[-1];

            $nIndex = $this->decodeString($value, $nIndex, 2);
            $name = $this->buffer[-1];

            $nIndex = $this->decodeString($value, $nIndex, 2);
            $uid = $this->buffer[-1];

            $nIndex = $this->decodeString($value, $nIndex, 2);
            $gid = $this->buffer[-1];

            $nIndex = $this->decodeString($value, $nIndex, 2);
            $muid = $this->buffer[-1];

            $stats[] = new Net_9P_Stat($type, $dev, $qid, $mode, $atime, $mtime,
                                       $len, $name, $uid, $gid, $muid);
            
            array_splice($this->buffer, -12);
        }

        $this->buffer[] = $stats;
        return $nIndex;
    }

    // }}}
    // {{{ encodeMessage

    /**
     * Encode a 9P message
     *
     * @access  public
     * @param   int     $tag    Message Tag
     * @param   String  $type   Message Type
     * @param   Array   $args   Message Arguments
     * @param   Boolean $base   Pass true if message is to be Base64 encoded
     * @return  String  Binary 9P Message
     *          False   Message could not be formed
     */
    public function encodeMessage($tag, $type, $args, $base = false)
    {
        $this->buffer = array();
        if (!$this->messageCodes[$type])
            return false;

        $code = $this->messageCodes[$type];
        $this->encodeInteger($code, 1);
        $this->encodeInteger($tag, 2);

        $format = $this->messageFormats[$code];

        $j = 0;
        for ($i = 0; $i < strlen($format); $i++) {
            switch($format[$i]) {
                case '1':   $this->encodeInteger($args[$j], 1); break;
                case '2':   $this->encodeInteger($args[$j], 2); break;
                case '4':   $this->encodeInteger($args[$j], 4); break;
                case '8':   $this->encodeInteger($args[$j], 8); break;
                case 'S':   $this->encodeString($args[$j], 2); break;
                case 'D':   $this->encodeString($args[$j], 4); break;
                case '{':
                    $k = $i + 1;
                    $func = '';
                    while ($format[$k] != '}') {
                        $func .= $format[$k];
                        $k += 1;
                    }
                    switch ($func) {
                        case 'Twalk':   _encTwalk($args[$j]); break;
                        case 'Rwalk':   _encRwalk($args[$j]); break;
                        case 'Stat':    _encStat($args[$j]); break;
                    }
                    $i = $k;
                    break;
                default:
                    return false;
            }
            $j += 1;
        }

        $len = 0;
        $msg = '';
        foreach ($this->buffer as $item) {
            $len += strlen($item);
            $msg .= $item;
        }

        $this->buffer = array();
        $this->encodeInteger($len + 4, 4);

        $this->buffer = $this->buffer[0].$msg;

        if ($base) {
            $this->buffer = base64_encode($this->buffer);
        }

        return $this->buffer;
    }

}

/* vim: set expandtab tabstop=4 shiftwidth=4 softtabstop=4: */

