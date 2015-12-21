// Copyright 2013-2014, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#ifndef QTMINISEEDDECODER_H
#define QTMINISEEDDECODER_H


#include "qtminiseedrecord.h"
#include <QIODevice>


/*!
 * \brief The QgeMiniSeedDecoder provides the functionality to decode the miniseed
 * stream into discret miniseed records. \see QgeMiniSeedRecord
 */
class QtMiniSEEDDecoder
{
public:
    enum Error {
        NoError = 0,
        FewDataError,
        FormatError,
        ReadError,
        UnexpectedOffsetError,
        UnexpectedEndError,
        UnknowError = 128
    };

    explicit QtMiniSEEDDecoder(qint64 expectedSize = 4096, bool blocked = true);
    virtual ~QtMiniSEEDDecoder();

    QtMiniSEEDRecord *readRecord(QIODevice *device);
    QtMiniSEEDRecord *readRecord(const char *data, int size, int *position);

    void reset();

    inline int lastError() const;
    QString errorString() const;

    static QtMiniSEEDRecord *decode(char *data, int size, QString *reason = 0);
    static QtMiniSEEDRecord *decode(const char *data, int size, QString *reason = 0);
    static QtMiniSEEDRecord *decode(QIODevice *device, int size, QString *reason = 0);

private:
    void resize();
    QtMiniSEEDRecord *iteration();

private:
    qint64 m_expectedSize;
    qint64 m_realRecordLen;
    char *m_record;
    Error m_lastError;
    bool m_isBlocked;

    qint64 offset;
    // current read processor state
    qint64 asize;
    qint64 rsize;
    qint8 is_end;
    qint8 bitswp;
    // element type
    // 0: FSDH
    // 1: BLKHDR
    // 2: DATA
    // 3: 1000 blk
    qint8 r_type;
};

/*!
 * \brief QgeMiniSeedDecoder::lastError
 * \return last error of the stream processing
 */
int QtMiniSEEDDecoder::lastError() const
{
    return m_lastError;
}


#endif // QTMINISEEDDECODER_H
