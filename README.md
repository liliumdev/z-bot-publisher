# z-bot-publisher

Basic anti-leak system with simple encryption for delivery of Z-Bot to end-users. Consists of an application which generates a GUID for the end-user, which the end-user then needs to send to the admin. The admin then XOR-encrypts the Z-Bot binary using the end-user's GUID as a key and uploads it to server. The end-user then uses a delivery application which downloads his encrypted binary from the server, decrypts it using his GUID and finally runs Z-Bot. 

Developed around 2010 and 2011. It's now on github only for archival and educational purposes.