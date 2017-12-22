# cddos
<b>cddos</b>--program for dosing websites writte on plain c and using pthread library</br>
<b>THIS PROGRAM PROVIDED FOR ONLY EDUCATIONAL PURPOSES</b><br>
<b>Compilation</b><br>
```$ gcc main.c ddos.c socket.c message.c util.c -lpthread -o ddos ```
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
-t <THREAD COUNT> -- describes thread count,by default set to 5<br>
-s <PACKET SIZE> -- set packet size<br>
--http -- Make http request<br>
--no-warnings -- ignore all warnings<br>
--no-wait -- Do not wait server to respond<br>
--no-check -- Do not check server before starting ddos<br>
</p><br>
  
<b>Example usage:</b><br>
``` $ ./ddos some.boring.long.domain.net 80 -t 70 --no-wait --http ```
  <br>
  This will send packets  some.boring.long.domain.net:80 without waiting for response usin http syntax in 70 threads.<br>
``` $./ddos example.com 88 --no-check --no-wait -u ```
  <br>
  With this options program will send  packets to example.com:88 without pre-start host check and without waiting for response.Program will use 5 threads. Program will use UDP protocol.
