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
#ifndef QTMINISEEDRECORD_H
#define QTMINISEEDRECORD_H

#include <libmseed.h>
#include <QString>

class QtMiniSEEDRecord
{
    friend class QtMiniSEEDDecoder;
private:
    explicit QtMiniSEEDRecord(MSRecord *record);

public:
    enum DataType { Unknow, Ascii, Int, Float, Double };
    ~QtMiniSEEDRecord();

    const QString network() const;
    const QString station() const;
    const QString channel() const;
    const QString location() const;

    qint32 sequenceNumber() const;
    qint64 startTime() const;
    double sampleRate() const;
    qint32 sampleCount() const;
    DataType sampleType() const;
    qint32 sampleSize() const;

    void unpack(void* data, qint32 offset, qint32 count);
    QByteArray toAscii() const;

    qint32 recordSize() const;
    const char *recordData() const;

private:
    MSRecord *mRecord;

};

#endif // QTMINISEEDRECORD_H
