# NES-Emulator


Compilation
```sh
cmake -G "Unix Makefiles" -S ./
make
```




## Implementation choices:
I decided to declare `class Debugger` as a friend of many composant's classes because I did not want to create functions to access members that are fundamently private (eg. registers)  

