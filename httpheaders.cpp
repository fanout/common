/*
 * Copyright (C) 2012-2015 Fanout, Inc.
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

#include "httpheaders.h"

static QList<QByteArray> header_split(const QByteArray &in)
{
	QList<QByteArray> parts = in.split(',');
	for(int n = 0; n < parts.count(); ++n)
		parts[n] = parts[n].trimmed();
	return parts;
}

// search for one of many chars
static int findNext(const QByteArray &in, const char *charList, int start = 0)
{
	int len = qstrlen(charList);
	for(int n = start; n < in.size(); ++n)
	{
		char c = in[n];
		for(int i = 0; i < len; ++i)
		{
			if(c == charList[i])
				return n;
		}
	}

	return -1;
}

bool HttpHeaderParameters::contains(const QByteArray &key) const
{
	for(int n = 0; n < count(); ++n)
	{
		if(qstricmp(at(n).first.data(), key.data()) == 0)
			return true;
	}

	return false;
}

QByteArray HttpHeaderParameters::get(const QByteArray &key) const
{
	for(int n = 0; n < count(); ++n)
	{
		const HttpHeaderParameter &h = at(n);
		if(qstricmp(h.first.data(), key.data()) == 0)
			return h.second;
	}

	return QByteArray();
}

bool HttpHeaders::contains(const QByteArray &key) const
{
	for(int n = 0; n < count(); ++n)
	{
		if(qstricmp(at(n).first.data(), key.data()) == 0)
			return true;
	}

	return false;
}

QByteArray HttpHeaders::get(const QByteArray &key) const
{
	for(int n = 0; n < count(); ++n)
	{
		const HttpHeader &h = at(n);
		if(qstricmp(h.first.data(), key.data()) == 0)
			return h.second;
	}

	return QByteArray();
}

HttpHeaderParameters HttpHeaders::getAsParameters(const QByteArray &key, ParseMode mode) const
{
	QByteArray h = get(key);
	if(h.isEmpty())
		return HttpHeaderParameters();

	return parseParameters(h, mode);
}

QByteArray HttpHeaders::getAsFirstParameter(const QByteArray &key) const
{
	HttpHeaderParameters p = getAsParameters(key);
	if(p.isEmpty())
		return QByteArray();

	return p[0].first;
}

QList<QByteArray> HttpHeaders::getAll(const QByteArray &key, bool split) const
{
	QList<QByteArray> out;

	for(int n = 0; n < count(); ++n)
	{
		const HttpHeader &h = at(n);
		if(qstricmp(h.first.data(), key.data()) == 0)
		{
			if(split)
				out += header_split(h.second);
			else
				out += h.second;
		}
	}

	return out;
}

QList<HttpHeaderParameters> HttpHeaders::getAllAsParameters(const QByteArray &key, ParseMode mode, bool split) const
{
	QList<HttpHeaderParameters> out;

	foreach(const QByteArray &h, getAll(key, split))
	{
		bool ok;
		HttpHeaderParameters params = parseParameters(h, mode, &ok);
		if(ok)
			out += params;
	}

	return out;
}

QList<QByteArray> HttpHeaders::takeAll(const QByteArray &key, bool split)
{
	QList<QByteArray> out;

	for(int n = 0; n < count(); ++n)
	{
		const HttpHeader &h = at(n);
		if(qstricmp(h.first.data(), key.data()) == 0)
		{
			if(split)
				out += header_split(h.second);
			else
				out += h.second;

			removeAt(n);
			--n; // adjust position
		}
	}

	return out;
}

void HttpHeaders::removeAll(const QByteArray &key)
{
	for(int n = 0; n < count(); ++n)
	{
		if(qstricmp(at(n).first.data(), key.data()) == 0)
		{
			removeAt(n);
			--n; // adjust position
		}
	}
}

QByteArray HttpHeaders::join(const QList<QByteArray> &values)
{
	QByteArray out;

	bool first = true;
	foreach(const QByteArray &val, values)
	{
		if(!first)
			out += ", ";

		out += val;
		first = false;
	}

	return out;
}

HttpHeaderParameters HttpHeaders::parseParameters(const QByteArray &in, ParseMode mode, bool *ok)
{
	HttpHeaderParameters out;

	int start = 0;
	if(mode == NoParseFirstParameter)
	{
		int at = in.indexOf(';');
		if(at != -1)
		{
			out += HttpHeaderParameter(in.mid(0, at).trimmed(), QByteArray());
			start = at + 1;
		}
		else
		{
			out += HttpHeaderParameter(in.trimmed(), QByteArray());
			start = in.size();
		}
	}

	while(start < in.size())
	{
		QByteArray var;
		QByteArray val;

		int at = findNext(in, "=;", start);
		if(at != -1)
		{
			var = in.mid(start, at - start).trimmed();
			if(in[at] == '=')
			{
				if(at + 1 >= in.size())
				{
					if(ok)
						*ok = false;
					return HttpHeaderParameters();
				}

				++at;

				if(in[at] == '\"')
				{
					++at;

					bool complete = false;
					for(int n = at; n < in.size(); ++n)
					{
						if(in[n] == '\\')
						{
							if(n + 1 >= in.size())
							{
								if(ok)
									*ok = false;
								return HttpHeaderParameters();
							}

							++n;
							val += in[n];
						}
						else if(in[n] == '\"')
						{
							complete = true;
							at = n + 1;
							break;
						}
						else
							val += in[n];
					}

					if(!complete)
					{
						if(ok)
							*ok = false;
						return HttpHeaderParameters();
					}

					at = in.indexOf(';', at);
					if(at != -1)
						start = at + 1;
					else
						start = in.size();
				}
				else
				{
					int vstart = at;
					at = in.indexOf(';', vstart);
					if(at != -1)
					{
						val = in.mid(vstart, at - vstart).trimmed();
						start = at + 1;
					}
					else
					{
						val = in.mid(vstart).trimmed();
						start = in.size();
					}
				}
			}
			else
				start = at + 1;
		}
		else
		{
			var = in.mid(start).trimmed();
			start = in.size();
		}

		out.append(HttpHeaderParameter(var, val));
	}

	if(ok)
		*ok = true;

	return out;
}
