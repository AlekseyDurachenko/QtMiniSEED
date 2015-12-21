// Copyright 2013-2015, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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


#include "qtminiseedrecorddata.h"
#include <QString>


class QtMiniSeedRecord
{
    friend class QtMiniSeedDecoder;
private:
    explicit QtMiniSeedRecord(MSRecord *record);
public:
    QtMiniSeedRecord();
    QtMiniSeedRecord(const QtMiniSeedRecord &other);
    enum DataType { Unknow, Ascii, Int, Float, Double };

    inline bool isNull() const;

    inline const QString network() const;
    inline const QString station() const;
    inline const QString channel() const;
    inline const QString location() const;

    inline qint32 sequenceNumber() const;
    inline qint64 startTime() const;
    inline double sampleRate() const;
    inline qint32 sampleCount() const;
    inline DataType sampleType() const;
    inline qint32 sampleSize() const;

    QByteArray toAscii() const;

    inline qint32 recordSize() const;
    inline const char *recordData() const;

private:
    QExplicitlySharedDataPointer<QtMiniSeedRecordData> d;
};

bool QtMiniSeedRecord::isNull() const
{
    return (d->msrecord == 0);
}

const QString QtMiniSeedRecord::network() const
{
    return d->msrecord->network;
}

const QString QtMiniSeedRecord::station() const
{
    return d->msrecord->station;
}

const QString QtMiniSeedRecord::channel() const
{
    return d->msrecord->channel;
}

const QString QtMiniSeedRecord::location() const
{
    return d->msrecord->location;
}

qint32 QtMiniSeedRecord::sequenceNumber() const
{
    return d->msrecord->sequence_number;
}

qint64 QtMiniSeedRecord::startTime() const
{
    return d->msrecord->starttime;
}

double QtMiniSeedRecord::sampleRate() const
{
    return d->msrecord->samprate;
}

qint32 QtMiniSeedRecord::sampleCount() const
{
    return d->msrecord->samplecnt;
}

QtMiniSeedRecord::DataType QtMiniSeedRecord::sampleType() const
{
    switch (d->msrecord->sampletype) {
    case 'a':
        return Ascii;
    case 'i':
        return Int;
    case 'f':
        return Float;
    case 'd':
        return Double;
    }
    return Unknow;
}

qint32 QtMiniSeedRecord::sampleSize() const
{
    switch (d->msrecord->sampletype) {
    case 'a':
        return 1;
    case 'i':
    case 'f':
        return 4;
    case 'd':
        return 8;
    }
    return 0;
}

qint32 QtMiniSeedRecord::recordSize() const
{
    return d->msrecord->reclen;
}

const char *QtMiniSeedRecord::recordData() const
{
    return d->msrecord->record;
}


#endif // QTMINISEEDRECORD_H
