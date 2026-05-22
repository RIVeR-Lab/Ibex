import eBUS as eb
from time import sleep
import signal

HostType = "Windows"
try:
    import msvcrt
except ImportError:
    import sys
    import termios
    import atexit
    from select import select
    HostType = "Posix"

class Singleton(type):
    _instances = {}
    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            cls._instances[cls] = super(Singleton, cls).__call__(*args, **kwargs)
        return cls._instances[cls]

# Keyboard handling
class PvKb(metaclass = Singleton):
    __start_count = 0
    __stopping = False
    def __init__(self):
        if HostType == "Posix":
            self.fd = sys.stdin.fileno()
            self.new_term = termios.tcgetattr(self.fd)
            self.old_term = termios.tcgetattr(self.fd)
            self.new_term[3] = (self.new_term[3] & ~termios.ICANON & ~termios.ECHO)
            atexit.register(self.__set_normal_term)
    def start(self):
        self.__start_count += 1
        if self.__start_count == 1:
            signal.signal(signal.SIGINT, self.__set_stopping)
            self.__stopping = False
            self.__set_nb_term()
    def __set_stopping(self, signum, frame):
        self.__stopping = True
    def is_stopping(self) :
        return self.__stopping
    def __set_nb_term(self):
        if HostType == "Posix":
            termios.tcsetattr(self.fd, termios.TCSAFLUSH, self.new_term)
    def stop(self):
        self.__start_count -= 1
        if self.__start_count == 0:
            self.__set_normal_term()
            signal.signal(signal.SIGINT, signal.SIG_DFL)
    def __set_normal_term(self):
        if HostType == "Posix":
            termios.tcsetattr(self.fd, termios.TCSAFLUSH, self.old_term)
    def getch(self):
        if HostType == "Posix":
            return sys.stdin.read(1)
        else:
            return msvcrt.getch().decode('utf-8')
    def kbhit(self):
        if HostType == "Posix":
            return select([sys.stdin], [], [], 0) == ([sys.stdin], [], [])
        else:
            return msvcrt.kbhit()

def PvSelectDevice() :
    lSystem = eb.PvSystem()

    print( "\nDetecting devices." )

    kb = PvKb()
    kb.start()
    while not kb.is_stopping() :
        lSystem.Find()

        # Detect, select device.
        lDIVector = []
        for i in range( lSystem.GetInterfaceCount() ) :
            lInterface = lSystem.GetInterface( i )
            print( f"   {lInterface.GetDisplayID()}" )
            for j in range( lInterface.GetDeviceCount() ) :
                lDI = lInterface.GetDeviceInfo( j )
                lDIVector.append( lDI )
                print( f"[{len(lDIVector) - 1}]\t{lDI.GetDisplayID()}" )

        if len(lDIVector) == 0 :
            print( f"No device found!" )

        print( f"[{len(lDIVector)}] to abort" ) 
        print( f"[{len(lDIVector) + 1}] to search again\n" ) 

        print( "Enter your action or device selection?" )
        print( ">", end = '' )

        # Read device selection, optional new IP address.
        try:
            lIndex = int( kb.getch(), 10 )
        except ValueError :
            print( "Invalid key pressed. Exiting" )
            return None

        if lIndex == len(lDIVector) :
            # We abort the selection process.
            return None
        elif lIndex < len(lDIVector) :
            # The device is selected
            lSelectedDI = lDIVector[ lIndex ]
            break

    # Is the IP Address valid?
    if lSelectedDI.IsConfigurationValid() :
        return lSelectedDI.GetConnectionID()

    if lSelectedDI.GetType() == eb.PvDeviceInfoTypeUSB or lSelectedDI.GetType() == eb.PvDeviceInfoTypeU3V :
        print( "This device must be connected to a USB 3.0 (SuperSpeed) port." )
        return None

    # Ask the user for a new IP address.
    print( "The IP configuration of the device is not valid." )
    print( "Which IP address should be assigned to the device?" )
    kb.stop()
    lNewIPAddress = input( ">" )
    kb.start()
    if not lNewIPAddress:
        return None

    if isinstance( lSelectedDI, eb.PvDeviceInfoGEV ) :
        # Force new IP address.
        lResult = eb.PvDeviceGEV.SetIPConfiguration( lSelectedDI.GetMACAddress(), lNewIPAddress,
            lSelectedDI.GetSubnetMask().GetAscii(), lSelectedDI.GetDefaultGateway().GetAscii() )
        if not lResult.IsOK() :
            print( "Unable to force new IP address." )
            return None
    # Wait for the device to come back on the network.
    lTimeout = 0
    while not kb.is_stopping() :
        lTimeout = 10
        while lTimeout > 0 :
            lSystem.Find()
            for i in range( lSystem.GetInterfaceCount() ) :
                lInterface = lSystem.GetInterface( i )
                for j in range( lInterface.GetDeviceCount() ) :
                    lDI = lInterface.GetDeviceInfo( j )
                    if isinstance(lDI, eb.PvDeviceInfoGEV ) :
                        if lNewIPAddress == lDI.GetIPAddress() :
                            return lDI.GetConnectionID()
            sleep( 1 )
            lTimeout = lTimeout - 1

        print( f"The device {lNewIPAddress} was not located. Do you want to continue waiting? (y/n)" )
        print( ">", end='' )
        lAnswer = kb.getch()
        if ( lAnswer == "n" ) :
            break
    return None

def PvSelectInterface() :
    lSystem = eb.PvSystem()
    lCount = lSystem.GetInterfaceCount()
    print( f"interface count: {lCount}")
    for i in range( lCount ) :
        lNA = lSystem.GetInterface( i )
        if not isinstance( lNA, eb.PvNetworkAdapter ) :
            continue
        if lNA.GetIPAddressCount() == 0 or lNA.GetIPAddress( 0 ) == "0.0.0.0" :
            continue
        print( f"{i}) {lNA.GetDisplayID()} ({lNA.GetIPAddress(0)})" )

    kb = PvKb()
    kb.start()
    print( f"Network interface selection?" )
    print( ">", end='', flush=True )
    try:
        lIndex = int( kb.getch(), 10 )
    except ValueError :
        print( "Invalid key pressed. Exiting" )
        return ""
    print( "", end='\r', flush=True )
    if lIndex >= 0 and lIndex < lCount :
        return lSystem.GetInterface( lIndex ).GetMACAddress()

    return ""
