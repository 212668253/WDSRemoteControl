# WDS Remote Control

Remote control based on [Windows Desktop Sharing API](https://docs.microsoft.com/en-us/windows/win32/api/_rdp/)

How to test locally:
- Clone & build repository
- Start "WinSharer" application
- Press "Start Presentation".
- Copy "inv.xml" that contains the authentication token from WinSharer to WinViewer bin folder
- Start "WinViewer" application
- Press "Connect"
- Observe that Windows desktop is being mirrored into the WinViewer application
