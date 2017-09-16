README.txt

jrmetzger
Jonathan Metzger
9/12/17
CS3516

*** Project 1 ***

Testing:

In a terminal under the proj1 directory:


********************************
********************************

CLIENT TO WEB

$ ./client [-p] [url] [port]
EXAMPLE: ./client -p www.cnn.com 80

===== Project 1 ====

Server Response
----------------
HTTP/1.1 200 OK
access-control-allow-origin: *
cache-control: max-age=60
content-security-policy: default-src 'self' blob: https://*.cnn.com:* http://*.cnn.com:* *.cnn.io:* *.cnn.net:* *.turner.com:* *.turner.io:* *.ugdturner.com:* courageousstudio.com *.vgtf.net:*; script-src 'unsafe-eval' 'unsafe-inline' 'self' *; style-src 'unsafe-inline' 'self' blob: *; child-src 'self' blob: *; frame-src 'self' *; object-src 'self' *; img-src 'self' data: blob: *; media-src 'self' blob: *; font-src 'self' data: *; connect-src 'self' *; frame-ancestors 'self' *.cnn.com:* *.turner.com:* courageousstudio.com;
Content-Type: text/html; charset=utf-8
x-content-type-options: nosniff
x-servedByHost: ::ffff:172.17.102.10
x-xss-protection: 1; mode=block
Content-Length: 140838
Accept-Ranges: bytes
Date: Tue, 12 Sep 2017 03:15:13 GMT
Via: 1.1 varnish
Age: 183
Connection: keep-alive
Set-Cookie: countryCode=US; Domain=.cnn.com; Path=/
Set-Cookie: geoData=oxford|MA|01540|US|NA; Domain=.cnn.com; Path=/
X-Served-By: cache-iad2636-IAD
X-Cache: HIT
X-Cache-Hits: 5
X-Timer: S1505186113.464260,VS0,VE0
Vary: Accept-Encoding, Fastly-SSL

[RTT is 198 milliseconds] <-- only with the "-p" option

Saved to index.html

========== EOF ==========

********************************
********************************

SERVER:

$ ./server [port]
EXAMPLE: ./server 5555

==== Welcome to the Server =====


Listening . . .



********************************

CLIENT:

$ ./client [-p] localhost/index.html [port]
EXAMPLE: ./client -p localhost/index.html 5555


===== Project 1 ====

Server Response
----------------

HTTP/1.1 200 OK
Date: Tue Sep 12 11:55:13 2017
Server: Apache/2.4.27 (Unix)
Content-Length: 58327
Content-Type: text/html; charset=utf-8

[RTT is 82 milliseconds] <-- only with the "-p" option

Saved to index.html

========== EOF ==========

********************************

SERVER:

==== Welcome to the Server =====

Listening . . .
Request Received for: /index.html

Listening . . .


********************************
