#!/usr/bin/env python
import sys
import time
import shlex, subprocess
import fcntl, os
import pty
import errno
import re
import select

# master1, slave = pty.openpty()
# slaveName1 = os.ttyname(slave)
# master2, slave = pty.openpty()
# slaveName2 = os.ttyname(slave)
# print '\nslave device names:', slaveName1, slaveName2

LC_CALL_OK         = 0
LC_CALL_WRITE_FAIL = 1
LC_CALL_TIME_OUT   = 2
LC_CALL_READ_ERR   = 3
LC_CALL_PARA_ERR   = 4
LC_CALL_RETURNED   = 5
NOT_WAIT_MATCH="#######"
def p_check_time(start_time, timeout):
    return time.time() >= float(timeout) + start_time
def p_check_match(output, expected):
    if NOT_WAIT_MATCH == expected: return False
    if isinstance(expected, basestring) or not hasattr(expected, "search"):
        expected = re.compile(expected)
    return expected.search(output)
    # return expected in output
def p_check(start_time,output,timeout,expected):
    if p_check_time(start_time, timeout):
        if NOT_WAIT_MATCH == expected:
            return (LC_CALL_OK, expected, output)
        else:
            return (LC_CALL_TIME_OUT, expected, output)

    if p_check_match(output, expected):
        return (LC_CALL_OK, expected, output)
    return None     #not matched

def lc_readfd(afd, expected,timeout=5):
    r_fds = [afd]
    time_begin = time.time()
    outdata = ""      # use bytes type in python3
    time.sleep(0.1)
    while True:
        try:
            rfds, wfds, efds = select.select(r_fds, [], [], 1)
            for rl in rfds:
                if rl in r_fds:
                    data = os.read(rl, 2048)
                    outdata = outdata + data
        except (OSError, IOError) as e:
            if e.errno == errno.EAGAIN:
                time.sleep(0.5)
                continue
            elif e.errno == errno.EIO:
                ret = (LC_CALL_RETURNED, expected, outdata)
                break
            raise(e)
        finally:
            ret = p_check(time_begin,outdata,timeout,expected)
            if ret is not None:
                break
            time.sleep(0.1)
    return (ret[0],ret[1],ret[2], time.time()-time_begin)

def lc_writefd(afd, cmd):
    try:
        w_fds = [afd]
        rfds, wfds, efds = select.select([], w_fds, [], 1)
        for wl in wfds:
            if wl in w_fds:
                size = os.write(wl, cmd)
                return (LC_CALL_OK, "OK")
    except Exception,e:
        return (LC_CALL_PARA_ERR, e.message)
    return (LC_CALL_WRITE_FAIL, "[error]:select afd[%d] not OK!." % afd)

def lc_print(cmd ,ret):
    print("#" * 80)
    print("cmd    :%s" % cmd.strip())
    print("retcode:%d" % ret[0])
    print("expected:%s" % str(ret[1]).strip())
    print("output  :++++++++++++++++++++++++++++++++\n%s<|" % ret[2])
    print("timeused:%f s", ret[3])
    print("=" * 80)

def lc_call(afd, cmd, expected,timeout=5):
    ret = lc_writefd(afd, cmd)
    if not LC_CALL_OK == ret[0]:
        callret = (ret[0], expected, ret[1], 0)
    else:
        callret = lc_readfd(afd, expected, timeout)
    lc_print(cmd, callret)
    return callret

STDIN_FILENO = 0
STDOUT_FILENO = 1
STDERR_FILENO = 2
CHILD = 0
# argv=r"docker exec -i nf-oam /bin/sh"
def lc_spawn(argv):
    if type(argv) == type(''):
        argv = (argv,)
    print argv
    pid, master_fd = pty.fork()
    if( pid == CHILD ):
        # masterNameInChild = os.ttyname(master_fd)
        # print "child:masterNmae[%s], masterFd[%d]" % ("master_fd is Bad file descriptor", master_fd)
        print "subprocess is==>:[%s]" % " ".join(argv)
        os.execlp(argv[0], *argv)
        os.close(master_fd)
        sys.exit(9)
    elif( pid < 0 ):
        raise(Exception("Failed to create child process[%s]" % " ".join(argv)))
        return
    else:
        masterName = os.ttyname(master_fd)
        print "parent:masterNmae[%s], masterFd[%d]" % (masterName, master_fd)
        print("lc_spawn subprocess:%s" % " ".join(argv))
        return pid,master_fd

def p_set_sync(p):
    flags = fcntl.fcntl(p.stdout, fcntl.F_GETFL)
    flags |= os.O_NONBLOCK
    fcntl.fcntl(p.stdout, fcntl.F_SETFL, flags)

def p_call(p, cmd, expected, timeout=5):
    p.stdin.write(cmd)
    time_begin = time.time()
    out_put = ""      # use bytes type in python3
    time.sleep(0.1)
    while True:
        try:
            if p_check_match(out_put, expected):
                ret = (0, expected, out_put)
                break
            if p_check_time(time_begin, timeout):
                ret = (7, expected, out_put)
                break
            tempread = p.stdout.read()
            out_put = out_put + tempread
        except (OSError, IOError) as e:
            if e.errno == errno.EAGAIN:
                continue
            raise
        except Exception, e:
            raise
    print("#"*80)
    print("cmd    :%s" % cmd.strip())
    print("retcode:%d" % ret[0])
    print("expected:%s" % str(ret[1]).strip())
    print("output  :%s<|" % ret[2])
    print("=" * 80)
    return ret

