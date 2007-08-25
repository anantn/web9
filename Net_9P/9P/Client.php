<?php

/**
 * PEAR::Net_9P_Client
 *
 * This class acts as an interface to a 9P client
 *
 * @category  Net
 * @package   Net_9P
 * @version   1.0
 * @author    Anant Narayanan <anant@php.net>
 * @copyright 2007 Anant Narayanan <anant@php.net>
 * @license   http://www.gnu.org/copyleft/lesser.html  LGPL License 2.1
 */

require_once 'PEAR/Exception.php';
require_once 'Net/9P/Base.php';
require_once 'Net/9P/Socket/Client.php';

class Net_9P_Client
{
    // {{{ properties
    
    /**
     * Instance of Net_9P_Socket_Client
     * associated with the object
     *
     * @var Net_9P_Socket_Client
     * @access private
     */
    protected $socket;

    /**
     * Instance of Net_9P_Base associated
     * with the object
     *
     * @var Net_9P_Base
     * @access protected
     */
    protected $generator;

    /**
     * fid for root of the file tree
     *
     * @var int
     * @access protected
     */
    protected $rootFid;

    /**
     * fid for root of the file tree
     *
     * @var int
     * @access protected
     */
    protected $rootFid;

    // }}}
    // {{{ __construct()

    /**
     * Create a new 9P client 
     *
     * @access  public
     * @param   String  $dialString     A 9P Dial String. Can be of the form
     *                                  protocol!host!port or host!port or simply host.
     *                                  Protocol should be either tcp or unix
     */
    public function __construct($dialString)
    {
        $parts = explode($dialString, '!');
        switch (count($parts)) {
            case 1: $this->socket = new Net_9P_Socket_Client('tcp', $parts[0]);
                    break;
            case 2: $this->socket = new Net_9P_Socket_Client($parts[0], $parts[1]);
                    break;
            case 3: $this->socket = new Net_9P_Socket_Client($parts[0], $parts[1], $parts[2]);
                    break;
            default:
                    throw new PEAR_Exception('Invalid Dial String');
                    break;
        }

        if (!$this->socket)
            throw new PEAR_Exception('Could not create socket');

        $this->generator = new Net_9P_Base();
        $this->doAttach();
    }

    // }}}
    // {{{ doTransaction()

    /**
     * Perform a single 9P transaction
     *
     * @access  private
     * @param   $msg    Message to send as returned by Net_9P_Base::encodeMessage()
     * @return  Message as decoded by Net_9P_Base::decodeMessage()
     */
    private function doTransaction($msg)
    {
        $this->socket->write($vMsg);
        $sMsg = $this->socket->read(4);
        $size = $this->generator->decodeRawInteger($sMsg);

        $rMsg = $this->socket->read($size - 4);
        return $this->generator->decodeMessage($sMsg.$rMsg);
    }

    /**
     * Perform the version and auth transactions
     *
     * @access  private
     */
    private function doAttach()
    {
        $vMsg = $this->generator->encodeMessage(Net_9P::NOTAG, 'Tversion', (16 * 1024, Net_9P::VERSION));
    }

    // }}}
    
}

/* vim: set expandtab tabstop=4 shiftwidth=4 softtabstop=4: */

