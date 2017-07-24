A proof of concept of making use of multiple cores on a system without any extra command line arguments during compilation.

Specifically, for ai contests such as theaigames.com or riddles.io where the compile time arguments are fixed and do not support multi threading.

The program makes use of ```fork()``` and shared memory to achieve this.


Discussion threads : http://theaigames.com/discussions/general/570920af5d203c9310cc86c7/c-threading-support/1/show and https://starapple.riddles.io/competitions/light-riders/discussions/discussion/7b06178d-e6ee-49e3-b8ab-3a153b631395/1

## Compiling

To compile simply run ```make``` within the root directory. This should work on linux (No support for windows or mac).

## Testing

There is a built in test within the simple program.

## Cleaning

To clean after a make or make check simply run ```make clean``` within the root directory.
