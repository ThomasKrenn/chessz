
#include "build.h"


#define VER_FILEDESCRIPTION_STR         STRINGIZE(Chessx)
#define VER_INTERNALNAME_STR            STRINGIZE(chessx.exe)
#define VER_ORIGINALFILENAME_STR        VER_INTERNALNAME_STR

#define VER_DOCCREATOR                  "ChessX (Version: " VER_FILE_VERSION_STR ", " VERSION_COMPILEDATE ")"

#if defined(_DEBUG) && defined(_WINDOWS)
#include "vld.h"
#endif


