import sys

if len(sys.argv) < 2:
	print 'usage: %s [file] [license_type:file] ...' % sys.argv[0]
	sys.exit(1)

licenses = {}
for arg in sys.argv[2:]:
	ltype, fname = arg.split(':')
	f = open(fname, 'r')
	licenses[ltype] = f.read()
	f.close()

f = open(sys.argv[1], 'r')
filedata = f.read()
f.close()

start_mark = '$FANOUT_BEGIN_LICENSE:'
end_mark = '$FANOUT_END_LICENSE$'

start = filedata.find(start_mark)
if start < 0:
	sys.stderr.write('%s: not marked\n' % sys.argv[1])
	sys.exit(1)
start += len(start_mark)
end = filedata.find('$', start)
ltype = filedata[start:end]
start = filedata.find('\n', start)
assert(start >= 0)
start += 1

end = filedata.find(end_mark)
assert(end >= 0)
end = filedata.rfind('\n', 0, end)
assert(end >= 0)
end += 1

newfiledata = filedata[:start] + licenses[ltype] + filedata[end:]
if newfiledata != filedata:
	f = open(sys.argv[1], 'w')
	f.write(newfiledata)
	f.close()
