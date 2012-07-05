# Rotating Status Message
# Copyright Skand Hurkat, 2012

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.



# This program can be used to set a rotating Status update in Google Talk.
# Extremely useful if you do not wish to be disturbed.

# Because the staus message updates every second, any friend who wishes to chat
# with you will have his chat window flooded with your Status updates, thereby
# killing any desire he may have to chat with you.

import sys, getpass, time, getopt, xmpp, msvcrt

message=""
JabberID=""
passwd=""
Status = "dnd"
charactersToDisplay = 13
cl=None
jid=None

def main():
    global message, JabberID, passwd
    try:
        opts, args = getopt.getopt(sys.argv[1:], "j:p:m:s:l:h", ["jid=", "password=", "message=", "Status=", "lengthofdisplay=","help"])
    except getopt.GetoptError, err:
        print str(err)
        usage()
        sys.exit(2)
    for o,a in opts:
        if o in ("-j", "--jid"):
            JabberID=a
        elif o in ("-p", "--password"):
            passwd=a
        elif o in ("-m", "--message"):
            message=a
        elif o in ("-s", "--Status"):
            Status=a
            if Status not in ("away", "chat", "dnd", "xa"):
                print "Oops! Status must be one of 'away', 'chat', 'dnd', or 'xa'"
                sys.exit(1)
        elif o in("-l", "--lengthofdisplay"):
            try:
                charactersToDisplay=int(a)
            except ValueError:
                print "Oops! Unfortunately, I can only display an integer number of characters"
                print "I give up!"
                sys.exit(1)
            if charactersToDisplay <= 0:
                print "Oops! I can only display positive integer number of characters"
                print "I give up!"
                sys.exit(1);
        elif o in ("-h", "--help"):
            usage()
            sys.exit(0)
        else:
            assert False, "Unhandled Option"

    if len(JabberID) and len(passwd) and len(message):
        connect()
        authorise()
        loopDisplay()
        pass
    else:
        print "Error! All details not provided"
        usage()
        sys.exit(1)
    cl.disconnect()

def usage():
    print "rotating_Status_xmpp.py"
    print "Usage:"
    print "python rotating_Status_xmpp.py <options>"
    print "\tOptions"
    print "\t--jid= -j <Jabber ID>"
    print "\t--password= -p <Password>"
    print "\t--message= -m <Message>"
    print "\t--Status= -s [Status] one of 'away', 'chat', 'dnd', or 'xa'. Default is 'dnd'"
    print "\t--lengthofDisplay= -l [characters to be displayed]"
    print "\t--help -h (Displays this message)"
        
def connect():
    global JabberID, cl, jid
    jid = xmpp.protocol.JID(JabberID)
    cl = xmpp.Client(jid.getDomain(), debug=[])
    con = cl.connect(('talk.l.google.com',443))
    if not con:
        print "Error: Not Connected"
        sys.exit(4)
    else:
        print "Connected via", con

def authorise():
    global passwd, cl
    auth = cl.auth(jid.getNode(), passwd, "botty")
    if not auth:
        print "Not Authenticated"
        sys.exit(4)
    else:
        print "Authenticated via", auth

def loopDisplay():
    global cl, message, charactersToDisplay, Status
    sm = ""
    while True:
        for i in range (1, (len(message)+charactersToDisplay)):
            if i < charactersToDisplay:
                sm = (charactersToDisplay-i)*" " + message[0:i]
            else:
                sm = message[i-charactersToDisplay:i]
            pres = xmpp.Presence(priority=5, show=Status, status=sm)
            cl.send(pres)
            sys.stdout.write(sm)
            sys.stdout.flush()
            if msvcrt.kbhit():
                msvcrt.getch()
                return
            time.sleep(1)
            sys.stdout.write(charactersToDisplay*"\b"+charactersToDisplay*" "+charactersToDisplay*"\b")
        pres = xmpp.Presence(priority=5, show=Status, status=sm)
        cl.send(pres)
        sys.stdout.write(sm)
        sys.stdout.flush()
        if msvcrt.kbhit():
            msvcrt.getch()
            return
        time.sleep(1)
        sys.stdout.write(charactersToDisplay*"\b"+charactersToDisplay*" "+charactersToDisplay*"\b")

if __name__ == "__main__":
    main()
