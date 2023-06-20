### user command syntax
- USER <username> <hostname> <servername> <realname>  
-  the username and nickname (nick) are separate concepts
- though they may or may not be the same  
- Nickname (nick): It is the unique identifier that represents a user in the IRC network. Nicknames are used for communication and identifying users in channels.  
- Username: It is a username associated with the client's connection or account. it is used for authentication purposes

### skeleton architecture
- this project consist mainly on assuring a communication between an irc official client and an irc server
- server send a message (command + parameters)
- client send a reply

### the 3 pertinent rfcs:
- [RFC2810] Internet Relay Chat: Architecture
- [RFC2812] Internet Relay Chat: Client Protocol (how to reply to client)
- [RFC2811] Internet Relay Chat: Channel Management

### message format
a message contains at least 2 part: command + first parameter, separated by a single ASCII space
<small>There may be at most 15 parameters</small>
- COMMAND PARAM1 PARAM2 PARAM3
- When the last parameter is prefixed with a colon character, the value of that parameter will be the remainder of the message (including space characters). 


### ping pong 
. IRC clients will sent PING messages periodically and, if they do not receive a PONG message back, they will close the connection.

<small><strong>thinking about hashing the password, for no package sniffing</strong></small>

### user command
- [RFC1459] (an old version of 2812) IRSSI use this one 
	USER <username> <hostname> <servername> <realname>
- [RFC2812]  
	USER <user> <mode> <unused> <realname>