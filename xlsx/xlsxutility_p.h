/****************************************************************************
** Copyright (c) 2013-2014 Debao Zhang <hello@debao.me>
** All right reserved.
**
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the
** "Software"), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
** NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
** LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
** OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
** WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
****************************************************************************/
#ifndef XLSXUTILITY_H
#define XLSXUTILITY_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt Xlsx API.  It exists for the convenience
// of the Qt Xlsx.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include "xlsxglobal.h"
class QPoint;
class QString;
#if (QT_VERSION<0x060000)
class QStringList;
#endif
class QColor;
class QDateTime;
class QTime;

namespace QXlsx {
class CellReference;

XLSX_AUTOTEST_EXPORT bool parseXsdBoolean(const QString &value, bool defaultValue=false);

XLSX_AUTOTEST_EXPORT QStringList splitPath(const QString &path);
XLSX_AUTOTEST_EXPORT QString getRelFilePath(const QString &filePath);

XLSX_AUTOTEST_EXPORT double datetimeToNumber(const QDateTime &dt, bool is1904=false);
XLSX_AUTOTEST_EXPORT QDateTime datetimeFromNumber(double num, bool is1904=false);
XLSX_AUTOTEST_EXPORT double timeToNumber(const QTime &t);

XLSX_AUTOTEST_EXPORT QString createSafeSheetName(const QString &nameProposal);
XLSX_AUTOTEST_EXPORT QString escapeSheetName(const QString &sheetName);
XLSX_AUTOTEST_EXPORT QString unescapeSheetName(const QString &sheetName);

XLSX_AUTOTEST_EXPORT bool isSpaceReserveNeeded(const QString &string);

XLSX_AUTOTEST_EXPORT QString convertSharedFormula(const QString &rootFormula, const CellReference &rootCell, const CellReference &cell);

} //QXlsx
#endif // XLSXUTILITY_H
