## Memcached server ips file
In this directory file with public memcached servers is stored.<br>
Originally takne from https://github.com/SecOps-Institute/memcached-server-iplist

## Format
Example:<br>
```
127.0.0.1
0.0.0.0
192.168.0.0
192.168.0.1
```
## Automatic formatting
prepare.py could format your file automtically.It'll delete all lines that are not supported<br>
Usage:<br>
```./prepare.py <ipfile>```<br>
Example:<br>
```
$ ./prepare.py ipfile
[+]:Total lines:58482
[+]:Unsupported lines:874
[*]:Rewritng file...
[+]:Done in 0.13s
```

