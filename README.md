# cddos
Simple cddos utility<br>
<b>Usage:</b><br>
<p>
./ddos <HOST> <PORT> [options...]
HOST -- host to ddos
PORT -- port to connect,if set to 0 will try random ports
</p>
<b>Options:</b><br>
<p>
-r -- generate random packet
-u -- Use UDP
-p -- Use TCP
-t <THREAD COUNT> -- describes thread count
-s <PACKET SIZE> -- set packet size
--http -- Make http request
--no-warnings -- ignore all warnings
--no-wait -- Do not wait server to respond
--no-check -- Do not check server before starting ddos
</p>
