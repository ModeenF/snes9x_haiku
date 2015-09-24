Snes9x: The Portable Super Nintendo Entertainment System Emulator
=================================================================
Files included in the Snes9x archive:
Snes9x.exe, readme.txt, license.txt fmod.dll faqs.txt changes.txt

Haiku/BeOS/Zeta changes below.

A BeOS native port of Snes9x for BeOS, based on the Snes9x team's work

Features:
-Fully Gui Driven Interface
-DirectWindow Support, all drawing uses MMX technology
-FullScreen Support
-Various scaling modes
-Sound Support
-Configuration Windows
-MultiPlayer Support
-Mouse/SuperScope Support
-One of the most fully featured emulators available on BeOS

New in 1.43 Code from Haiku are used so that it might be BeOS friendly, this code can be removed when Haiku are released and BeOS/Zeta support are dropped.
-Changed allot of code to be more Haiku friendly
-include the png file in resource.
-Added resent used rom file for easier access
-All settings that exist from the snes9x.h (SSettings) are stored in this version, but not all are used or can be changed with GUI and the emulator need to be restarted
-Overlay, Bitmap, DirectWindow and Fullscreen only allowed when they can be used. DirectWindow must be in Fullscreen.
-JMA
-Movie

To do
-Overlay don't work when a rom are running in bitmap or DirectWindow and then switch to Overlay but works when Overlay are chosen before starting the rom
-Update gui with settings, most are dune but need to be finished.
-add NetPlay (netplay.o server.o) (disabled in 1.43)

Requirements: Pentium with MMX

Send feedback if you enjoy using Snes9x to Fredrik Modéen (name @ LastName.se)
