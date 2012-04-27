#!/usr/bin/env python

import CGIHTTPServer
import BaseHTTPServer
import cgitb; cgitb.enable()

server = BaseHTTPServer.HTTPServer
handler = CGIHTTPServer.CGIHTTPRequestHandler
server_address = ("", 8000)
handler.cgi_directories = ["http://csweb.stuy.edu/~mrudoy/Assassin/"]

httpd = server(server_address, handler)
httpd.serve_forever()
