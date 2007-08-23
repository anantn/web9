<?php

/**
 * PEAR::Net_9P
 *
 * This class provides an interface to the 9P
 * file protocol.
 *
 * @category  Net
 * @package   Net_9P
 * @version   1.0
 * @author    Anant Narayanan <anant@php.net>
 * @copyright 2007 Anant Narayanan <anant@php.net>
 * @license   http://www.gnu.org/copyleft/lesser.html  LGPL License 2.1
 */

require_once 'PEAR.php';
require_once 'Net/9P/Base.php';
require_once 'Net/Socket.php';

class Net_9P
{
    // {{{ properties

    /**
     * The socket associated with th connection
     *
     * @var Net_Socket
     * @access protected
     */
    protected $socket;

    /**
     * Host name
     *
     * @var String
     * @access protected
     */
    protected $host;

    /**
     * Port Number
     *
     * @var int
     * @default 564
     * @access protected
     */
    protected $port;

    /**
     * Net_9P_Base instance
     *
     * @var Net_9P_Base
     * @access private
     */
    private $generator;

    // }}}
    // {{{ constants
    
    /**
     * Protocol Version
     */
    const VERSION = '9P2000';

    /**
     * Default FID's
     */
    const NOTAG = 0xffff;
    const NOFID = 0xffffffff;

    /**
     * Default Maximum Values
     */
    const MAX_VERSION = 32;
    const MAX_MSG = 8192;
    const MAX_ERROR = 128;
    const MAX_CACHE = 32;
    const MAX_FLEN = 128;
    const MAX_ULEN = 32;
    const MAX_WELEM = 16;

    /**
     * File open parameters
     */
    const OREAD = 0;
    const OWRITE = 1;
    const ORDWR = 2;
    const OEXEC = 3;
    const OTRUNC = 16;
    const OCEXEC = 32;
    const ORCLOSE = 64;
    const ODIRECT = 128;
    const ONONBLOCK = 256;
    const OEXCL = 0x1000;
    const OLOCK = 0x2000;
    const OAPPEND = 0x4000;

    /**
     * File types
     */
    const QTDIR = 0x80;
    const QTAPPEND = 0x40;
    const QTEXCL = 0x20;
    const QTMOUNT = 0x10;
    const QTAUTH = 0x08;
    const QTTMP = 0x04;
    const QTSYMLINK = 0x02;
    const QTFILE = 0x00;

    /**
     * Directory permission bits
     */
    const DMDIR = 0x80000000;
    const DMAPPEND = 0x40000000;    
    const DMEXCL = 0x20000000;  
    const DMMOUNT = 0x10000000;
    const DMAUTH = 0x08000000;  
    const DMTMP = 0x04000000;

    // }}}
    // {{{ __construct()

    /**
     * Initialize a 9P connection
     *
     * @access  public
     * @param   String  $host       Hostname
     * @param   int     $port       Port Number
     */
    public function __construct($host, $port = 564 )
    {
        $this->host = $host;
        $this->port = $port;
        $this->generator = new Net_9P_Base();

        try {
            $this->socket = new Net_Socket($host, $port, true);
        } catch (Exception $e) {
            throw $e;
        }
    }

    // }}}
    // {{{ __destruct()
    
    /**
     * Close the 9P connection
     *
     * @access  public
     */
    public function close()
    {
        $this->socket->disconnect();
    }

    // }}}
    
    /**
     * Send a 'Tversion' message
     *
     * @access  public
     * @param   int     $msize      Maximum message size for the connection
     * @param   String  $version    9P Version
     */
    public function version($msize, $version)
    {
        $message = $this->generator->encodeMessage(Net_9P::NOTAG, 'Tversion', array($msize, $version), true);
        return $message;
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4 softtabstop=4: */

