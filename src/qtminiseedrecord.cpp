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
#include "qtminiseedrecord.h"

QtMiniSEEDRecord::QtMiniSEEDRecord(MSRecord *record)
{
    mRecord = record;
}

QtMiniSEEDRecord::~QtMiniSEEDRecord()
{
    msr_free(&mRecord);
}

const QString QtMiniSEEDRecord::network() const
{
    return mRecord->network;
}

const QString QtMiniSEEDRecord::station() const
{
    return mRecord->station;
}

const QString QtMiniSEEDRecord::channel() const
{
    return mRecord->channel;
}

const QString QtMiniSEEDRecord::location() const
{
    return mRecord->location;
}

qint32 QtMiniSEEDRecord::sequenceNumber() const
{
    return mRecord->sequence_number;
}

qint64 QtMiniSEEDRecord::startTime() const
{
    return mRecord->starttime;
}

double QtMiniSEEDRecord::sampleRate() const
{
    return mRecord->samprate;
}

qint32 QtMiniSEEDRecord::sampleCount() const
{
    return mRecord->samplecnt;
}

QtMiniSEEDRecord::DataType QtMiniSEEDRecord::sampleType() const
{
    switch (mRecord->sampletype)
    {
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
    switch (mRecord->sampletype)
    {
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

void QtMiniSEEDRecord::unpack(void *data, qint32 offset, qint32 count)
{
    qMemCopy(data, &static_cast<char *>(mRecord->datasamples)[offset*sampleSize()], sampleSize() * count);
}

QByteArray QtMiniSEEDRecord::toAscii() const
{
    if (sampleType() == Ascii)
    {
        return QByteArray::fromRawData(reinterpret_cast<const char*>(mRecord->datasamples),
                sampleSize() * sampleCount());
    }

    return QByteArray();
}

qint32 QtMiniSEEDRecord::recordSize() const
{
    return mRecord->reclen;
}

const char *QtMiniSEEDRecord::recordData() const
{
    return mRecord->record;
}
