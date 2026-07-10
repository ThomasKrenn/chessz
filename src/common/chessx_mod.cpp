#include <build_inc.h>
#include "chessx_mod.hpp"

void chessxPrintVersionInfo(QString & versionString) {
    versionString = QString::fromLatin1(VER_DOCCREATOR);
    versionString += QStringLiteral("\n") + QString::fromLatin1(VER_LEGALCOPYRIGHT_WOS_STR);
}

const QLatin1String chessxCreator() {
    return QLatin1String(VER_DOCCREATOR);
}

const QLatin1String chessxCompany() {
    return QLatin1String(VER_LEGALCOPYRIGHT_WOS_STR);
}

const QLatin1String chessxApplicationName() {
    return QLatin1String(VER_FILEDESCRIPTION_STR);
}

const QLatin1String chessxApplicationVersion() {
    return QLatin1String(VER_FILE_VERSION_STR);
}

const QLatin1String chessxFileDescription() {
    return QLatin1String(VER_FILEDESCRIPTION_STR);
}