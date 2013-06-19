/*
 * Copyright (C) 2013 Fanout, Inc.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "bufferlist.h"

BufferList::BufferList() :
	size_(0),
	offset_(0)
{
}

void BufferList::append(const QByteArray &buf)
{
	if(buf.size() < 1)
		return;

	bufs_ += buf;
	size_ += buf.size();
}

QByteArray BufferList::take(int size)
{
	if(size == 0)
		return QByteArray();

	int toRead;
	if(size > 0)
		toRead = qMin(size, size_);
	else
		toRead = size_;

	// if we're reading the exact size of the first buffer, cheaply
	//   return it
	if(bufs_.first().size() == toRead)
		return bufs_.takeFirst();

	QByteArray out;
	out.resize(toRead);
	char *outp = out.data();

	while(toRead > 0)
	{
		QByteArray &buf = bufs_.first();
		int bsize = qMin(buf.size() - offset_, toRead);
		memcpy(outp, buf.data() + offset_, bsize);

		if(offset_ + bsize >= buf.size())
		{
			bufs_.removeFirst();
			offset_ = 0;
		}
		else
			offset_ += bsize;

		toRead -= bsize;
		size_ -= bsize;
		outp += bsize;
	}

	return out;
}
