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

#ifndef LOG_H
#define LOG_H

#include <QString>

// really simply logging stuff

#define LOG_LEVEL_ERROR   0
#define LOG_LEVEL_WARNING 1
#define LOG_LEVEL_INFO    2
#define LOG_LEVEL_DEBUG   3

void log_startClock();
int log_outputLevel();
void log_setOutputLevel(int level);
bool log_setFile(const QString &fname);
bool log_rotate();

void log_error(const char *fmt, ...);
void log_warning(const char *fmt, ...);
void log_info(const char *fmt, ...);
void log_debug(const char *fmt, ...);

// log without prefixing or anything. useful for forwarding log data
void log_raw(const char *line);

#endif
