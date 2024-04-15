# Snake Game in C++ using ncurses

The Snake game created using C++ and ncurses playable in the cmd

## For Mac users

To run the main.cpp file
1. In the terminal go to ur src folder where the main.cpp file is stored
2. The type
    ```
    g++ -o main main.cpp -lncurses
    ```
3. The run the executable file generated by
   ```
   ./main
   ```
## For Windows users

1. Change the include line to
   ```
   #include <ncurses/ncurses.h>
   ```
2. In the terminal go to ur src folder where the main.cpp file is stored
3. The type
    ```
    g++ main.cpp -o main -I C:\msys64\mingw64\include -L C:\msys64\mingw64\lib -lncurses  -DNCURSES_STATIC
    ```
4. The run the executable file generated by
   ```
   ./main
   ```

##Output
[![Snake](snake.png)]
