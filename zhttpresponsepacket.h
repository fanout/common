/*
 * Copyright (C) 2012-2016 Fanout, Inc.
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

#ifndef ZHTTPRESPONSEPACKET_H
#define ZHTTPRESPONSEPACKET_H

#include <QVariant>
#include "httpheaders.h"

class ZhttpResponsePacket
{
public:
	enum Type
	{
		Data,
		Error,
		Credit,
		KeepAlive,
		Cancel,
		HandoffStart,
		HandoffProceed,
		Close, // WebSocket
		Ping, // WebSocket
		Pong // WebSocket
	};

	QByteArray from;
	QList<QByteArray> ids;

	Type type;
	QByteArray condition;
	int seq;

	int credits;
	bool more;

	int code;
	QByteArray reason;
	HttpHeaders headers;
	QByteArray body;

	QByteArray contentType; // WebSocket

	QVariant userData;

	bool multi;

	ZhttpResponsePacket() :
		type((Type)-1),
		seq(-1),
		credits(-1),
		more(false),
		code(-1),
		multi(false)
	{
	}

	QVariant toVariant() const;
	bool fromVariant(const QVariant &in);
};

#endif
