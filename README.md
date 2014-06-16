stegdetect
==========

Stegdetect 0.6 fix for building on Mac Os X

Remember to modify your .bash_profile
1) "cd ~/" to go to your home folder
2) "touch .bash_profile" to create your new file.
3) "vi .bash_profile" to edit it( you can use your favourite editor)
4) add
```
export LC_CTYPE=C 
export LANG=C
```

You can build with:
```
./configure
make
```