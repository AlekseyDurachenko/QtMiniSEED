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
    m_record = record;
}

QtMiniSEEDRecord::~QtMiniSEEDRecord()
{
    msr_free(&m_record);
}

void QtMiniSEEDRecord::unpack(void *data, qint32 offset, qint32 count)
{
    qMemCopy(data, &static_cast<char *>(m_record->datasamples)[offset * sampleSize()], sampleSize() * count);
}

QByteArray QtMiniSEEDRecord::toAscii() const
{
    if (sampleType() == Ascii) {
        return QByteArray::fromRawData(reinterpret_cast<const char *>(m_record->datasamples),
                                       sampleSize() * sampleCount());
    }

    return QByteArray();
}

