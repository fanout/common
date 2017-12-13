/*
 * Copyright (C) 2013 Fanout, Inc.
 * 
 * $FANOUT_BEGIN_LICENSE:GPL$
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Alternatively, this program may be used under the terms of a commercial
 * license, where the commercial license agreement is provided with the
 * software or contained in a written agreement between you and Fanout. For
 * further information use the contact form at <https://fanout.io/enterprise/>.
 *
 * $FANOUT_END_LICENSE$
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

	QByteArray mid(int pos, int size = -1) const;

	void clear();
	void append(const QByteArray &buf);
	QByteArray take(int size = -1);

	QByteArray toByteArray(); // non-const because we rewrite the list

	BufferList & operator+=(const QByteArray &buf)
	{
		append(buf);
		return *this;
	}

private:
	QList<QByteArray> bufs_;
	int size_;
	int offset_;

	void findPos(int pos, int *bufferIndex, int *offset) const;
};

#endif
