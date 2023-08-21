#include "zinc_ui.h"

const char *zinc_help_text = 
"Pattern:\n"
"  for Linux & Mac:\n"
"    ./zinc [Options] [Image]\n"
"  for Windows:\n"
"    zinc [Options] [Image]\n\n"

"Note:\n"
"  You have to give the parameters after function call.\n\n"
"  You neigther have to put <> brackets nor commas when\n"
"  when <r,g,b> or <x,y> is specified. Just put values\n"
"  and spaces between the parameters.\n"
"  Example: 124 23 53 (r,g,b example)\n"
"           425 645   (x,y example).\n\n"

"Options:\n"
"  --gray                       turn an image gray.\n\n"
"  --gray-correc                as humans don\'t perceive colors equally\n"
"                               so, it is a more accurate version of gray.\n\n"
"  --gray-lumin                 turn the image gray with luminosity.\n\n"
"  --gray-light                 turn the image gray with min,max values.\n\n"
"  --tint <r,g,b>               tint the image with the <r,g,b> values.\n\n"
"  --brightness <percentage>    change the brightness depending on the\n"
"                               percentage.\n\n"
"  more soon...\n\n"

"Short forms:\n"
"  --gray           ->     -g\n"
"  --gray-correc    ->     -gc\n"
"  --gray-lumin     ->     -glu\n"
"  --gray-light     ->     -gli\n"
"  --tint           ->     -ti\n"
"  --brightness     ->     -br\n\n"
"  more soon...\n";
