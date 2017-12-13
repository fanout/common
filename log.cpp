/*
 * Copyright (C) 2012-2016 Fanout, Inc.
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

#include "log.h"

#include <stdio.h>
#include <stdarg.h>
#include <QString>
#include <QTime>
#include <QMutex>

Q_GLOBAL_STATIC(QMutex, g_mutex)
static int g_level = LOG_LEVEL_DEBUG;
static QTime g_time;
static QString *g_filename;
static FILE *g_file;

static void log(const char *s)
{
	FILE *out;
	if(g_file)
		out = g_file;
	else
		out = stdout;
	fprintf(out, "%s\n", s);
	fflush(out);
}

static void log(int level, const char *fmt, va_list ap)
{
	g_mutex()->lock();
	int current_level = g_level;
	int elapsed;
	if(!g_time.isNull())
		elapsed = g_time.elapsed();
	else
		elapsed = -1;
	g_mutex()->unlock();

	if(level <= current_level)
	{
		QString str;
		str.vsprintf(fmt, ap);

		const char *lstr;
		switch(level)
		{
			case LOG_LEVEL_ERROR:   lstr = "ERR"; break;
			case LOG_LEVEL_WARNING: lstr = "WARN"; break;
			case LOG_LEVEL_INFO:    lstr = "INFO"; break;
			case LOG_LEVEL_DEBUG:
			default:
				lstr = "DEBUG"; break;
		}

		QString tstr;
		if(elapsed != -1)
		{
			QTime t(0, 0);
			t = t.addMSecs(elapsed);
			tstr = t.toString("HH:mm:ss.zzz");
		}
		else
		{
			tstr = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
		}

		FILE *out;
		if(g_file)
			out = g_file;
		else
			out = stdout;
		fprintf(out, "[%s] %s %s\n", lstr, qPrintable(tstr), qPrintable(str));
		fflush(out);
	}
}

void log_startClock()
{
	QMutexLocker locker(g_mutex());
	g_time.start();
}

int log_outputLevel()
{
	QMutexLocker locker(g_mutex());
	return g_level;
}

void log_setOutputLevel(int level)
{
	QMutexLocker locker(g_mutex());
	g_level = level;
}

bool log_setFile(const QString &fname)
{
	QMutexLocker locker(g_mutex());
	if(g_file)
	{
		fclose(g_file);
		delete g_filename;
		g_filename = 0;
		g_file = 0;
	}
	if(fname.isEmpty())
		return true;
	FILE *f = fopen(fname.toLocal8Bit().data(), "a");
	if(!f)
		return false;
	setbuf(f, NULL);
	g_filename = new QString(fname);
	g_file = f;
	return true;
}

bool log_rotate()
{
	QMutexLocker locker(g_mutex());
	if(!g_file)
		return true;
	if(!freopen(g_filename->toLocal8Bit().data(), "a", g_file))
		return false;
	setbuf(g_file, NULL);
	return true;
}

void log(int level, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log(level, fmt, ap);
	va_end(ap);
}

void log_error(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log(LOG_LEVEL_ERROR, fmt, ap);
	va_end(ap);
}

void log_warning(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log(LOG_LEVEL_WARNING, fmt, ap);
	va_end(ap);
}

void log_info(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log(LOG_LEVEL_INFO, fmt, ap);
	va_end(ap);
}

void log_debug(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log(LOG_LEVEL_DEBUG, fmt, ap);
	va_end(ap);
}

void log_raw(const char *s)
{
	log(s);
}
