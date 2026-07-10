

/*
* Turn A into a string literal without expanding macro definitions
* (however, if invoked from a macro, macro arguments are expanded).
*/
#define STRINGIZE_NX(A) #A

#define STRINGIZE(s) STRINGIZE_NX(s)

#define VERSION_MAJOR               CHESSZ_VERSION_MAJOR
#define VERSION_MINOR               CHESSZ_VERSION_MINOR
#define VERSION_REVISION            CHESSZ_VERSION_REVISION
#define VERSION_BUILD               CHESSZ_VERSION_BUILD
#define VERSION_DATE                CHESSZ_VERSION_DATE
#define VERSION_YEAR                CHESSZ_VERSION_YEAR

#define VERSION_DATE_STR            STRINGIZE(VERSION_DATE)

#define VERSION_DATE_PREFIX         "(Build: "
#define VERSION_DATE_POSTFIX        ")"

#define VERSION_COMPILEDATE         VERSION_DATE_PREFIX STRINGIZE(VERSION_DATE) VERSION_DATE_POSTFIX
 
#define VER_COMPYANYNAME_STR        STRINGIZE(ChessZ Systems)
#define VER_FILE_VERSION            VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION, VERSION_BUILD
#define VER_FILE_VERSION_STR        STRINGIZE(VERSION_MAJOR)        \
                                    "." STRINGIZE(VERSION_MINOR)    \
                                    "." STRINGIZE(VERSION_REVISION) \
                                    "." STRINGIZE(VERSION_BUILD)    \

#define VER_LEGALCOPYRIGHT_STR      STRINGIZE(\x28C\x29ChessZ Systems) " " STRINGIZE(VERSION_YEAR)
#define VER_LEGALCOPYRIGHT_WOS_STR  STRINGIZE((C) ChessZ Systems) " " STRINGIZE(VERSION_YEAR)
#define VER_PRODUCTNAME_STR         STRINGIZE(ChessZ)
