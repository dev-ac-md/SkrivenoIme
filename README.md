Source code modified to vs22 standards so things can be added more easly.

How to compile exe. Dlls are dependednt on library of exe which is dependent on libraries of dlls.

1. For all except cossacks2 go to properties and change configuration type to lib
2. Right click dmcr.lib in every project and exclude it from build in properties (not delete)
3. build libraries
4. Build cossacks 2
5. add back dmcr.lib which was excluded earlier
6. change configuration type to dll
7. build one after another (Commcore and gMotor arent DLL files they stay libraries)


Known issues for now:

-Diplomacy with natives doesnt work properly

-Windowed/Borderless not fully implemented

-Main menu top left because windowed implementation isnt done

-Formations are stacked in perfect order instead of little variation in position of soldier

-Produced units exit to wrong location