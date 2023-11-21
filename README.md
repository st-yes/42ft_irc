
<h1 align="center">
  <br>
  <img src="https://github.com/st-yes/42-project-badges/blob/main/badges/ft_ircm.png?raw=true" width="200">
  <br>
  42-FT_IRC
  <br>
</h1>

<h4 align="center">Internet Relay Chat</h4>


<p align="center">
  <a href="#DESCRIPTION">DESCRIPTION</a> •
  <a href="#BUILT WITH">BUILT WITH</a> •
  <a href="#INSTALLATION">INSTALLATION</a> •
  <a href="#USAGE">USAGE</a> •
  <a href="#RESSOURCES">RESSOURCES</a> •
  <a href="#COLLABORATORS">COLLABORATORS</a> •
</p>

## DESCRIPTION
<p>
FT_IRC is a custom implementation of an IRC (Internet Relay Chat server), developed as part of the 42 school curriculum. This project aims to replicate the backbone features of traditional IRC servers, allowing users to connect, join channels, and communicate in real-time. Emphasis is placed on understanding network programming concepts such as: TCP protocol, socket programming... and implementing RFC standards for IRC.<br >
Moreover, the construction of the server was also required to adhere to programming constraints imposed by the project guidelines, such as the use of the C++ 98 standard and restrictions on certain functions.
</p>

## BUILT WITH
![C++](https://img.shields.io/badge/C++-00599C?style=flat-square&logo=C%2B%2B&logoColor=white)

## INSTALLATION
1. Clone the repository to your local machine:
```bash
git clone https://github.com/st-yes/42ft_irc.git
```
2. Navigate to the project's directory:
```bash
cd 42ft_irc
```
3. Compile (with bonus)
```bash
make
```

## USAGE
```css
./ircserv [port] [password]
```
- Replace [port] with the port number you want the server to listen on, and [password] with a password to connect with an irc client
- Use the irssi IRC client or any other IRC client and connect to the server using the specified port.

## RESSOURCES
[**modern irc docs**](https://modern.ircdocs.horse/)   
[**RFC 2810**](https://www.rfc-editor.org/rfc/rfc2810)  
[**RFC 2812**](https://www.rfc-editor.org/rfc/rfc2812)  
[**RFC 2811**](https://www.rfc-editor.org/rfc/rfc2811)  
[**Initialization**](http://chi.cs.uchicago.edu/chirc/index.html?utm_source=pocket_saves)  
[**Used client: IRSSI**](https://irssi.org/)  

## COLLABORATORS
- <a href="https://github.com/mustapha-belbiad">Mustapha Belbiad</a>  
- <a href="https://github.com/4vr3L">Oussama Ouazahrou</a>  
- <a href="https://github.com/st-yes">Soukaina Tyes</a>  
