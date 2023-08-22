<p align="center">
  <img src="https://github.com/MubinMuhammad/Zinc/blob/master/readme_res/logo.png?raw=true" 
  alt="Zinc Logo" 
  width=500 /> 
</p>

## What is it?
Well, Zinc is an image processor, written in C. It supports:
- 4 types of grayscale conversion.
- brightness adjustment
- tinting
- inverting colors

more things will be added soon like: blur, sharpening, edge-detection etc.

## Building
Go to the project directory. And then run this command in your terminal or command prompt (you might have to open up cmd in the file explorer in Windows for easier access):
```bash
gcc src/*.c -o zinc -lm
```
You will see a executable in the project's main directory.

## How to use it?
Use `./zinc --help` and for Windows do `zinc --help` for more information.
But where is the output of the help command:

```
Pattern:
  for Linux & Mac:
    ./zinc [Options] [Image]
  for Windows:
    zinc [Options] [Image]

Note:
  You have to give the parameters after function call.

  You neigther have to put <> brackets nor commas when
  when <r,g,b> or <x,y> is specified. Just put values
  and spaces between the parameters.
  Example: 124 23 53 (r,g,b example)
           425 645   (x,y example).

Options:
  --gray                       turn an image gray.

  --gray-correc                as humans don't perceive colors equally
                               so, it is a more accurate version of gray.

  --gray-lumin                 turn the image gray with luminosity.

  --gray-light                 turn the image gray with min,max values.

  --tint <r,g,b>               tint the image with the <r,g,b> values.

  --brightness <percentage>    change the brightness depending on the
                               percentage.

  more soon...

Short forms:
  --gray           ->     -g
  --gray-correc    ->     -gc
  --gray-lumin     ->     -glu
  --gray-light     ->     -gli
  --tint           ->     -ti
  --brightness     ->     -br

  more soon...
```