# # argv = (r"ssh",r"root@192.222.199.199")
# argv = shlex.split(r"ssh root@192.222.199.199")
# (pid, mfd) = lc_spawn(argv)
# ret = lc_readfd(mfd, "password:",20)
# lc_print("readfd after[%s]" % " ".join(argv) ,ret)
# lc_call(mfd, "123\n", "ubuntuforITRAN:~#", 5)
# lc_call(mfd, "pwd\n", "ubuntuforITRAN:~#", 5)
# lc_call(mfd, "w\n", "ubuntuforITRAN:~#", 5)
# lc_call(mfd, "ps aux | grep ssh\n", "ubuntuforITRAN:~#", 5)
# lc_call(mfd, "exit\n", "ubuntuforITRAN:~#", 3)
# print("over"*7)


########################################################## pty mode#####################################################
# argv = shlex.split(r"docker exec -it nf-oam /bin/netopeer2-cli.sh")
argv = shlex.split(r"docker exec -it nf-oam /bin/sh")
(pid, mfd) = lc_spawn(argv)
# ret = lc_readfd(mfd, "password:",20)
# lc_print("readfd after[%s]" % " ".join(argv) ,ret)
# lc_call(mfd, "123\n", "ubuntuforITRAN:~#", 5)
lc_call(mfd, "/bin/sysrepoctl.sh -l\n", "Module Name", 5)
lc_call(mfd, "ps\n", "TIME   COMMAND", 5)

print "1"*80
lc_call(mfd, "telnet 192.254.7.1\n", NOT_WAIT_MATCH, 1)
lc_call(mfd, "ps\n", r"/MGR\.EXE", 5)
lc_call(mfd, "exit\n", "Connection closed by foreign host", 5)
print "2"*80

lc_call(mfd, "/bin/netopeer2-cli.sh\n", NOT_WAIT_MATCH, 1)
lc_call(mfd, "help\r", NOT_WAIT_MATCH, 1)
lc_call(mfd, "knownhosts\r", NOT_WAIT_MATCH, 1)
lc_call(mfd, "knownhosts --del 0\r", NOT_WAIT_MATCH, 1)
lc_call(mfd, "connect -p 6001\r", r"\(yes/no\)\?", 1)
lc_call(mfd, "yes\r", r"Password:", 1)
lc_call(mfd, "root123\r", r"> ", 10)
lc_call(mfd, "get-config --source running\r", r"> ", 2)
lc_call(mfd, "knownhosts\r", NOT_WAIT_MATCH, 1)
lc_call(mfd, "help\r", NOT_WAIT_MATCH, 1)
lc_call(mfd, "exit\r", NOT_WAIT_MATCH, 1)
print "3"*80
lc_call(mfd, "telnet 192.254.7.1\n", NOT_WAIT_MATCH, 1)
lc_call(mfd, "ps\n", r"/MGR\.EXE", 5)
lc_call(mfd, "exit\n", "Connection closed by foreign host", 5)
lc_call(mfd, "ps\n", r"netopeer2-server", 5)
sys.exit(0)

############################################# subprocess.Popen mode#####################################################
# /bin/netopeer2-cli.sh
# /bin/sysrepoctl.sh -l
args=r"docker exec -i nf-oam /bin/sh"
# args = shlex.split(r"docker exec -i nf-oam /bin/netopeer2-cli.sh")
print args
p=subprocess.Popen(args,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,shell=True)
p_set_sync(p)
print "0"*80
p_call(p, "/bin/sysrepoctl.sh -l\n", "Module Name", 5)
p_call(p, "ps\n", "TIME   COMMAND", 5)
print "1"*80
p_call(p, "telnet 192.254.7.1\n", NOT_WAIT_MATCH, 1)
p_call(p, "ps\n", r"/MGR\.EXE", 5)
p_call(p, "exit\n", "Connection closed by foreign host", 5)
print "2"*80
p_call(p, "/bin/netopeer2-cli.sh\n", NOT_WAIT_MATCH, 1)
p_call(p, "help\n", NOT_WAIT_MATCH, 1)
p_call(p, "knownhosts\n", NOT_WAIT_MATCH, 1)
p_call(p, "knownhosts --del 0\n", NOT_WAIT_MATCH, 1)
p_call(p, "connect -p 6001\n", r"\(yes/no\)\?", 1)
p_call(p, "yes\n", r"Password:", 1)
p_call(p, "root123\n", r"> ", 10)
p_call(p, "get-config --source running\n", r"> ", 2)
p_call(p, "knownhosts\n", NOT_WAIT_MATCH, 1)
p_call(p, "help\n", NOT_WAIT_MATCH, 1)
p_call(p, "exit\n", NOT_WAIT_MATCH, 1)
print "3"*80
p_call(p, "telnet 192.254.7.1\n", NOT_WAIT_MATCH, 1)
p_call(p, "ps\n", r"/MGR\.EXE", 5)
p_call(p, "exit\n", "Connection closed by foreign host", 5)
p_call(p, "ps\n", r"netopeer2-server", 5)
print "4"*80
print("overover")
