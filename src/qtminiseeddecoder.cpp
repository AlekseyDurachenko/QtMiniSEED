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
#include "qtminiseeddecoder.h"


typedef struct __blkhdr_t {
    quint16 type;
    quint16 offset;
}
blkhdr_t;


QtMiniSEEDDecoder::QtMiniSEEDDecoder(qint64 expectedSize, bool blocked)
{
    m_expectedSize = expectedSize;
    m_record = new char[expectedSize];
    m_isBlocked = blocked;

    reset();
}

QtMiniSEEDDecoder::~QtMiniSEEDDecoder()
{
    delete []m_record;
}

QtMiniSEEDRecord *QtMiniSEEDDecoder::readRecord(QIODevice *device)
{
    // algorithm
    try {
        while (device->bytesAvailable()) {
            resize();

            qint64 toRead = rsize - asize;
            if (device->bytesAvailable() < toRead)
                toRead = device->bytesAvailable();

            if (device->read(&m_record[offset + asize], toRead) != toRead)
                throw ReadError;

            asize += toRead;
            if (rsize == asize) {
                asize = 0;
                QtMiniSEEDRecord *record = iteration();
                if (record)
                    return record;
            }
        }

        throw FewDataError;
    }
    catch (Error error) {
        m_lastError = error;
    }

    // error
    return 0;
}

QtMiniSEEDRecord *QtMiniSEEDDecoder::readRecord(const char *data, int size, int *position)
{
    // algorithm
    try {
        while (*position < size) {
            resize();

            qint64 toRead = rsize - asize;
            qint64 avaible = size - *position;
            if (avaible < toRead)
                toRead = avaible;

            qMemCopy(&m_record[offset + asize], &data[*position], toRead);
            *position += toRead;

            asize += toRead;
            if (rsize == asize) {
                asize = 0;
                QtMiniSEEDRecord *record = iteration();
                if (record)
                    return record;
            }
        }

        throw FewDataError;
    }
    catch (Error error) {
        m_lastError = error;
    }

    // error
    return 0;
}

void QtMiniSEEDDecoder::reset()
{
    offset = 0;
    rsize = sizeof(fsdh_s);
    asize = 0;
    is_end = 0;
    bitswp = 0;
    r_type = 0;
    m_lastError = NoError;
}

QString QtMiniSEEDDecoder::errorString() const
{
    switch (lastError()) {
    case NoError:
        return QObject::tr("No Error", "QgeMiniSeedRecord");
    case FewDataError:
        return QObject::tr("Need more data", "QgeMiniSeedRecord");
    }

    return QString();
}

QtMiniSEEDRecord *QtMiniSEEDDecoder::decode(char *data, int size, QString *reason)
{
    Q_UNUSED(reason);

    MSRecord *msrec = 0;
    msr_unpack(data, size, &msrec, true, 1);
    return new QtMiniSEEDRecord(msrec);
}

QtMiniSEEDRecord *QtMiniSEEDDecoder::decode(const char *data, int size, QString *reason)
{
    char *tmp = new char[size];
    memcpy(tmp, data, size);
    return decode(tmp, size, reason);
}

QtMiniSEEDRecord *QtMiniSEEDDecoder::decode(QIODevice *device, int size, QString *reason)
{
    Q_UNUSED(reason);

    if (device->bytesAvailable() >= size) {
        MSRecord *msrec = 0;
        char *data = new char[size];
        device->read(data, size);
        msr_unpack(data, size, &msrec, true, 1);
        delete []data;
        return new QtMiniSEEDRecord(msrec);
    }

    return 0;
}

void QtMiniSEEDDecoder::resize()
{
    m_realRecordLen = offset + rsize;
    if ((m_realRecordLen  > m_expectedSize && !is_end) || (m_realRecordLen  != m_expectedSize && is_end)) {
        while (m_expectedSize < m_realRecordLen)
            m_expectedSize *= 2;

        char *record = 0;
        record = new char[m_expectedSize];
        qMemCopy(record, m_record, offset);
        delete [] m_record;
        m_record = record;
    }
}

QtMiniSEEDRecord *QtMiniSEEDDecoder::iteration()
{
    // current ptr for quick access
    char *curptr = &m_record[offset];
    offset += rsize;

    // MiniSEED record complitly readed
    if (is_end == 1) {
        // reset to ready next record
        reset();

        // create the miniseed record
        MSRecord *msrec = 0;
        msr_unpack(m_record, m_realRecordLen, &msrec, true, 1);
        return new QtMiniSEEDRecord(msrec);
    }
    // FSDH readed
    else if (r_type == 0) {
        // check for correct FSDH
        if (!MS_ISVALIDHEADER(curptr))
            throw FormatError;

        // next: blkhdr
        r_type = 1;
        rsize = sizeof(blkhdr_t);

        // Check byteswap needed by year
        if ((((fsdh_s *)curptr)->start_time.year < 1900) || (((fsdh_s *)curptr)->start_time.year > 2050))
            bitswp = 1;
    }
    // blkhdr readed
    else if (r_type == 1) {
        quint16 hdr_type = ((blkhdr_t *)curptr)->type;
        quint16 hdr_offset = ((blkhdr_t *)curptr)->offset;

        // bitswap if needed
        if (bitswp) {
            ms_gswap2(&hdr_type);
            ms_gswap2(&hdr_offset);
        }

        //
        if (hdr_type == 1000) {
            // start read the 1000blk
            r_type = 3;
            //rsize = hdr_offset - offset;
            rsize = sizeof(blkt_1000_s);
        }
        else if (hdr_offset != 0) {
            // next blocket offset cannot point early than the current offset
            if (hdr_offset < offset)
                throw UnexpectedOffsetError;

            // start read the raw data
            r_type = 2;
            rsize = hdr_offset - offset;
        }
        else {
            throw UnexpectedEndError;
        }
    }
    // data readed
    else if (r_type == 2) {
        r_type = 1;
        rsize = sizeof(blkhdr_t);
    }
    // blk1000 breaded
    else if (r_type == 3) {
        is_end = 1;
        rsize = (1 << ((blkt_1000_s *)curptr)->reclen) - offset;
    }

    return 0;
}
