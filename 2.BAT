SET PATH=C:\TC\BIN;C:\TC\INCLUDE;C:\PE2;
rem dos_win dos_win.c dos_winw.c
rem dos_win AVT_CTRL.C AVT_CTRw.C
tcc -ml -M -N -Ic:\tc\include -Lc:\tc\lib Dens.c 7188xal.lib x607.lib  > 1.RPT
del *.obj