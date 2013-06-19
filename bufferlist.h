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

#ifndef BUFFERLIST_H
#define BUFFERLIST_H

#include <QList>
#include <QByteArray>

class BufferList
{
public:
	BufferList();

	int size() const { return size_; }
	bool isEmpty() const { return size_ == 0; }

	void append(const QByteArray &buf);
	QByteArray take(int size = -1);

	BufferList & operator+=(const QByteArray &buf)
	{
		append(buf);
		return *this;
	}

private:
	QList<QByteArray> bufs_;
	int size_;
	int offset_;
};

#endif
