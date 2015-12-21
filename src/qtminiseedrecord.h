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

    void unpack(void *data, qint32 offset, qint32 count);
    QByteArray toAscii() const;

    inline qint32 recordSize() const;
    inline const char *recordData() const;

private:
    MSRecord *m_record;
};

const QString QtMiniSEEDRecord::network() const
{
    return m_record->network;
}

const QString QtMiniSEEDRecord::station() const
{
    return m_record->station;
}

const QString QtMiniSEEDRecord::channel() const
{
    return m_record->channel;
}

const QString QtMiniSEEDRecord::location() const
{
    return m_record->location;
}

qint32 QtMiniSEEDRecord::sequenceNumber() const
{
    return m_record->sequence_number;
}

qint64 QtMiniSEEDRecord::startTime() const
{
    return m_record->starttime;
}

double QtMiniSEEDRecord::sampleRate() const
{
    return m_record->samprate;
}

qint32 QtMiniSEEDRecord::sampleCount() const
{
    return m_record->samplecnt;
}

QtMiniSEEDRecord::DataType QtMiniSEEDRecord::sampleType() const
{
    switch (m_record->sampletype) {
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

qint32 QtMiniSEEDRecord::sampleSize() const
{
    switch (m_record->sampletype) {
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

qint32 QtMiniSEEDRecord::recordSize() const
{
    return m_record->reclen;
}

const char *QtMiniSEEDRecord::recordData() const
{
    return m_record->record;
}

#endif // QTMINISEEDRECORD_H
