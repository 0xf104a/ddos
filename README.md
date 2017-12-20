# cddos
<b>Compilation</b><br>
```$ gcc main.c ddos.c socket.c message.c util.c ```
<br>
Simple cddos utility<br>
<b>Usage:</b><br>
<p>

./ddos (HOST) (PORT) [options...]<br>
HOST -- host to ddos<br>
PORT -- port to connect,if set to 0 will try random ports<br>

</p>
<b>Options:</b><br>
<p>
-r -- generate random packet<br>
-u -- Use UDP<br>
-p -- Use TCP<br>
-t <THREAD COUNT> -- describes thread count<br>
-s <PACKET SIZE> -- set packet size<br>
--http -- Make http request<br>
--no-warnings -- ignore all warnings<br>
--no-wait -- Do not wait server to respond<br>
--no-check -- Do not check server before starting ddos<br>
</p>
