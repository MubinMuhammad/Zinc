<p align="center">
  <img src="https://github.com/MubinMuhammad/Zinc/blob/master/readme_res/logo.png?raw=true" 
  alt="Zinc Logo" 
  width=500 /> 
</p>

## What is it?
Well, Zinc is an image processor, written in C. It supports:
- GrayScale Avarage
- GrayScale Luminosity 
- GrayScale Lightness
- Brightness adjustment
- Tinting
- Inverting colors
- Cropping without changing the resolution
- Cropping with changing the resolution

More things will be added soon like: blur, sharpening, edge-detection etc. As it's under the MIT License, you can see the algorithms by yourself. It might be help full is just getting started with Image Processing.

## Building
If you are using Windows you do have to use mingw32 or mingw64.
Go to the project's root directory. And then run the command below in a terminal(cmd for Windows):
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
  You neigther have to put <> brackets nor commas when <r,g,b> or <x,y> is specified. Just put values and spaces between the parameters.
    Example: 0.43 0.654 0.12 (r,g,b example)
             425 645   (x,y example).
  Lastly we use factors instead of percentages. As it's more accurate.
    Example: for brightness, setting factor to 2.0 will double the brightness.
             for tinting, changing the r value to 1.5 will increase half of the red value.

Options:
  -v, --version                                    check the version of zinc.
  -h, --help                                       show this help message.
  -o, --output                                     choose a name for the output file.
  -g, --gray                                       turn an image gray.
  -gl, --gray-lumin                                turn the image gray with rgb values that fits human eyes.
  -gli, --gray-light                               turn the image gray with min,max values.
  -ti, --tint <r,g,b>                              tint the image with the <r,g,b> values.
  -br, --brightness <factor>                       change the brightness depending on the percentage.
  -in --invert                                     invert the colors of the image.
  -cwctr, --cropwctr <x1,y1> <x2,y2> <r,g,b>       crop the image without changing the resolution. x1,y1 = top left coordinate. x2,y2 = bottom right coordinate. r,g,b = color for the uncropped areas.
  -cctr, --cropctr <x1,y1> <x2,y2>                 crop the image and change the resolution. x1,y1 = top left coordinate. x2,y2 = bottom right coordinate.
  more soon...
```
