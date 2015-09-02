/*
 * Copyright (C) 2012-2013 Fanout, Inc.
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

#include "zhttpresponsepacket.h"

QVariant ZhttpResponsePacket::toVariant() const
{
	QVariantHash obj;

	if(!from.isEmpty())
		obj["from"] = from;

	if(!id.isEmpty())
		obj["id"] = id;

	QByteArray typeStr;
	switch(type)
	{
		case Error:          typeStr = "error"; break;
		case Credit:         typeStr = "credit"; break;
		case KeepAlive:      typeStr = "keep-alive"; break;
		case Cancel:         typeStr = "cancel"; break;
		case HandoffStart:   typeStr = "handoff-start"; break;
		case HandoffProceed: typeStr = "handoff-proceed"; break;
		case Close:          typeStr = "close"; break;
		case Ping:           typeStr = "ping"; break;
		case Pong:           typeStr = "pong"; break;
		default: break;
	}

	if(!typeStr.isEmpty())
		obj["type"] = typeStr;

	if(type == Error && !condition.isEmpty())
		obj["condition"] = condition;

	if(seq != -1)
		obj["seq"] = seq;

	if(credits != -1)
		obj["credits"] = credits;

	if(more)
		obj["more"] = true;

	if(code != -1)
	{
		obj["code"] = code;
		obj["reason"] = reason;
		QVariantList vheaders;
		foreach(const HttpHeader &h, headers)
		{
			QVariantList vheader;
			vheader += h.first;
			vheader += h.second;
			vheaders += QVariant(vheader);
		}
		obj["headers"] = vheaders;
	}

	if(!body.isNull())
		obj["body"] = body;

	if(!contentType.isEmpty())
		obj["content-type"] = contentType;

	if(userData.isValid())
		obj["user-data"] = userData;

	return obj;
}

bool ZhttpResponsePacket::fromVariant(const QVariant &in)
{
	if(in.type() != QVariant::Hash)
		return false;

	QVariantHash obj = in.toHash();

	from.clear();
	if(obj.contains("from"))
	{
		if(obj["from"].type() != QVariant::ByteArray)
			return false;

		from = obj["from"].toByteArray();
	}

	if(obj.contains("id"))
	{
		if(obj["id"].type() != QVariant::ByteArray)
			return false;

		id = obj["id"].toByteArray();
	}

	type = Data;
	if(obj.contains("type"))
	{
		if(obj["type"].type() != QVariant::ByteArray)
			return false;

		QByteArray typeStr = obj["type"].toByteArray();

		if(typeStr == "error")
			type = Error;
		else if(typeStr == "credit")
			type = Credit;
		else if(typeStr == "keep-alive")
			type = KeepAlive;
		else if(typeStr == "cancel")
			type = Cancel;
		else if(typeStr == "handoff-start")
			type = HandoffStart;
		else if(typeStr == "handoff-proceed")
			type = HandoffProceed;
		else if(typeStr == "close")
			type = Close;
		else if(typeStr == "ping")
			type = Ping;
		else if(typeStr == "pong")
			type = Pong;
		else
			return false;
	}

	if(type == Error)
	{
		condition.clear();
		if(obj.contains("condition"))
		{
			if(obj["condition"].type() != QVariant::ByteArray)
				return false;

			condition = obj["condition"].toByteArray();
		}
	}

	seq = -1;
	if(obj.contains("seq"))
	{
		if(!obj["seq"].canConvert(QVariant::Int))
			return false;

		seq = obj["seq"].toInt();
	}

	credits = -1;
	if(obj.contains("credits"))
	{
		if(!obj["credits"].canConvert(QVariant::Int))
			return false;

		credits = obj["credits"].toInt();
	}

	more = false;
	if(obj.contains("more"))
	{
		if(obj["more"].type() != QVariant::Bool)
			return false;

		more = obj["more"].toBool();
	}

	code = -1;
	if(obj.contains("code"))
	{
		if(!obj["code"].canConvert(QVariant::Int))
			return false;

		code = obj["code"].toInt();
	}

	reason.clear();
	if(obj.contains("reason"))
	{
		if(obj["reason"].type() != QVariant::ByteArray)
			return false;

		reason = obj["reason"].toByteArray();
	}

	headers.clear();
	if(obj.contains("headers"))
	{
		if(obj["headers"].type() != QVariant::List)
			return false;

		foreach(const QVariant &i, obj["headers"].toList())
		{
			QVariantList list = i.toList();
			if(list.count() != 2)
				return false;

			if(list[0].type() != QVariant::ByteArray || list[1].type() != QVariant::ByteArray)
				return false;

			headers += QPair<QByteArray, QByteArray>(list[0].toByteArray(), list[1].toByteArray());
		}
	}

	body.clear();
	if(obj.contains("body"))
	{
		if(obj["body"].type() != QVariant::ByteArray)
			return false;

		body = obj["body"].toByteArray();
	}

	contentType.clear();
	if(obj.contains("content-type"))
	{
		if(obj["content-type"].type() != QVariant::ByteArray)
			return false;

		contentType = obj["content-type"].toByteArray();
	}

	userData = obj["user-data"];

	return true;
}
