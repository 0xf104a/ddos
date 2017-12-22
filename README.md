# cddos
<b>THIS PROGRAM PROVIDED FOR ONLY EDUCATIONAL PURPOSES</b><br>
<b>Compilation</b><br>
```$ gcc main.c ddos.c socket.c message.c util.c -o ddos ```
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
</p><br>
  <b>Example usage:</b><br>
  ```$ ./ddos some.boring.long.domain.net 80 --t 70 --no-wait --http```
  This will send packets  some.boring.long.domain.net:80 without waiting for response usin http syntax
